#include "Server.h"
#include "interface.h"
#include "interface_constants.h"
#include "System.h"
#include "crc.h"
#include "ffmpeg_utils.h"
#include <boost/bind.hpp>

class server_session : public boost::enable_shared_from_this<server_session>
{
public:
    explicit server_session(
            boost::asio::io_service &io_service,
            AVFormatContext *ofmt_ctx,
            AVCodecContext *enc_ctx,
            SwrContext *resample_context,
            AVAudioFifo *audio_fifo,
            int64_t *pts)
            : socket_(io_service),
              channel_id_(0),
              ofmt_ctx_(ofmt_ctx),
              enc_ctx_(enc_ctx),
              resample_context_(resample_context),
              audio_fifo_(audio_fifo),
              pts_(pts)
    {
    }

    boost::asio::ip::tcp::socket &socket()
    {
        return socket_;
    }

    void start(void)
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(input_buffer_, MESSAGE_HEAD_LENGTH),
                                boost::bind(
                                        &server_session::handle_read_header, shared_from_this(),
                                        boost::asio::placeholders::error));
    }

    void handle_read_header(const boost::system::error_code &error)
    {
        if (!error) {
            if (!info_.parse_message_head(input_buffer_)) {
                do_close();
                printf("Error message head.\n");
                return;
            }

            boost::asio::async_read(socket_,
                                    boost::asio::buffer(&input_buffer_[MESSAGE_HEAD_LENGTH],
                                                        info_.get_message_body_length()),
                                    boost::bind(&server_session::handle_read_body, shared_from_this(),
                                                boost::asio::placeholders::error));
        } else {
            do_close();
        }
    }

    void handle_read_body(const boost::system::error_code &error)
    {
        if (!error) {
            if (!info_.parse_message(input_buffer_)) {
                do_close();
                return;
            }

            switch (info_.get_message_type()) {
                case interface::MESSAGE_CHANNEL_SETUP:
                    process_channel_setup();
                    break;
                case interface::MESSAGE_CHANNEL_TEST:
                    process_channel_test();
                    break;
                case interface::MESSAGE_CHANNEL_STATUS:
                    process_channel_status();
                    break;
                case interface::MESSAGE_CHANNEL_ERROR:
                    process_channel_error();
                    break;
                case interface::MESSAGE_CHANNEL_CLOSE:
                    process_channel_close();
                    break;
                case interface::MESSAGE_PCM_DATA_SEND_REQUEST:
                    process_pcm_data_send_request();
                    break;
                default:
                    process_default();
            }
        } else {
            do_close();
        }
    }

    void handle_write(const boost::system::error_code &error)
    {
        if (!error) {
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(input_buffer_, MESSAGE_HEAD_LENGTH),
                                    boost::bind(
                                            &server_session::handle_read_header, shared_from_this(),
                                            boost::asio::placeholders::error));
        } else {
            do_close();
        }
    }

    void do_close()
    {
        socket_.close();
    }

private:
    boost::asio::ip::tcp::socket socket_;
    unsigned char input_buffer_[128 * 1024]{};
    unsigned char output_buffer_[128 * 1024]{};
    interface::Interface info_;
    //temp variables
    unsigned short channel_id_;
    static const int MAX_PCM_DATA_LENGTH = 32 * 1024;
    //ffmpeg variables
    AVFormatContext *ofmt_ctx_;
    AVCodecContext *enc_ctx_;
    SwrContext *resample_context_;
    AVAudioFifo *audio_fifo_;
    int64_t *pts_;

    //help functions
    void send_data(const unsigned char *send_buffer, const unsigned int len)
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(send_buffer, len),
                                 boost::bind(&server_session::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error));
    }

    void process_channel_setup()
    {
        if (!info_.get_channel_id(channel_id_)) {
            return;
        }
        const auto pkg_len = interface::channel_status(interface::PROTOCOL_VERSION, channel_id_, output_buffer_);
        send_data(output_buffer_, pkg_len);

    }

    void process_channel_test()
    {
        const auto pkg_len = interface::channel_status(interface::PROTOCOL_VERSION, channel_id_, output_buffer_);
        send_data(output_buffer_, pkg_len);
    }

    void process_channel_status()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(input_buffer_, MESSAGE_HEAD_LENGTH),
                                boost::bind(
                                        &server_session::handle_read_header, shared_from_this(),
                                        boost::asio::placeholders::error));
    }

    void process_channel_close()
    {
        do_close();
    }

    void process_channel_error()
    {
        printf("Received a error message, break the channel.\n");
        do_close();
    }

    void process_pcm_data_send_request()
    {
        unsigned int block_id;
        uint32_t crc;
        if (info_.get_channel_id(channel_id_)) return;
        if (info_.get_block_id(block_id)) return;
        if (info_.get_crc(crc)) return;
        unsigned short pcm_length;
        unsigned char pcm_data[MAX_PCM_DATA_LENGTH];
        int pkg_len;
        if (!info_.get_block_data(pcm_length, pcm_data))
        {
            pkg_len = interface::channel_error(interface::PROTOCOL_VERSION, channel_id_, interface::ERROR_NO_PAYLOAD, output_buffer_);
            send_data(output_buffer_, pkg_len);
            return;
        }
        if (!validate_crc(pcm_data, pcm_length))
        {
            pkg_len = interface::channel_error(interface::PROTOCOL_VERSION, channel_id_, interface::ERROR_CRC_INVALID, output_buffer_);
            send_data(output_buffer_, pkg_len);
            return;
        }
        //now we got the data, todo process
        const int nb_bytes = pcm_length - 4;
        const auto nb_samples = nb_bytes / 4; //we only support 2 channel stereo
        uint8_t **converted_input_samples = nullptr;
        if (init_converted_samples(&converted_input_samples, enc_ctx_, nb_samples))
        {
            printf("Can't init converted samples.\n");
            pkg_len = interface::channel_error(interface::PROTOCOL_VERSION, channel_id_, interface::ERROR_UNKNOWN_ERROR, output_buffer_);
            send_data(output_buffer_, pkg_len);
            return;
        }
        /**
		* Convert the input samples to the desired output sample format.
		* This requires a temporary storage provided by converted_input_samples.
		*/
        if (convert_samples((const uint8_t **)&pcm_data, converted_input_samples,
                            nb_samples, resample_context_))
        {
            free_converted_samples(converted_input_samples);
            pkg_len = interface::channel_error(interface::PROTOCOL_VERSION, channel_id_, interface::ERROR_UNKNOWN_ERROR, output_buffer_);
            send_data(output_buffer_, pkg_len);
            return;
        }

        /** Add the converted input samples to the FIFO buffer for later processing. */
        if (add_samples_to_fifo(audio_fifo_, converted_input_samples,
                                nb_samples))
        {
            free_converted_samples(converted_input_samples);
            pkg_len = interface::channel_error(interface::PROTOCOL_VERSION, channel_id_, interface::ERROR_UNKNOWN_ERROR, output_buffer_);
            send_data(output_buffer_, pkg_len);
            return;
        }
        free_converted_samples(converted_input_samples);

        const auto output_frame_size = enc_ctx_->frame_size;
        while (av_audio_fifo_size(audio_fifo_) > output_frame_size) //we have enough data
        {
            /** Temporary storage of the output samples of the frame written to the file. */
            AVFrame *output_frame;
            /** Initialize temporary storage for one output frame. */
            if (init_output_frame(&output_frame, enc_ctx_, output_frame_size))
            {
                break;
            }
            /**
            * Read as many samples from the FIFO buffer as required to fill the frame.
            * The samples are stored in the frame temporarily.
            */
            if (av_audio_fifo_read(audio_fifo_, reinterpret_cast<void **>(output_frame->data), output_frame_size) < output_frame_size) {
                fprintf(stderr, "Could not read data from FIFO\n");
                av_frame_free(&output_frame);
                break;
            }


            output_frame->pts = *pts_;
            *pts_ += output_frame->nb_samples;
            auto error = avcodec_send_frame(enc_ctx_, output_frame);
            av_frame_free(&output_frame);
            if (error < 0)
            {
                break;
            }
            while (!error)
            {
                /** Packet used for temporary storage. */
                AVPacket output_packet;
                init_packet(&output_packet);
                error = avcodec_receive_packet(enc_ctx_, &output_packet);
                if (!error)
                {
                    output_packet.stream_index = ofmt_ctx_->streams[0]->index;
                    av_packet_rescale_ts(&output_packet, enc_ctx_->time_base, ofmt_ctx_->streams[0]->time_base);
                    av_interleaved_write_frame(ofmt_ctx_, &output_packet);
                    av_packet_unref(&output_packet);
                }
            }
        }
        pkg_len = interface::pcm_data_send_response(interface::PROTOCOL_VERSION, channel_id_, block_id, output_buffer_);
        send_data(output_buffer_, pkg_len);
    }

    void process_default()
    {
        const auto pkg_len = interface::channel_error(interface::PROTOCOL_VERSION, channel_id_,
                                                      interface::ERROR_UNKNOWN_MESSAGE_TYPE, output_buffer_);
        send_data(output_buffer_, pkg_len);
    }

};

Server::Server(boost::asio::io_service &io_service,
               const boost::asio::ip::tcp::endpoint &endpoint)
        : is_working_(false),
          io_service_(io_service),
          acceptor_(io_service, endpoint),
          audio_fifo_(nullptr),
          resample_context_(nullptr),
          enc_ctx_(nullptr),
          ofmt_ctx_(nullptr),
          pts_(0)
{
}

Server::~Server()
{
    if (audio_fifo_)
        av_audio_fifo_free(audio_fifo_);
    swr_free(&resample_context_);
    if (enc_ctx_)
        avcodec_free_context(&enc_ctx_);
    if (ofmt_ctx_) {
        avio_closep(&ofmt_ctx_->pb);
        avformat_free_context(ofmt_ctx_);
    }
}

void Server::start_accept()
{
    session_ptr new_session(new server_session(io_service_, ofmt_ctx_, enc_ctx_, resample_context_ , audio_fifo_, &pts_));
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&Server::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}

int Server::open(void)
{
    if (!open_output_file())
    {
        printf("Can't init output file: %s.\n", System::instance()->output_url().c_str());
        return -1;
    }
    printf("Start audio server at %d.\n", System::instance()->server_port());
    is_working_ = true;
    start_accept();
    return 0;
}

void Server::close()
{
    is_working_ = false;
    acceptor_.close();
}

void Server::handle_accept(session_ptr session,
                           const boost::system::error_code &error)
{
    if (!error) {
        session->start();
    }
    if (is_working_) {
        start_accept();
    }
}

unsigned short pack_crc(unsigned char *cache, const unsigned short pkg_len)
{
    const unsigned int crc = crc32(cache, pkg_len);
    cache[pkg_len] = static_cast<unsigned char>(crc >> 24);
    cache[pkg_len + 1] = static_cast<unsigned char>(crc >> 16);
    cache[pkg_len + 2] = static_cast<unsigned char>(crc >> 8);
    cache[pkg_len + 3] = static_cast<unsigned char>(crc);
    return pkg_len + 4;
}

bool validate_crc(unsigned char *cache, unsigned short pkg_len)
{
    const unsigned int crc = crc32(cache, pkg_len - 4);
    return crc == static_cast<unsigned int>((cache[pkg_len - 4] << 24) + (cache[pkg_len - 3] << 16) + (cache[pkg_len - 2] << 8) + cache[
            pkg_len - 1]);
}

bool Server::open_output_file()
{
    ofmt_ctx_ = NULL;
    avformat_alloc_output_context2(&ofmt_ctx_, NULL, "mpegts", System::instance()->output_url().c_str());
    if (!ofmt_ctx_)
    {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context.\n");
        return false;
    }
    if (System::instance()->codec_name() == "aac"){
        ofmt_ctx_->audio_codec_id = AV_CODEC_ID_AAC;
    }
    else if (System::instance()->codec_name() == "mp3"){
        ofmt_ctx_->audio_codec_id = AV_CODEC_ID_MP3;
    }
    else {
        av_log(NULL, AV_LOG_ERROR, "Unsupported codec name: %s.\n", System::instance()->codec_name().c_str());
        return false;
    }
    ofmt_ctx_->duration = INT64_MAX;
    //add stream to context
    auto out_stream = avformat_new_stream(ofmt_ctx_, nullptr);
    if (!out_stream)
    {
        av_log(nullptr, AV_LOG_ERROR, "Failed allocating output stream.\n");
        return false;
    }

    out_stream->index = 0;
    out_stream->id = 0;

    auto encoder = avcodec_find_encoder(ofmt_ctx_->audio_codec_id);
    if (!encoder)
    {
        av_log(nullptr, AV_LOG_FATAL, "Necessary codec not found.\n");
        return false;
    }
    auto enc_ctx = avcodec_alloc_context3(encoder);
    if (!enc_ctx)
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to allocate the encoder context.\n");
        return false;
    }
    enc_ctx_ = enc_ctx;
    enc_ctx->sample_rate = System::instance()->sample_rate();
    enc_ctx->bit_rate = System::instance()->bitrate();
    enc_ctx->channel_layout = AV_CH_LAYOUT_STEREO;
    enc_ctx->channels = av_get_channel_layout_nb_channels(enc_ctx->channel_layout);
    enc_ctx->sample_fmt = encoder->sample_fmts[0];
    enc_ctx->time_base = {1, enc_ctx->sample_rate};

    if (avcodec_open2(enc_ctx, encoder, nullptr) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open encoder for stream #%u.\n", out_stream->id);
        return false;
    }

    if (avcodec_parameters_from_context(out_stream->codecpar, enc_ctx))
    {
        av_log(nullptr, AV_LOG_ERROR, "Failed to copy encoder parameters to output steam.");
        return false;
    }

    out_stream->time_base = enc_ctx->time_base;
    //encoder_ = encoder;

    if (init_resampler(enc_ctx->sample_fmt, AV_SAMPLE_FMT_S16, enc_ctx->channel_layout, enc_ctx->sample_rate, &resample_context_))
    {
        av_log(nullptr, AV_LOG_ERROR, "Failed to init resampler");
        return false;
    }

    //set program
    const int id = 1;
    auto program = av_new_program(ofmt_ctx_, id);
    program->discard = AVDISCARD_NONE;
    program->flags = 0;
    program->program_num = id;
    program->nb_stream_indexes = 1;
    program->stream_index = new unsigned int[program->nb_stream_indexes];
    program->stream_index[0] = 0;
    av_dict_set(&program->metadata, "service_name", "channel one", 0);
    av_dict_set(&program->metadata, "service_provider", "telecast", 0);
    program->start_time = 0;
    program->end_time = 0;

    if (!(ofmt_ctx_->oformat->flags & AVFMT_NOFILE)) {
        int ret = avio_open(&ofmt_ctx_->pb, System::instance()->output_url().c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", System::instance()->output_url().c_str());
            return false;
        }
    }
    /* init muxer, write output file header */
    const auto ret = avformat_write_header(ofmt_ctx_, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return false;
    }
    return true;
}


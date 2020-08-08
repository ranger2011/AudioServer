#ifndef AUDIOSERVER_SERVER_H
#define AUDIOSERVER_SERVER_H
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

//ffmpeg support
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
#include <libswresample/swresample.h>
}

class server_session;
typedef boost::shared_ptr<server_session> session_ptr;

class Server
{
public:
	explicit Server(boost::asio::io_service& io_service,
                    const boost::asio::ip::tcp::endpoint& endpoint);
    ~Server();
    int open(void);

    void close();
private:
    volatile bool is_working_;
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;

	AVFormatContext *ofmt_ctx_;
	AVCodecContext *enc_ctx_;
	//AVCodec *encoder_;
	SwrContext *resample_context_;
	AVAudioFifo *audio_fifo_;
	int64_t pts_;
	bool open_output_file();
	void start_accept();
	void handle_accept(session_ptr session,
		const boost::system::error_code& error);
}
;
unsigned short pack_crc(unsigned char *cache, const unsigned short pkg_len);
bool validate_crc(unsigned char *cache, unsigned short pkg_len);
#endif
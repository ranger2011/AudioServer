//
// Created by ranger on 2020/7/30.
//

#include "ffmpeg_utils.h"

void init_packet(AVPacket * packet)
{
    av_init_packet(packet);
    /** Set the packet data and size so that it is recognized as being empty. */
    packet->data = nullptr;
    packet->size = 0;
}

int init_input_frame(AVFrame ** frame)
{
    if (!((*frame = av_frame_alloc()))) {
        fprintf(stderr, "Could not allocate input frame\n");
        return AVERROR(ENOMEM);
    }
    return 0;
}

int init_fifo(AVAudioFifo ** fifo, AVSampleFormat fmt, int channels)
{
    /** Create the FIFO buffer based on the specified output sample format. */
    if (!((*fifo = av_audio_fifo_alloc(fmt, channels, 1)))) {
        fprintf(stderr, "Could not allocate FIFO\n");
        return AVERROR(ENOMEM);
    }
    return 0;
}

int init_resampler(
        const AVSampleFormat out_sample_fmt,
        const AVSampleFormat in_sample_fmt,
        const int64_t ch_layout,
        const int sample_rate,
        SwrContext ** resample_context)
{
    int error;
    *resample_context = swr_alloc_set_opts(
            nullptr,
            ch_layout,
            out_sample_fmt,
            sample_rate,
            ch_layout,
            in_sample_fmt,
            sample_rate,
            0,
            nullptr
    );
    if (!*resample_context) {
        fprintf(stderr, "Could not allocate resample context\n");
        return AVERROR(ENOMEM);
    }
    /** Open the resampler with the specified parameters. */
    if ((error = swr_init(*resample_context)) < 0) {
        fprintf(stderr, "Could not open resample context\n");
        swr_free(resample_context);
        return error;
    }
    return 0;
}

int init_converted_samples(uint8_t *** converted_input_samples, AVCodecContext * output_codec_context, const int frame_size)
{
    int error;

    /**
    * Allocate as many pointers as there are audio channels.
    * Each pointer will later point to the audio samples of the corresponding
    * channels (although it may be NULL for interleaved formats).
    */
    if (!((*converted_input_samples = static_cast<uint8_t **>(calloc(output_codec_context->channels,
                                                                     sizeof(**converted_input_samples)))))) {
        fprintf(stderr, "Could not allocate converted input sample pointers\n");
        return AVERROR(ENOMEM);
    }

    /**
    * Allocate memory for the samples of all channels in one consecutive
    * block for convenience.
    */
    if ((error = av_samples_alloc(*converted_input_samples, nullptr,
                                  output_codec_context->channels,
                                  frame_size,
                                  output_codec_context->sample_fmt, 0)) < 0) {
        av_freep(&(*converted_input_samples)[0]);
        free(*converted_input_samples);
        return error;
    }
    return 0;
}

int convert_samples(const uint8_t ** input_data, uint8_t ** converted_data, const int frame_size, SwrContext * resample_context)
{
    int error;

    /** Convert the samples using the resampler. */
    if ((error = swr_convert(resample_context,
                             converted_data, frame_size,
                             input_data, frame_size)) < 0) {
        return error;
    }

    return 0;
}

int add_samples_to_fifo(AVAudioFifo * fifo, uint8_t ** converted_input_samples, const int frame_size)
{
    int error;

    /**
    * Make the FIFO as large as it needs to be to hold both,
    * the old and the new samples.
    */
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + frame_size)) < 0) {
        fprintf(stderr, "Could not reallocate FIFO\n");
        return error;
    }

    /** Store the new samples in the FIFO buffer. */
    if (av_audio_fifo_write(fifo, (void **)converted_input_samples,
                            frame_size) < frame_size) {
        fprintf(stderr, "Could not write data to FIFO\n");
        return AVERROR_EXIT;
    }
    return 0;
}

int init_output_frame(AVFrame ** frame, AVCodecContext * output_codec_context, const int frame_size)
{
    int error;

    /** Create a new frame to store the audio samples. */
    if (!((*frame = av_frame_alloc()))) {
        fprintf(stderr, "Could not allocate output frame\n");
        return AVERROR_EXIT;
    }

    /**
    * Set the frame's parameters, especially its size and format.
    * av_frame_get_buffer needs this to allocate memory for the
    * audio samples of the frame.
    * Default channel layouts based on the number of channels
    * are assumed for simplicity.
    */
    (*frame)->nb_samples = frame_size;
    (*frame)->channel_layout = output_codec_context->channel_layout;
    (*frame)->format = output_codec_context->sample_fmt;
    (*frame)->sample_rate = output_codec_context->sample_rate;

    /**
    * Allocate the samples of the created frame. This call will make
    * sure that the audio frame can hold as many samples as specified.
    */
    if ((error = av_frame_get_buffer(*frame, 0)) < 0) {
        av_frame_free(frame);
        return error;
    }

    return 0;
}


void free_converted_samples(uint8_t **converted_input_samples)
{
    if (converted_input_samples) {
        av_freep(&converted_input_samples[0]);
        free(converted_input_samples);
    }
}
//
// Created by ranger on 2020/7/30.
//

#ifndef AUDIOSERVER_FFMPEG_UTILS_H
#define AUDIOSERVER_FFMPEG_UTILS_H
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/audio_fifo.h>
#include <libswresample/swresample.h>
};
void init_packet(AVPacket *packet);
int init_input_frame(AVFrame **frame);
int init_fifo(AVAudioFifo **fifo, AVSampleFormat fmt, int channels);
int init_resampler(
        AVSampleFormat out_sample_fmt,
        AVSampleFormat in_sample_fmt,
        int64_t ch_layout,
        int sample_rate,
        SwrContext **resample_context
);
int init_converted_samples(uint8_t ***converted_input_samples,
                           AVCodecContext *output_codec_context,
                           int frame_size);
/**
* Convert the input audio samples into the output sample format.
* The conversion happens on a per-frame basis, the size of which is specified
* by frame_size.
*/
int convert_samples(const uint8_t **input_data,
                    uint8_t **converted_data, const int frame_size,
                    SwrContext *resample_context);
/** Add converted input audio samples to the FIFO buffer for later processing. */
int add_samples_to_fifo(AVAudioFifo *fifo,
                        uint8_t **converted_input_samples,
                        const int frame_size);
int init_output_frame(AVFrame **frame,
                      AVCodecContext *output_codec_context,
                      int frame_size);

void free_converted_samples(uint8_t **converted_input_samples);
#endif //AUDIOSERVER_FFMPEG_UTILS_H

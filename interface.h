//
// Created by ranger on 2020/7/30.
//

#ifndef AUDIOSERVER_INTERFACE_H
#define AUDIOSERVER_INTERFACE_H

#include "protocol.h"
#include <stdint.h>

namespace interface {
    int channel_setup(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    );
    int channel_test(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    );
    int channel_status(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    );
    int channel_close(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    );
    int channel_error(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned short error_code,
            unsigned char *output_buffer
    );
    int pcm_data_send_request(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned int block_id,
            const unsigned char *block_data,
            unsigned short data_length,
            uint32_t crc,
            unsigned char *output_buffer
    );
    int pcm_data_send_response(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned int block_id,
            unsigned char *output_buffer
            );

    class Interface : public CommonMessage {
    public:
        Interface();
        ~Interface();
        bool get_channel_id(unsigned short &channel_id);
        bool get_block_id(unsigned int &block_id);
        bool get_block_data(unsigned short &length, unsigned char *data);
        bool get_crc(uint32_t &crc);
        bool get_error_code(unsigned short &error_code);
    };
}

#endif //AUDIOSERVER_INTERFACE_H

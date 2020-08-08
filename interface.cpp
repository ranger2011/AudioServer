//
// Created by ranger on 2020/7/30.
//

#include "interface.h"
#include "interface_constants.h"
#include "pack_data.h"
namespace interface
{
    int channel_setup(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    )
    {
        return package_message_common(
                output_buffer,
                protocol_version, MESSAGE_CHANNEL_SETUP,
                PARAMETER_CHANNEL_ID, channel_id, 0
        );
    }

    int channel_test(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    )
    {
        return package_message_common(
                output_buffer,
                protocol_version, MESSAGE_CHANNEL_TEST,
                PARAMETER_CHANNEL_ID, channel_id, 0
        );
    }

    int channel_status(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    )
    {
        return package_message_common(
                output_buffer,
                protocol_version, MESSAGE_CHANNEL_STATUS,
                PARAMETER_CHANNEL_ID, channel_id, 0
        );
    }

    int channel_close(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned char *output_buffer
    )
    {
        return package_message_common(
                output_buffer,
                protocol_version, MESSAGE_CHANNEL_CLOSE,
                PARAMETER_CHANNEL_ID, channel_id, 0
        );
    }

    int channel_error(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned short error_code,
            unsigned char *output_buffer
    )
    {
        return package_message_common(
                output_buffer,
                protocol_version, MESSAGE_CHANNEL_ERROR,
                PARAMETER_CHANNEL_ID, channel_id, 0,
                PARAMETER_ERROR_CODE, error_code, 0
        );
    }

    int pcm_data_send_request(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned int block_id,
            const unsigned char *block_data,
            unsigned short data_length,
            uint32_t crc,
            unsigned char *output_buffer
            )
    {
        return package_message_common(
                output_buffer,
                protocol_version, MESSAGE_PCM_DATA_SEND_REQUEST,
                PARAMETER_CHANNEL_ID, channel_id, 0,
                PARAMETER_BLOCK_ID, block_id, 0,
                PARAMETER_BLOCK_DATA, block_data, data_length,
                PARAMETER_CRC, crc, 0
                );
    }

    int pcm_data_send_response(
            unsigned char protocol_version,
            unsigned short channel_id,
            unsigned int block_id,
            unsigned char *output_buffer
    )
    {
        return package_message_common(
                output_buffer,
                protocol_version, MESSAGE_PCM_DATA_SEND_RESPONSE,
                PARAMETER_CHANNEL_ID, channel_id, 0,
                PARAMETER_BLOCK_ID, block_id, 0
                );
    }

    Interface::Interface()
        = default;

    Interface::~Interface() noexcept
        = default;

    bool Interface::get_channel_id(unsigned short &channel_id)
    {
        return get_parameter_value(PARAMETER_CHANNEL_ID, channel_id);
    }
    bool Interface::get_block_id(unsigned int &block_id)
    {
        return get_parameter_value(PARAMETER_BLOCK_ID, block_id);
    }
    bool Interface::get_block_data(unsigned short &length, unsigned char *data)
    {
        return get_parameter_content(PARAMETER_BLOCK_DATA, length, data);
    }
    bool Interface::get_crc(uint32_t &crc)
    {
        return get_parameter_value(PARAMETER_CRC, crc);
    }
    bool Interface::get_error_code(unsigned short &error_code)
    {
        return get_parameter_value(PARAMETER_ERROR_CODE, error_code);
    }
}
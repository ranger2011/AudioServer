#include "protocol.h"
#include <memory.h>

int package_message_head( unsigned char *output_buffer, unsigned char protocol_version, unsigned short message_type )
{
	output_buffer[0] = protocol_version;
	output_buffer[1] = (unsigned char)(message_type >> 8);
	output_buffer[2] = (unsigned char)message_type;
	output_buffer[3] = 0;
	output_buffer[4] = 0;
	return MESSAGE_HEAD_LENGTH;
}

int unpack_message_head( const unsigned char *input_buffer, unsigned char &protocol_version, unsigned short &message_type, unsigned short &message_length )
{
    protocol_version = input_buffer[0];
    message_type = (input_buffer[1] << 8) + input_buffer[2];
    message_length = (input_buffer[3] << 8) + input_buffer[4];
    return MESSAGE_HEAD_LENGTH;
}

int package_message_parameter_content(unsigned char *output_buffer, unsigned short parameter_type, const unsigned char *parameter_value, unsigned short parameter_length)
{
	unsigned short length = (output_buffer[3] << 8) + output_buffer[4] + MESSAGE_HEAD_LENGTH;
	output_buffer[length++] = (unsigned char)(parameter_type >> 8);
	output_buffer[length++] = (unsigned char)parameter_type;
	output_buffer[length++] = (unsigned char)(parameter_length >> 8);
	output_buffer[length++] = (unsigned char)parameter_length;
	for (unsigned short i = 0; i < parameter_length; ++i)
	{
		output_buffer[length++] = parameter_value[i];
	}
	length  -= MESSAGE_HEAD_LENGTH;
	output_buffer[3] = (unsigned char)(length >> 8);
	output_buffer[4] = (unsigned char)length;
 	return MESSAGE_HEAD_LENGTH + length;
}

int unpack_parameter( const unsigned char *input_buffer, unsigned short &parameter_type, unsigned short &parameter_length )
{
    parameter_type = (input_buffer[0] << 8) + input_buffer[1];
    parameter_length = (input_buffer[2] << 8) + input_buffer[3];
    return parameter_length + 4;
}

template <>
bool unpack_parameterT<unsigned char>(const unsigned char *input_buffer, unsigned short parameter_type, unsigned char &parameter_value)
{
    unsigned short current_type = 0;
    unsigned short current_length = 0;
    unpack_parameter(input_buffer, current_type, current_length);
    if (current_type != parameter_type)
    {
        return false;
    }
    parameter_value = input_buffer[PARAMETER_HEAD_LENGTH];
    return true;
}

template <>
bool unpack_parameterT<unsigned short>(const unsigned char *input_buffer, unsigned short parameter_type, unsigned short &parameter_value)
{
    unsigned short current_type = 0;
    unsigned short current_length = 0;
    unpack_parameter(input_buffer, current_type, current_length);
    if (current_type != parameter_type)
    {
        return false;
    }
    parameter_value = (input_buffer[PARAMETER_HEAD_LENGTH] << 8) +
        input_buffer[PARAMETER_HEAD_LENGTH + 1];
    return true;
}

template <>
bool unpack_parameterT<short>(const unsigned char *input_buffer, unsigned short parameter_type, short &parameter_value)
{
    unsigned short current_type = 0;
    unsigned short current_length = 0;
    unpack_parameter(input_buffer, current_type, current_length);
    if (current_type != parameter_type)
    {
        return false;
    }
    parameter_value = (input_buffer[PARAMETER_HEAD_LENGTH] << 8) +
        input_buffer[PARAMETER_HEAD_LENGTH + 1];
    return true;
}

template <>
bool unpack_parameterT<unsigned int>(const unsigned char *input_buffer, unsigned short parameter_type, unsigned int &parameter_value)
{
    unsigned short current_type = 0;
    unsigned short current_length = 0;
    unpack_parameter(input_buffer, current_type, current_length);
    if (current_type != parameter_type)
    {
        return false;
    }
    parameter_value = (input_buffer[PARAMETER_HEAD_LENGTH] << 24) +
        (input_buffer[PARAMETER_HEAD_LENGTH + 1] << 16) +
        (input_buffer[PARAMETER_HEAD_LENGTH + 2] << 8) +
        input_buffer[PARAMETER_HEAD_LENGTH + 3];
    return true;
}

template <>
bool unpack_parameterT<int>(const unsigned char *input_buffer, unsigned short parameter_type, int &parameter_value)
{
    unsigned short current_type = 0;
    unsigned short current_length = 0;
    unpack_parameter(input_buffer, current_type, current_length);
    if (current_type != parameter_type)
    {
        return false;
    }
    parameter_value = (input_buffer[PARAMETER_HEAD_LENGTH] << 24) +
        (input_buffer[PARAMETER_HEAD_LENGTH + 1] << 16) +
        (input_buffer[PARAMETER_HEAD_LENGTH + 2] << 8) +
        input_buffer[PARAMETER_HEAD_LENGTH + 3];
    return true;
}

template <>
int package_message_parameter_template<unsigned char>(unsigned char *output_buffer, unsigned short parameter_type, unsigned char parameter_value, unsigned short)
{
	return package_message_parameter_content(output_buffer, parameter_type, &parameter_value, 1);
}

template <>
int package_message_parameter_template<const unsigned char*>(unsigned char *output_buffer, unsigned short parameter_type, const unsigned char* parameter_value, unsigned short parameter_length)
{
    return package_message_parameter_content(output_buffer, parameter_type, parameter_value, parameter_length);
}

template <>
int package_message_parameter_template<unsigned short>(unsigned char *output_buffer, unsigned short parameter_type, unsigned short parameter_value, unsigned short)
{
	unsigned char cache[8];
	cache[0] = (unsigned char)(parameter_value >> 8);
	cache[1] = (unsigned char)parameter_value;
	return package_message_parameter_content(output_buffer, parameter_type, cache, 2);
}

template <>
int package_message_parameter_template<short>(unsigned char *output_buffer, unsigned short parameter_type, short parameter_value, unsigned short)
{
	unsigned char cache[8];
	cache[0] = (unsigned char)(parameter_value >> 8);
	cache[1] = (unsigned char)parameter_value;
	return package_message_parameter_content(output_buffer, parameter_type, cache, 2);
}

template <>
int package_message_parameter_template<unsigned int>(unsigned char *output_buffer, unsigned short parameter_type, unsigned int parameter_value, unsigned short)
{
	unsigned char cache[8];
	cache[0] = (unsigned char)(parameter_value >> 24);
	cache[1] = (unsigned char)(parameter_value >> 16);
	cache[2] = (unsigned char)(parameter_value >> 8);
	cache[3] = (unsigned char)parameter_value;
	return package_message_parameter_content(output_buffer, parameter_type, cache, 4);
}

template <>
int package_message_parameter_template<int>(unsigned char *output_buffer, unsigned short parameter_type, int parameter_value, unsigned short)
{
	unsigned char cache[8];
	cache[0] = (unsigned char)(parameter_value >> 24);
	cache[1] = (unsigned char)(parameter_value >> 16);
	cache[2] = (unsigned char)(parameter_value >> 8);
	cache[3] = (unsigned char)parameter_value;
	return package_message_parameter_content(output_buffer, parameter_type, cache, 4);
}

CommonMessage::CommonMessage()
    :message_type_(0)
{
}

CommonMessage::~CommonMessage()
= default;

bool CommonMessage::parse_message_head(const unsigned char *message_head)
{
	unpack_message_head(message_head, protocol_version_, message_type_, message_body_length_);
	return message_body_length_ < MAX_MESSAGE_LENGTH - MESSAGE_HEAD_LENGTH;
}

bool CommonMessage::parse_message(const unsigned char *message)
{
    unpack_message_head(message, protocol_version_, message_type_, message_body_length_);
	if (message_body_length_ >= MAX_MESSAGE_LENGTH - MESSAGE_HEAD_LENGTH)
	{
		return false;
	}
	if (!parameter_positions_.empty())
	{
		parameter_positions_.clear();
	}
    
    unsigned int byte_count = MESSAGE_HEAD_LENGTH;
    parameter_positions_.push_back(byte_count);
	try
	{
		do 
    {
        unsigned short parameter_type = 0;
        unsigned short parameter_length = 0;
        byte_count += unpack_parameter(&message[byte_count], parameter_type, parameter_length);
        parameter_positions_.push_back(byte_count);
    } while (message_body_length_ > (byte_count - MESSAGE_HEAD_LENGTH));
    if ((MESSAGE_HEAD_LENGTH + message_body_length_) != byte_count)
    {
        return false;
    }
	}
    catch (...)
    {
		return false;
    }
    memcpy(message_data_, message, byte_count);
    return true;
}

bool CommonMessage::get_parameter_content(unsigned short parameter_type, unsigned short &content_size, unsigned char *content, unsigned int index)const
{
    unsigned short current_type = 0;
    unsigned short current_length = 0;
    unsigned int count = 0;
    for (auto itr = parameter_positions_.begin(); itr != parameter_positions_.end(); ++itr)
    {
        unpack_parameter(&message_data_[*itr], current_type, current_length);
		if (current_length > MAX_PARAMETER_LENGTH)return false;
        if (parameter_type == current_type)
        {
            if (count == index)
            {
                content_size = current_length;
                memcpy(content, &message_data_[*itr + PARAMETER_HEAD_LENGTH], content_size);
                return true;
            }
            ++count;
        }
    }
    return false;
}

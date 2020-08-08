//
// Created by ranger on 2020/7/30.
//

#ifndef AUDIOSERVER_INTERFACE_CONSTANTS_H
#define AUDIOSERVER_INTERFACE_CONSTANTS_H

namespace interface
{
    static const unsigned char PROTOCOL_VERSION = 1;
    static const unsigned int MAX_DATA_BLOCK_SIZE = 16 * 1024;
    //message types
    static const unsigned short MESSAGE_CHANNEL_SETUP = 0X10;
    static const unsigned short MESSAGE_CHANNEL_TEST = 0X11;
    static const unsigned short MESSAGE_CHANNEL_STATUS = 0X12;
    static const unsigned short MESSAGE_CHANNEL_CLOSE = 0X13;
    static const unsigned short MESSAGE_CHANNEL_ERROR = 0X14;
    static const unsigned short MESSAGE_PCM_DATA_SEND_REQUEST = 0X20;
    static const unsigned short MESSAGE_PCM_DATA_SEND_RESPONSE = 0X21;
    //parameters
    static const unsigned short PARAMETER_CHANNEL_ID = 0X01;
    static const unsigned short PARAMETER_BLOCK_ID = 0X02;
    static const unsigned short PARAMETER_BLOCK_DATA = 0X03;
    static const unsigned short PARAMETER_CRC = 0X04;
    static const unsigned short PARAMETER_ERROR_CODE = 0X05;
    //error code
    static const unsigned short ERROR_UNSUPPORTED_PROTOCOL_VERSION = 0X01;
    static const unsigned short ERROR_NO_PAYLOAD = 0X02;
    static const unsigned short ERROR_CRC_INVALID = 0X03;
    static const unsigned short ERROR_REPEATED_BLOCK = 0X04;
    static const unsigned short ERROR_UNKNOWN_MESSAGE_TYPE = 0X05;
    static const unsigned short ERROR_UNKNOWN_ERROR = 0X7000;
}

#endif //AUDIOSERVER_INTERFACE_CONSTANTS_H

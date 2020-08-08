//
// Created by ranger on 2020/7/30.
//

#ifndef AUDIOSERVER_SYSTEM_H
#define AUDIOSERVER_SYSTEM_H
#include <string>

using namespace std;
class System {
public:
    static System *instance();
    unsigned short server_port() const
    {
        return server_port_;
    }
    string output_url()const
    {
        return output_url_;
    }
    string codec_name()const
    {
        return codec_name_;
    }
    int bitrate()const
    {
        return bitrate_;
    }
    int bits_per_sample()const
    {
        return bits_per_sample_;
    }
    int sample_rate()const
    {
        return sample_rate_;
    }
private:
    System();
    unsigned short server_port_;
    string output_url_;
    string codec_name_;
    unsigned int bitrate_;
    unsigned int channels_;
    unsigned int bits_per_sample_;
    unsigned int sample_rate_;

    static const char *FILENAME;
};


#endif //AUDIOSERVER_SYSTEM_H

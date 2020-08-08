//
// Created by ranger on 2020/7/30.
//

#include "System.h"
#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost;

const char *System::FILENAME = "config.json";

System *System::instance()
{
    static System obj;
    return &obj;
}

System::System()
        : server_port_(5678),
          output_url_("test.ts"),
          codec_name_("mp3"),
          bitrate_(64000),
          channels_(2),
          bits_per_sample_(16),
          sample_rate_(48000)
{
    property_tree::ptree root;
    try {
        property_tree::read_json(FILENAME, root);
        server_port_ = root.get<unsigned short>("server_port");
        output_url_ = root.get<string>("output_url");
        codec_name_ = root.get<string>("codec_name");
        bitrate_ = root.get<unsigned int>("bitrate");
        channels_ = root.get<unsigned int>("channels");
        bits_per_sample_ = root.get<unsigned int>("bits_per_sample");
        sample_rate_ = root.get<unsigned int>("sample_rate");
    }
    catch (const std::exception &) {
        //we have defaults, so if exception occurred, just ignore it
    }
}

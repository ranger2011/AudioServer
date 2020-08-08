#ifndef _GS_PROTOCOL_H_
#define _GS_PROTOCOL_H_
#include <vector>

static const unsigned int MESSAGE_HEAD_LENGTH = 5;
static const unsigned int PARAMETER_HEAD_LENGTH = 4;
// data package functions

int package_message_head(unsigned char *output_buffer, unsigned char protocol_version, unsigned short message_type);

template <typename T> int package_message_parameter_template(unsigned char *output_buffer, unsigned short parameter_type, T parameter_value, unsigned short parameter_length);
// data unpackage functions
int unpack_message_head(const unsigned char *input_buffer, unsigned char &protocol_version, unsigned short &message_type, unsigned short &message_length);
int unpack_parameter(const unsigned char *input_buffer, unsigned short &parameter_type, unsigned short &parameter_length);

template <typename T>
int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type, T t, unsigned short parameter_length
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    return package_message_parameter_template(output_buffer, parameter_type, t, parameter_length);
}

template <typename T1, typename T2>
int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    return package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
}

template <typename T1, typename T2, typename T3>
int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    return package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
}

template <typename T1, typename T2, typename T3, typename T4>
int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3,
    unsigned short parameter_type4, T4 t4, unsigned short parameter_length4
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
    return package_message_parameter_template(output_buffer, parameter_type4, t4, parameter_length4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3,
    unsigned short parameter_type4, T4 t4, unsigned short parameter_length4,
    unsigned short parameter_type5, T5 t5, unsigned short parameter_length5
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
    package_message_parameter_template(output_buffer, parameter_type4, t4, parameter_length4);
    return package_message_parameter_template(output_buffer, parameter_type5, t5, parameter_length5);
}

template <typename T1, typename T2, typename T3, 
         typename T4, typename T5, typename T6>
int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3,
    unsigned short parameter_type4, T4 t4, unsigned short parameter_length4,
    unsigned short parameter_type5, T5 t5, unsigned short parameter_length5,
    unsigned short parameter_type6, T6 t6, unsigned short parameter_length6
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
    package_message_parameter_template(output_buffer, parameter_type4, t4, parameter_length4);
    package_message_parameter_template(output_buffer, parameter_type5, t5, parameter_length5);
    return package_message_parameter_template(output_buffer, parameter_type6, t6, parameter_length6);
}

template <typename T1, typename T2, typename T3, typename T4, 
    typename T5, typename T6, typename T7>
    int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3,
    unsigned short parameter_type4, T4 t4, unsigned short parameter_length4,
    unsigned short parameter_type5, T5 t5, unsigned short parameter_length5,
    unsigned short parameter_type6, T6 t6, unsigned short parameter_length6,
    unsigned short parameter_type7, T7 t7, unsigned short parameter_length7
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
    package_message_parameter_template(output_buffer, parameter_type4, t4, parameter_length4);
    package_message_parameter_template(output_buffer, parameter_type5, t5, parameter_length5);
    package_message_parameter_template(output_buffer, parameter_type6, t6, parameter_length6);
    return package_message_parameter_template(output_buffer, parameter_type7, t7, parameter_length7);
}

template <typename T1, typename T2, typename T3, typename T4, 
    typename T5, typename T6, typename T7, typename T8>
    int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3,
    unsigned short parameter_type4, T4 t4, unsigned short parameter_length4,
    unsigned short parameter_type5, T5 t5, unsigned short parameter_length5,
    unsigned short parameter_type6, T6 t6, unsigned short parameter_length6,
    unsigned short parameter_type7, T7 t7, unsigned short parameter_length7,
    unsigned short parameter_type8, T8 t8, unsigned short parameter_length8
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
    package_message_parameter_template(output_buffer, parameter_type4, t4, parameter_length4);
    package_message_parameter_template(output_buffer, parameter_type5, t5, parameter_length5);
    package_message_parameter_template(output_buffer, parameter_type6, t6, parameter_length6);
    package_message_parameter_template(output_buffer, parameter_type7, t7, parameter_length7);
    return package_message_parameter_template(output_buffer, parameter_type8, t8, parameter_length8);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,  
    typename T6, typename T7, typename T8, typename T9, typename T10>
    int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3,
    unsigned short parameter_type4, T4 t4, unsigned short parameter_length4,
    unsigned short parameter_type5, T5 t5, unsigned short parameter_length5,
    unsigned short parameter_type6, T6 t6, unsigned short parameter_length6,
    unsigned short parameter_type7, T7 t7, unsigned short parameter_length7,
    unsigned short parameter_type8, T8 t8, unsigned short parameter_length8,
    unsigned short parameter_type9, T9 t9, unsigned short parameter_length9,
    unsigned short parameter_type10, T10 t10, unsigned short parameter_length10
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
    package_message_parameter_template(output_buffer, parameter_type4, t4, parameter_length4);
    package_message_parameter_template(output_buffer, parameter_type5, t5, parameter_length5);
    package_message_parameter_template(output_buffer, parameter_type6, t6, parameter_length6);
    package_message_parameter_template(output_buffer, parameter_type7, t7, parameter_length7);
    package_message_parameter_template(output_buffer, parameter_type8, t8, parameter_length8);
    package_message_parameter_template(output_buffer, parameter_type9, t9, parameter_length9);
    return package_message_parameter_template(output_buffer, parameter_type10, t10, parameter_length10);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, 
    typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
    int package_message_common(
    unsigned char *output_buffer, 
    unsigned char protocol_version, unsigned short message_type,
    unsigned short parameter_type1, T1 t1, unsigned short parameter_length1,
    unsigned short parameter_type2, T2 t2, unsigned short parameter_length2,
    unsigned short parameter_type3, T3 t3, unsigned short parameter_length3,
    unsigned short parameter_type4, T4 t4, unsigned short parameter_length4,
    unsigned short parameter_type5, T5 t5, unsigned short parameter_length5,
    unsigned short parameter_type6, T6 t6, unsigned short parameter_length6,
    unsigned short parameter_type7, T7 t7, unsigned short parameter_length7,
    unsigned short parameter_type8, T8 t8, unsigned short parameter_length8,
    unsigned short parameter_type9, T9 t9, unsigned short parameter_length9,
    unsigned short parameter_type10, T10 t10, unsigned short parameter_length10,
    unsigned short parameter_type11, T11 t11, unsigned short parameter_length11,
    unsigned short parameter_type12, T12 t12, unsigned short parameter_length12,
    unsigned short parameter_type13, T13 t13, unsigned short parameter_length13,
    unsigned short parameter_type14, T14 t14, unsigned short parameter_length14
    )
{
    package_message_head(output_buffer, protocol_version, message_type);
    package_message_parameter_template(output_buffer, parameter_type1, t1, parameter_length1);
    package_message_parameter_template(output_buffer, parameter_type2, t2, parameter_length2);
    package_message_parameter_template(output_buffer, parameter_type3, t3, parameter_length3);
    package_message_parameter_template(output_buffer, parameter_type4, t4, parameter_length4);
    package_message_parameter_template(output_buffer, parameter_type5, t5, parameter_length5);
    package_message_parameter_template(output_buffer, parameter_type6, t6, parameter_length6);
    package_message_parameter_template(output_buffer, parameter_type7, t7, parameter_length7);
    package_message_parameter_template(output_buffer, parameter_type8, t8, parameter_length8);
    package_message_parameter_template(output_buffer, parameter_type9, t9, parameter_length9);
    package_message_parameter_template(output_buffer, parameter_type10, t10, parameter_length10);
    package_message_parameter_template(output_buffer, parameter_type11, t11, parameter_length11);
    package_message_parameter_template(output_buffer, parameter_type12, t12, parameter_length12);
    package_message_parameter_template(output_buffer, parameter_type13, t13, parameter_length13);
    return package_message_parameter_template(output_buffer, parameter_type14, t14, parameter_length14);
}

template<typename T>
bool unpack_parameterT(const unsigned char *input_buffer, unsigned short parameter_type, T &parameter_value);

class CommonMessage
{
public:
    CommonMessage();
    virtual ~CommonMessage();
    bool parse_message_head(const unsigned char *message_head);
    bool parse_message(const unsigned char *message);

    unsigned short get_message_body_length()const
    { 
        return message_body_length_;
    }

    unsigned short get_message_type()const
    {
        return message_type_;
    }

    unsigned char get_protocol_version()const
    {
        return protocol_version_;
    }
    template <typename T>
    bool get_parameter_value(unsigned short parameter_type, T& parameter_value)const
    {
        for (auto itr = parameter_positions_.begin(); itr != parameter_positions_.end(); ++itr)
        {
            if (unpack_parameterT(&message_data_[*itr], parameter_type, parameter_value))
            {
                return true;
            }
        }
        return false;
    }
    bool get_parameter_content(unsigned short parameter_type, unsigned short &content_size, unsigned char *content, unsigned int index = 0)const;
private:
    static const unsigned int MAX_MESSAGE_LENGTH = 16 * 1024;
	static const unsigned int MAX_PARAMETER_LENGTH = 8 * 1024;
    unsigned short message_body_length_;
    unsigned char message_data_[MAX_MESSAGE_LENGTH];
    unsigned short message_type_;
    unsigned char protocol_version_;
    std::vector<unsigned int> parameter_positions_;
};
#endif

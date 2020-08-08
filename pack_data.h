#ifndef _PACK_DATA_H_
#define _PACK_DATA_H_
#endif

template<typename T>
unsigned int pack_primitive(unsigned char *dst, T src)
{
	const auto size = sizeof(src);
	for (unsigned int i = 0; i < size; ++i)
	{
		dst[i] = static_cast<unsigned char>(src >> ((size - 1 - i) * 8));
	}
	return size;
}

template<typename T>
unsigned int parse_primitive(const unsigned char *src, T &dst)
{
	const auto size = sizeof(dst);
	dst = 0;
	for (unsigned int i = 0; i < size; ++i)
	{
		dst = (dst << 8) + src[i];
	}
	return size;
}

unsigned int move_array(unsigned char *dst, const unsigned char *src, unsigned int len);
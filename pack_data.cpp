#include "pack_data.h"
#include <memory.h>

unsigned int move_array(unsigned char *dst, const unsigned char *src, unsigned int len)
{
	if (len == 0)
	{
		return 0;
	}
	memcpy(dst, src, len);
	return len;
}

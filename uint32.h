/* reimplementation, GPL */
#ifndef UINT32_H
#define UINT32_H

#include "typesize.h"
typedef uo_uint32_t uint32;

void uint32_pack(char *,uint32);
void uint32_pack_big(char *,uint32);
void uint32_unpack(const char *,uint32 *);
void uint32_unpack_big(const char *,uint32 *);

#endif

/* reimplementation, GPL */
#ifndef UINT16_H
#define UINT16_H

#include "typesize.h"
typedef uo_uint16_t uint16;

void uint16_pack(char *,uint16);
void uint16_pack_big(char *,uint16);
void uint16_unpack(const char *,uint16 *);
void uint16_unpack_big(const char *,uint16 *);

#endif

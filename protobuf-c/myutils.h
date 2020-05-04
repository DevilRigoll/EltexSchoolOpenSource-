#ifndef _MY_UTILS_
#define _MY_UTILS_

#include "mytypes.h"
#include "msg.pb-c.h"

size_t packProtoBufMsg(struct Msg msg, uint8_t ** buf);

struct Msg unpackProtoBufMsg(uint8_t * buf);

struct Msg genMsg();

void dieWithError(char *errorMessage);

#endif

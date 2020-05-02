#ifndef _MY_QUEUE_
#define _MY_QUEUE_

#include "mytypes.h"

Queue initQ();

void qPushBack(Queue * q, Msg msg);

void qPopFront(Queue * q, Msg * msg);

void qClear(Queue * q);

#endif

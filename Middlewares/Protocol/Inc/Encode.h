#ifndef __ENCODE_H
#define __ENCODE_H

#include "stdint.h"
#include "Decode.h"


int Encode_Frame(Decode_Cmd_t *cmd, uint8_t *buf);

#endif
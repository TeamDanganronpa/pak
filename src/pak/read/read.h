#ifndef PAK_READ_READ_H
#define PAK_READ_READ_H

#include <stdint.h>

#include "pak/dll-api.h"

void* PAK_API OpenPAKRead(const char* FilePath);
void PAK_API DestroyPAKRead(void* Pak);

uint32_t PAK_API GetPAKOffsetsCount(void* Pak);
uint8_t* PAK_API GetPAKData(void* Pak, size_t Index, uint32_t* DataSizeBuffer);

#endif // PAK_READ_READ_H

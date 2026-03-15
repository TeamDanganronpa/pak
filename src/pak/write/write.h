#ifndef PAK_WRITE_WRITE_H
#define PAK_WRITE_WRITE_H

#include <stdint.h>

#include "pak/dll-api.h"

void* PAK_API OpenPAKWrite(const char* FilePath, uint32_t FilesCount);
void PAK_API DestroyPAKWrite(void* Pak);

void PAK_API PutPAKData(void* Pak, const void* Data, size_t DataSize);

#endif // PAK_WRITE_WRITE_H

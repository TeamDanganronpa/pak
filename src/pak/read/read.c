#include "read.h"

#include <stdlib.h>
#include <string.h>

#include <bytepicker/bytepicker.h>
#include <bytepicker/error.h>

typedef struct {
	void* PositionalFileReader;
	uint32_t OffsetsCount;
	uint32_t* Offsets;
} PAKHandle;

void* OpenPAKRead(const char* FilePath) {
	PAKHandle* PakHandle = malloc(sizeof(PAKHandle));
	if (!PakHandle) return NULL;

	PakHandle->PositionalFileReader = CreatePositionalFileReader(FilePath);
	if (!PakHandle->PositionalFileReader) {
		free(PakHandle);
		return NULL;
	}

	PositionalFileReader_Read(PakHandle->PositionalFileReader, &PakHandle->OffsetsCount, 4);
	PakHandle->Offsets = malloc(4 * PakHandle->OffsetsCount);
	for (uint32_t i = 0; i < PakHandle->OffsetsCount; i++)
		PositionalFileReader_Read(PakHandle->PositionalFileReader, &PakHandle->Offsets[i], 4);
	return PakHandle;
}

void DestroyPAKRead(void* Pak) {
	if (Pak) DestroyPositionalFileReader(Pak);
}

uint32_t GetPAKOffsetsCount(void* Pak) {
	if (!Pak) return 0;

	return ((PAKHandle*)Pak)->OffsetsCount;
}

uint8_t* GetPAKData(void* Pak, size_t Index, uint32_t* DataSizeBuffer) {
	if (!Pak) return NULL;

	PAKHandle* PakHandle = Pak;

	if (Index >= PakHandle->OffsetsCount) return NULL;

	if (Index + 1 >= PakHandle->OffsetsCount) /* Last offset */ {
		PositionalFileReader_SetPosition(PakHandle->PositionalFileReader, PakHandle->Offsets[Index]);
		uint8_t Buffer;
		while (1) {
			const char* Result = PositionalFileReader_Read(PakHandle->PositionalFileReader, &Buffer, 1);
			if (Result && strcmp(Result, BYTEPICKER_ERROR_EOF) == 0) break;
			++*DataSizeBuffer;
		}
	} else *DataSizeBuffer = PakHandle->Offsets[Index + 1] - PakHandle->Offsets[Index];
	uint8_t* PakData = malloc(*DataSizeBuffer);
	PositionalFileReader_SetPosition(PakHandle->PositionalFileReader, PakHandle->Offsets[Index]);
	PositionalFileReader_Read(PakHandle->PositionalFileReader, PakData, *DataSizeBuffer);
	return PakData;
}
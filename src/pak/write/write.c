#include "write.h"

#include <stdlib.h>
#include <string.h>

#include <bytepicker/bytepicker.h>

typedef struct {
	void* PositionalFileWriter;
	uint32_t OffsetsCount, CurrentOffsetIndex, CurrentRawDataOffset;
	uint32_t* Offsets;
} PAKWHandle;

void* OpenPAKWrite(const char* FilePath, uint32_t FilesCount) {
	PAKWHandle* PakWriteHandle = malloc(sizeof(PAKWHandle));
	if (!PakWriteHandle) return NULL;

	PakWriteHandle->PositionalFileWriter = CreatePositionalFileWriter(FilePath);
	if (!PakWriteHandle->PositionalFileWriter) return NULL;

	PakWriteHandle->CurrentOffsetIndex = 0;
	PakWriteHandle->OffsetsCount = FilesCount;
	PakWriteHandle->Offsets = malloc(4 * FilesCount);
	if (!PakWriteHandle->Offsets) return NULL;

	PositionalFileWriter_Write(PakWriteHandle->PositionalFileWriter, &PakWriteHandle->OffsetsCount, 4);
	uint32_t TmpValue = 0;
	for (uint32_t i = 0; i < PakWriteHandle->OffsetsCount; ++i)
		PositionalFileWriter_Write(PakWriteHandle->PositionalFileWriter, &TmpValue, 4);
	PositionalFileWriter_Write(PakWriteHandle->PositionalFileWriter, &TmpValue, 4);
	PakWriteHandle->CurrentRawDataOffset = PositionalFileWriter_GetPosition(PakWriteHandle->PositionalFileWriter);
	return PakWriteHandle;
}

void DestroyPAKWrite(void* Pak) {
	if (!Pak) return;

	PAKWHandle* PakWriteHandle = Pak;

	PositionalFileWriter_SetPosition(PakWriteHandle->PositionalFileWriter, 4);
	for (uint32_t i = 0; i < PakWriteHandle->OffsetsCount; ++i)
		PositionalFileWriter_Write(PakWriteHandle->PositionalFileWriter, &PakWriteHandle->Offsets[i], 4);
}

void PutPAKData(void* Pak, const void* Data, size_t DataSize) {
	if (!Pak) return;
	PAKWHandle* PakWriteHandle = Pak;

	uint32_t Offset = PakWriteHandle->CurrentRawDataOffset;
	PakWriteHandle->CurrentRawDataOffset = DataSize;
	for (uint32_t i = 0; i < PakWriteHandle->CurrentOffsetIndex; ++i) Offset += PakWriteHandle->Offsets[i];
	PakWriteHandle->Offsets[PakWriteHandle->CurrentOffsetIndex++] = Offset;
	PositionalFileWriter_Write(PakWriteHandle->PositionalFileWriter, Data, DataSize);
}
//
// Created by chris_000 on 10/31/2015.
//

#ifndef MGS_V_PATCH_READFILE_H
#define MGS_V_PATCH_READFILE_H

#include "common.h"

typedef struct{
    long binarySize;
    char* fileName;
    byte* bytes;
}BinaryFile;

BinaryFile*getBinaryFile(char *name);

unsigned long getFileSizeInBytes(FILE* file);

FILE* openBinaryFile(char* name, char* args);



byte* readAllBytes(FILE* file, unsigned long size, unsigned long* bytesRead);

void binaryFileInfo(BinaryFile* binaryFile, unsigned long bytesRead);



#endif //MGS_V_PATC_READFILE_H

//
// Created by chris_000 on 10/31/2015.
//

#ifndef MGS_V_PATCH_READFILE_H
#define MGS_V_PATCH_READFILE_H

#include "common.h"

typedef struct{
    int binarySize;
    char* fileName;
    byte* bytes;
}BinaryFile;

BinaryFile* readEntireFile(char* name);

long getFileSizeInBytes(FILE* file);

FILE* openBinaryFile(char* name, char* args);

int writeChanges(BinaryFile* binaryFile);

#endif //MGS_V_PATC_READFILE_H

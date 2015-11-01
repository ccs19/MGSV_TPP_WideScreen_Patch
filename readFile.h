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

long getFileSizeInBytes(FILE* file);

FILE* openBinaryFile(char* name, char* args);

int writeChanges(BinaryFile* binaryFile);

byte* readAllBytes(FILE* file, long size, long* bytesRead);

void binaryFileInfo(BinaryFile* binaryFile, long bytesRead);

long findHexLocation(BinaryFile* binaryFile, long lookFor);

void applyWidescreenPatch(BinaryFile* binaryFile, long changeTo);

#endif //MGS_V_PATC_READFILE_H

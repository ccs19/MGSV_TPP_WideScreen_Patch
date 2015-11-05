/**
Copyright 2015 Christopher Schneider

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

#ifndef MGS_V_PATCH_READFILE_H
#define MGS_V_PATCH_READFILE_H

#include "common.h"

#define CONFIG_FILE "MGSV_TPP_WideScreen_Patch.cfg"

typedef struct{
    float horizontalRes;
    float verticalRes;
    byte* lookFor;
    byte* lookForSquareScreenValue1;
    byte* lookForSquareScreenValue2;
    byte* replaceWith;
    int backupExecutable;
    char* backupExecutableName;
    unsigned long binarySize;
    char* fileName;
    byte* bytes;
}BinaryFile;

StrMap* readConfig();

void iter(const char *key, const char *value, const void *obj);

BinaryFile* getBinaryFile(char* name);

unsigned long getFileSizeInBytes(FILE* file);

FILE* openBinaryFile(char* name, char* args);

byte* readAllBytes(FILE* file, unsigned long size);

void binaryFileInfo(BinaryFile* binaryFile);

FILE* checkForExistingFile(char* fileName);

FILE* checkForExistingFileRecurse(char* fileName, int num);

char* parseFileName(char* fileName, char* fileNameBuffer, size_t bufSize, int fileNum);

Boolean backupFile(char* fileName, char* backupName);

Boolean createBackup(FILE* original, FILE* backup);

char* getFullyQualifiedPath(char* fileName);

#endif //MGS_V_PATC_READFILE_H

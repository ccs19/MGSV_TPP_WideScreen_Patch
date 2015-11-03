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

#ifndef MGS_V_PATCH_UTILITY_H
#define MGS_V_PATCH_UTILITY_H

#include "common.h"
#include "readFile.h"

typedef struct{
    unsigned char lookFor[4];
    unsigned char replaceWith[4];
}PatchValuesWideScreen;

unsigned int floatToHexPrint(float f);

void beginPatch();
void patchInitMessage(BinaryFile* binaryFile, unsigned char* lookFor, unsigned char* changeTo);
void fillMap();
void fillConfig(BinaryFile*, StrMap*);
void patch(BinaryFile* binaryFile, unsigned char* lookFor, unsigned char* changeTo);
int writeChanges(BinaryFile* binaryFile);
void applyBinaryPatch(BinaryFile *binaryFile, long foundIndex);
long findHexLocation(BinaryFile* binaryFile, unsigned char* lookFor);
char* getMapVal(StrMap* map, const char* key);
unsigned char* parseHexValues(char* hexVals);
unsigned char* calculateNewHex(float horizontal, float vertical);
int getNumHexEntries(char* hexVals);
unsigned char hexToUnsignedChar(char* hexVal);
unsigned char hctoi(const char h);
unsigned char* floatToHex(float val);
#endif //MGS_V_PATCH_UTILITY_H

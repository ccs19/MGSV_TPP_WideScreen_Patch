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
#include "fileOperations.h"


unsigned int floatToHexPrint(float f);
void beginPatch();
void patchInitMessage(BinaryFile* binaryFile);
void fillMap();
void fillConfig(BinaryFile*, StrMap*);
void patch(byte* byteArray, byte* lookFor, byte*replaceWith, unsigned long byteArrayLength);
size_t writeChanges(BinaryFile *binaryFile);
void applyBinaryPatch(byte* byteArray, byte* replaceWith, unsigned long replacementLength, long foundIndex);
unsigned long findHexLocation(byte* byteArray, byte* lookFor, unsigned long byteArrayLength, long replacementLength);
char* getMapVal(StrMap* map, const char* key);
byte* parseHexValues(char* hexVals);
byte* calculateNewHex(float horizontal, float vertical);
int getNumHexEntries(char* hexVals);
byte hexToUnsignedChar(char* hexVal);
byte hexCharToByte(const char h);
byte* floatToHex(float val);
Boolean byteToString(byte* bytes, char* buffer, size_t bufSize);
#endif //MGS_V_PATCH_UTILITY_H

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

unsigned int floatToHex(float f);

void beginPatch();
void patchInitMessage(unsigned int lookFor, unsigned int changeTo);
void fillMap();
void patch(BinaryFile* binaryFile, unsigned int lookFor, unsigned int changeTo);
int writeChanges(BinaryFile* binaryFile);
void applyBinaryPatch(BinaryFile *binaryFile, unsigned long changeTo, long foundIndex);
unsigned long findHexLocation(BinaryFile* binaryFile, unsigned long lookFor);

#endif //MGS_V_PATCH_UTILITY_H

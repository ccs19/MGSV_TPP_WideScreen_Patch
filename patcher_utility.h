//
// Created by chris_000 on 10/31/2015.
//

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

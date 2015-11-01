//
// Created by chris_000 on 10/31/2015.
//

#ifndef MGS_V_PATCH_UTILITY_H
#define MGS_V_PATCH_UTILITY_H

#include "common.h"

unsigned int floatToHex(float f);

void beginPatch();
void patchInitMessage(unsigned int lookFor, unsigned int changeTo);
void fillMap();

#endif //MGS_V_PATCH_UTILITY_H

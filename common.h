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

#ifndef MGS_V_PATCH_COMMON_H
#define MGS_V_PATCH_COMMON_H
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CCS_CLogger.h"

#define CONFIG_FILE "mgsv_patcher.cfg"

typedef unsigned char byte;

typedef enum{
    ZERO = 0,
    ONE = 1,TWO=2,THREE=3,FOUR=4,FIVE=5,SIX=6,SEVEN=7,EIGHT=8,NINE=9,A=10,B=11,C=12,D=13,E=14,F=15
}CCS_HEX;

typedef enum{ //from left to right
    FIRST
}CCS_HEX_MASK;

void printSummary();

void printError(int errorNum);


#endif //MGS_V_PATC_COMMON_H

//
// Created by chris_000 on 10/31/2015.
//

#ifndef MGS_V_PATCH_COMMON_H
#define MGS_V_PATCH_COMMON_H
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

//
// Created by chris_000 on 10/31/2015.
//

#include "patcher_utility.h"
#include "readFile.h"
#include "strmap.h"

StrMap* map;

static int hexNums[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
static char hexChars[] = {'0','1','2','3','4','5','6','7','8','9',
                          'a','b','c','d','e','f'};

unsigned int floatToHex(float f){
    return *(unsigned int*)&f;
}


//TODO remove
//hex to look for: 66 6f 6f 6c
//hex to replace: 64 75 64 65
void beginPatch(){
    map = sm_new(16);
    fillMap();
    BinaryFile* file = getBinaryFile("./simple.exe");
    patch(file, 0x666f6f6c, 0x64756465);
}

void patch(BinaryFile* binaryFile, unsigned int lookFor, unsigned int changeTo){
    patchInitMessage(lookFor, changeTo);
    long index = findHexLocation(binaryFile,lookFor);
    applyWidescreenPatch(binaryFile, changeTo);
}

long findHexLocation(BinaryFile* binaryFile, long lookFor){
    long i = 0;
    for(i; i<binaryFile->binarySize; i++){

    }
}

void applyWidescreenPatch(BinaryFile* binaryFile, long changeTo){

}

void fillMap(){

}

void patchInitMessage(unsigned int lookFor, unsigned int changeTo) {
    LogD("Beginning widescreen patch...");
    LogD("Looking for %x", lookFor);
    LogD("For %dx%d resolution, the replacement value is %x", 1920,1080,changeTo);
}
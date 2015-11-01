//
// Created by chris_000 on 10/31/2015.
//

#include "patcher_utility.h"
#include "strmap.h"
#include "readFile.h"

StrMap* map;

const int MAP_SIZE = 16;
static unsigned char hexNums[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
static unsigned char hexChars[] = {'0','1','2','3','4','5','6','7','8','9',
                          'a','b','c','d','e','f'};

unsigned int floatToHex(float f){
    return *(unsigned int*)&f;
}
unsigned char lookForVal[] = {0x39, 0x8E, 0xE3, 0x3F};
unsigned char replaceWithVal[] = {0xAB, 0xAA, 0xAA, 0x40};

//TODO remove
//hex to look for: 39 8e e3 ef
//hex to replace for 48:9 : AB AA AA 40
void beginPatch(){
    map = sm_new(MAP_SIZE);
    fillMap();
    BinaryFile* binaryFile = getBinaryFile("./mgsvtpp.exe");
    patch( binaryFile, 0x666f6f6c, 0x64756465);
    writeChanges(binaryFile);
}

void patch(BinaryFile* binaryFile, unsigned int lookFor, unsigned int changeTo){
    patchInitMessage(lookFor, changeTo);
    long index = findHexLocation(binaryFile,lookFor);
    applyBinaryPatch(binaryFile, changeTo, index);
}

unsigned long findHexLocation(BinaryFile* binaryFile, unsigned long lookFor){
    int lookForIndex = 0;
    int lookForSize = 4;
    long foundIndex = 0;
    int found = 0;
    for(long i= 0; i<binaryFile->binarySize; i++){
        LogV("%02x ", binaryFile->bytes[i]);
        if(i%15 == 0 && i != 0){
            LogV("\n");
        }
        fflush(stdout);

        if(binaryFile->bytes[i] == lookForVal[lookForIndex]){
            LogI("Found a match! foundIndex: %ld lookForIndex: %d", lookForIndex, foundIndex);
            if(lookForIndex++ == 0){
                foundIndex = i;
            }
            if(lookForIndex == lookForSize){
                LogI("Found result at index %ld", foundIndex);
                found = 1;
                break;
            }
            fflush(stdout);
        }
        else{
            lookForIndex = 0;
            foundIndex = 0;
        }
    }
    if(!found){
        LogE("Failed to find matching hex value, cannot patch");
        printSummary();
    }
    return foundIndex;
}

void applyBinaryPatch(BinaryFile *binaryFile, unsigned long changeTo, long foundIndex){
    long index = foundIndex;
    byte* bytes = binaryFile->bytes;
    for(int i = 0; i < 4; i++){
        bytes[index++] = replaceWithVal[i];
    }
}

void fillMap(){
    for(int i = 0; i < 1; i++) {
        if(!sm_put(map,&hexNums[i], &hexChars[i])){
            LogE("Failed to create hex lookup map");
        }
    }
}

void patchInitMessage(unsigned int lookFor, unsigned int changeTo) {
    LogD("Beginning widescreen patch...");
    LogD("Looking for %x", lookFor);
    LogD("For %dx%d resolution, the replacement value is %02x %02x %02x %02x"
                 , 1920,1080, replaceWithVal[0], replaceWithVal[1],replaceWithVal[2],replaceWithVal[3]);
}

int writeChanges(BinaryFile* binaryFile){
    FILE* file = openBinaryFile(binaryFile->fileName, "r+b");
    fseek(file, 0, SEEK_SET);
    size_t result = fwrite(binaryFile->bytes, sizeof(byte), binaryFile->binarySize, file);
    if(result != binaryFile->binarySize){
        LogE("Failed to write all data");
        LogE("Expected to write %d bytes", binaryFile->binarySize);
        LogE("Wrote %d bytes", result);
    }else{
        LogD("Patch appears to have been applied successfully! Yay!");
    }
    return result;
}
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

#include <ctype.h>
#include "patcher_utility.h"
#include "readFile.h"

const char* KEY_HORIZONTAL= "horizontalRes";
const char* KEY_VERTICAL = "verticalRes";
const char* KEY_EXEC = "executableName";
const char* KEY_BACKUP= "backupExecutable";
const char* KEY_BACKUP_NAME = "backupExecutableName";
const char* KEY_WIDESCREEN_HEX = "widescreenHexValues";


unsigned int floatToHex(float f){
    return *(unsigned int*)&f;
}
unsigned char lookForVal[] = {0x39, 0x8E, 0xE3, 0x3F};
unsigned char replaceWithVal[] = {0xAB, 0xAA, 0xAA, 0x40};

//TODO remove
//hex to look for: 39 8e e3 ef
//hex to replace for 48:9 : AB AA AA 40
void beginPatch(){
    StrMap* map = readConfig();

    int size = sm_get(map, KEY_EXEC, NULL, 0);
    char* fileName = malloc(size);
    sm_get(map, KEY_EXEC, fileName, size);

    BinaryFile* binaryFile = getBinaryFile(fileName);

    fillConfig(binaryFile, map);

    patch( binaryFile, 0x666f6f6c, 0x64756465);
    writeChanges(binaryFile);
    free(fileName);
    sm_delete(map);
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
        //LogV("%02x ", binaryFile->bytes[i]);
        if(i%15 == 0 && i != 0){
           // LogV("\n");
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

/**void fillMap(){
    for(int i = 0; i < 1; i++) {
        if(!sm_put(map,&hexNums[i], &hexChars[i])){
            LogE("Failed to create hex lookup map");
        }
    }
}**/



void patchInitMessage(unsigned int lookFor, unsigned int changeTo) {
    LogD("Beginning widescreen patch...");
    LogD("Looking for %x", lookFor);
    LogD("For %dx%d resolution, the replacement value is %02x %02x %02x %02x"
                 , 1920,1080, replaceWithVal[0], replaceWithVal[1],replaceWithVal[2],replaceWithVal[3]);
}



/**
 * Fill BinaryFile struct with remaining data
 */
void fillConfig(BinaryFile* binaryFile, StrMap* map){
    char *hRes,*vRes,*execName,*backup,*backupName,*hexVals;
    hRes = getMapVal(map, KEY_HORIZONTAL);
    vRes = getMapVal(map, KEY_VERTICAL);
    execName = getMapVal(map, KEY_EXEC);
    backup = getMapVal(map, KEY_BACKUP);
    backupName = getMapVal(map, KEY_BACKUP_NAME);
    hexVals = getMapVal(map, KEY_WIDESCREEN_HEX);
    binaryFile->horizontalRes = strtof(hRes, NULL);
    binaryFile->verticalRes = strtof(vRes, NULL);
    binaryFile->backupExecutable = strcmp(backup,"false");
    binaryFile->backupExecutableName = backupName;
    binaryFile->fileName = execName;
    binaryFile->lookFor = parseHexValues(hexVals);
    binaryFile->replaceWith = calculateNewHex(binaryFile->horizontalRes,
                                              binaryFile->verticalRes);
}

char* getMapVal(StrMap* map, const char* key){
    int reqSize = sm_get(map, key, NULL, 0);
    if(reqSize == 0){
        LogE("Failed to read property %s from config file", key);
        LogE("Cannot continue");
        printSummary();
    }
    char* val = malloc(reqSize);
    sm_get(map, key, val, reqSize);
    return val;
}


/**
 * write changes to file
 */
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

/**
 * Parses string containing hex values into unsigned char
 */
unsigned char* parseHexValues(char* hexVals){
    int count = 0;
    int numEntries = getNumHexEntries(hexVals);
    unsigned char* results = malloc(sizeof(char*)*numEntries);
    char* temp;
    while(1){
        temp = hexVals+((count)*3);
        results[count++] = hexToUnsignedChar(temp);
        if(count == numEntries){
            break;
        }
    }
    return results;
}

/**
 * Takes comma separated string and returns the number of entries
 */
int getNumHexEntries(char* hexVals){
    return (strlen(hexVals) / 3)+1;
}

unsigned char hexToUnsignedChar(char* hexVal){
    LogV("Attempting to convert %c%c", hexVal[0], hexVal[1]);
    unsigned char val1 = hctoi(hexVal[0])*0x10;
    unsigned char val2 = hctoi(hexVal[1]);
    LogV("Val1 = %d, Val2 = %d, Total = %d", val1, val2, val1+val2);
    return (val1+val2);
}

unsigned char hctoi(const char h){
    if(isdigit(h))
        return h - '0';
    else
        return toupper(h) - 'A' + 10;
}

unsigned char* calculateNewHex(float horizontal, float vertical){
    float result = horizontal/vertical;
}
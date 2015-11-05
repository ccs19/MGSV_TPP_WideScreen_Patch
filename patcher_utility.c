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
#include "fileOperations.h"

const char* KEY_HORIZONTAL= "horizontalRes";
const char* KEY_VERTICAL = "verticalRes";
const char* KEY_EXEC = "executableName";
const char* KEY_BACKUP= "backupExecutable";
const char* KEY_BACKUP_NAME = "backupExecutableName";
const char* KEY_WIDESCREEN_HEX = "widescreenHexValues";
const char* KEY_SQUARESCREENVAL_1 = "squareScreenValue1";
const char* KEY_SQUARESCREENVAL_2 = "squareScreenValue2";
Boolean oldAspectRatio = false;


unsigned int floatToHexPrint(float f){
    return *(unsigned int*)&f;
}

//TODO remove
//hex to look for: 39 8e e3 ef
//hex to replace for 48:9 : AB AA AA 40
void beginPatch(){
    StrMap* map = readConfig(); //Fill map with data from config file
    char backupBool[BUF_SIZE];
    char backupName[BUF_SIZE];
    memset(backupBool, 0, BUF_SIZE);
    memset(backupName,0,BUF_SIZE);
    sm_get(map, KEY_BACKUP, backupBool, BUF_SIZE);
    sm_get(map, KEY_BACKUP_NAME,backupName,BUF_SIZE);
    if(!strcmp(backupBool, "true")){
        LogI("You have selected to backup your exe. It will be backed up to %s", backupName);
    }else{
        LogW("You have executable backup disabled. Assuming you know what you're doing.");
    }
    LogI("If you're ready to proceed, press enter, otherwise close the window");
    getchar();
    int size = sm_get(map, KEY_EXEC, NULL, 0);
    char* fileName = malloc(size);
    sm_get(map, KEY_EXEC, fileName, size);

    BinaryFile* binaryFile = getBinaryFile(fileName);

    fillConfig(binaryFile, map);
    if(!strcmp(backupBool,"true")){
        backupFile(binaryFile->fileName, binaryFile->backupExecutableName);
    }
    patchInitMessage(binaryFile);
    patch( binaryFile->bytes, binaryFile->lookFor, binaryFile->replaceWith, binaryFile->binarySize);
    if(oldAspectRatio){
        LogI("Applying additional patch measures for aspect ratios less than 4:3");
        patch(binaryFile->bytes, binaryFile->lookForSquareScreenValue1, binaryFile->replaceWith, binaryFile->binarySize);
        patch(binaryFile->bytes, binaryFile->lookForSquareScreenValue2, binaryFile->replaceWith, binaryFile->binarySize);
        LogI("Finished additional patch");
    }
    writeChanges(binaryFile);
    free(fileName);
    sm_delete(map);
}

void patch(byte* byteArray, byte* lookFor, byte* replaceWith, unsigned long byteArraySize){
    long index = findHexLocation(byteArray, lookFor, byteArraySize, 4L);
    applyBinaryPatch(byteArray, replaceWith, 4L, index);
}

unsigned long findHexLocation(byte* byteArray, byte* lookFor, unsigned long byteArraySize, long replacementLength){
    LogI("Beginning lookup of hex values");
    char lookingForString[BUF_SIZE];
    byteToString(lookFor,lookingForString,BUF_SIZE);
    LogI("Looking for %s", lookingForString);
    int lookForIndex = 0;
    int lookForSize = replacementLength;
    unsigned long foundIndex = 0;
    int found = 0;
    for(unsigned long i= 0; i< byteArraySize; i++){
        if(byteArray[i] == lookFor[lookForIndex]){
            if(lookForIndex++ == 0){
                if(DEBUG_MODE)if(lookForIndex >= 2) {
                    LogMV("Found match at index %d", lookForIndex);
                    LogMV("Value: %x", byteArray[i]);
                }
                foundIndex = i;
            }
            if(lookForIndex == lookForSize){
                LogMV("!FOUND A MATCH! foundIndex: %ld lookForIndex: %d", lookForIndex, foundIndex);
                LogMV("Found result at index %ld", foundIndex);
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

void applyBinaryPatch(byte* byteArray, byte* replaceWith, unsigned long replacementLength, long foundIndex){
    long index = foundIndex;
    for(int i = 0; i < replacementLength; i++){
        byteArray[index++] = replaceWith[i];
    }
}

void patchInitMessage(BinaryFile* binaryFile) {
    float hRes = binaryFile->horizontalRes;
    float vRes = binaryFile->verticalRes;
    byte* lookFor = binaryFile->lookFor;
    byte* lookForSquare1 = binaryFile->lookForSquareScreenValue1;
    byte* lookForSquare2 = binaryFile->lookForSquareScreenValue2;
    byte* replaceWithVal = binaryFile->replaceWith;
    char buffer[BUF_SIZE];
    byteToString(lookFor,buffer,BUF_SIZE);
    LogI("Beginning widescreen patch...");
    LogI("Looking for %s", buffer);
    byteToString(replaceWithVal,buffer,BUF_SIZE);
    LogI("For %.fx%.f resolution, the replacement value is %s",hRes,vRes, buffer);
    LogMV("%f <= %f", (hRes/vRes), ((4.0f/3.0f)+0.000001f));
    if(hRes/vRes <= (4.0f/3.0f)+0.000001f){ //checking for older aspect ratios
        char bufferTwo[BUF_SIZE];
        char bufferThree[BUF_SIZE];
        byteToString(lookForSquare1,bufferTwo,BUF_SIZE);
        byteToString(lookForSquare2,bufferThree,BUF_SIZE);
        LogI("Detected an older aspect ratio, additional patching will occur");
        LogI("Also replacing %s and %s",bufferTwo, bufferThree);
        LogI("with %s",buffer);
        oldAspectRatio = true;
    }
}



/**
 * Fill BinaryFile struct with remaining data
 */
void fillConfig(BinaryFile* binaryFile, StrMap* map){
    char *hRes,*vRes,*execName,*backup,*backupName,*hexVals,*squareScreen1,*squareScreen2;
    hRes = getMapVal(map, KEY_HORIZONTAL);
    vRes = getMapVal(map, KEY_VERTICAL);
    execName = getMapVal(map, KEY_EXEC);
    backup = getMapVal(map, KEY_BACKUP);
    backupName = getMapVal(map, KEY_BACKUP_NAME);
    hexVals = getMapVal(map, KEY_WIDESCREEN_HEX);
    squareScreen1 = getMapVal(map, KEY_SQUARESCREENVAL_1);
    squareScreen2 = getMapVal(map, KEY_SQUARESCREENVAL_2);
    binaryFile->horizontalRes = strtof(hRes, NULL);
    binaryFile->verticalRes = strtof(vRes, NULL);
    binaryFile->backupExecutable = strcmp(backup,"false");
    binaryFile->backupExecutableName = backupName;
    binaryFile->fileName = execName;
    binaryFile->lookFor = parseHexValues(hexVals);
    binaryFile->lookForSquareScreenValue1 = parseHexValues(squareScreen1);
    binaryFile->lookForSquareScreenValue2 = parseHexValues(squareScreen2);
    binaryFile->replaceWith = calculateNewHex(binaryFile->horizontalRes,
                                              binaryFile->verticalRes);
}

char* getMapVal(StrMap* map, const char* key){
    int reqSize = sm_get(map, key, NULL, 0);
    if(reqSize == 0){
        LogE("Failed to read property %s from map", key);
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
size_t writeChanges(BinaryFile *binaryFile){
    FILE* file = openBinaryFile(binaryFile->fileName, "r+b");
    fseek(file, 0, SEEK_SET);
    size_t result = fwrite(binaryFile->bytes, sizeof(byte), binaryFile->binarySize, file);
    if(result != binaryFile->binarySize){
        LogE("Failed to write all data");
        LogE("Expected to write %d bytes", binaryFile->binarySize);
        LogE("Wrote %d bytes", result);
    }else{
        LogI("Patch appears to have been applied successfully! Yay!");
    }
    fclose(file);
    return result;
}

/**
 * Parses string containing hex values into byte
 */
byte* parseHexValues(char* hexVals){
    int count = 0;
    int numEntries = getNumHexEntries(hexVals);
    byte* results = malloc(sizeof(char*)*numEntries);
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
    return (int)((strlen(hexVals) / 3)+1);
}

byte hexToUnsignedChar(char* hexVal){
    LogMV("Attempting to convert %c%c", hexVal[0], hexVal[1]);
    byte val1 = hexCharToByte(hexVal[0]) * 0x10;
    byte val2 = hexCharToByte(hexVal[1]);
    LogMV("Val1 = %d, Val2 = %d, Total = %d", val1, val2, val1+val2);
    return (val1+val2);
}

//TODO improve this accept a char* and return a byte array
byte hexCharToByte(const char h){
    if(isdigit(h))
        return h - '0';
    else
        return toupper(h) - 'A' + 10;
}


byte* calculateNewHex(float horizontal, float vertical){
    return floatToHex(horizontal/vertical);
}

/**
 * assumes 32 bit float
 */
byte* floatToHex(float val){
    LogMV("Float val %f", val);
    byte* hexVals = malloc(sizeof(float));
    hexVals[0] = ((byte*)&val)[0];
    hexVals[1] = ((byte*)&val)[1];
    hexVals[2] = ((byte*)&val)[2];
    hexVals[3] = ((byte*)&val)[3];
    char buffer[BUF_SIZE];
    byteToString(hexVals,buffer,BUF_SIZE);
    LogMV("Hex values to replace %s", buffer);
    return hexVals;
}

/*
 * Converts 32 bit byte array to printable string
 * return false if either buffer is too small or null
 */
Boolean byteToString(byte* bytes, char* buffer, size_t bufSize){
    static const char* format = "%02x %02x %02x %02x";
    static size_t length = 0;
    if(!length){
        length = strlen(format);
    }
    if(buffer != NULL) {
        if(bufSize >= length) {
            memset(buffer, 0, bufSize);
            sprintf(buffer, format, bytes[0], bytes[1], bytes[2], bytes[3]);
            return true;
        }
    }
    return false;
}

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

#include "readFile.h"


void iter(const char *key, const char *value, const void *obj){
    LogV("key: %s val: %s",key, value);
}

StrMap* readConfig() {
    StrMap* map = sm_new(16);
    FILE* fp = fopen(CONFIG_FILE, "r");
    char* key;
    char* val;
    char buffer[256];
    if(fp != NULL){
        while(fgets(buffer, sizeof(buffer), fp) != NULL){
            key = strtok(buffer, "=");
            val = strtok(NULL, "=");
            if((key && val) && !(strcmp(key, "\r\n"))) {
                    val[strlen(val) - 1] = '\0';
                    sm_put(map, key, val);

            }
        }
    }else{
        LogE("Failed to open config file %s", CONFIG_FILE);
    }
    sm_enum(map,iter,NULL);
    return map;
}


BinaryFile* getBinaryFile(char* name){
    LogD("Attempting to access file %s-", name);
    FILE* file = openBinaryFile(name, "r+b");
    unsigned long size = getFileSizeInBytes(file);
    unsigned long bytesRead = 0;

    BinaryFile* binaryFile = malloc(sizeof(BinaryFile));
    binaryFile->fileName = malloc((strlen(name)+1));
    memcpy(binaryFile->fileName, name, (strlen(name)+1));
    binaryFile->binarySize = size;
    binaryFile->bytes = readAllBytes(file, size, &bytesRead);
    binaryFileInfo(binaryFile,bytesRead);

    fclose(file); //TODO add a check for successful close.
    return binaryFile;
}

void binaryFileInfo(BinaryFile* binaryFile, unsigned long bytesRead){
    LogD("Completed read of binary file. Stats:");
    LogD("Name: %s",binaryFile->fileName);
    LogD("Expected bytes read: %ld", binaryFile->binarySize +1);
    LogD("Actual bytes read: %ld", bytesRead);
    /**if(bytesRead == binaryFile->binarySize+1){
        LogD("Successfully loaded file :-)");
    }else{
        LogE("Failed to read the entire file, cannot continue");
        printSummary();
    }**/

}

unsigned long getFileSizeInBytes(FILE* file){
    fseek(file, 0L, SEEK_END);
    unsigned long size = ftell(file);
    rewind(file);
    return size;
}

FILE* openBinaryFile(char* name, char* args){
    FILE* file = fopen(name, args);
    if(!file){
        int errorNum = errno;
        LogE("Error opening file %s. Cannot proceed.", name);
        printError(errorNum);
        if(DEBUG_MODE)getchar();
        exit(0);
    }
}
/**
int writeChanges(BinaryFile* binaryFile){
    LogD("Attempting to write changes to %s", binaryFile->fileName);
    FILE* file = openBinaryFile(binaryFile->fileName, "r+b");
    if(!file){
        int errorNum = errno;
        LogE("Failed to write to file");
        printError(errorNum);
    }
    LogD("Successfully wrote to %s.");
    LogD("==========================");
    LogD("If the patch was successful, the file sizes should be identical");
    LogD("Original File Size: %ld", binaryFile->binarySize);
    LogD("Patched  File Size: %ld", getFileSizeInBytes(file));
    return 0;
}**/

byte* readAllBytes(FILE* file,unsigned long size, unsigned long* bytesRead){
    byte* buffer = malloc(size+1);
    fseek(file, 0, SEEK_SET);
    while(feof(file) == 0) {
        fread(buffer, 1, size, file);
    }
    return buffer;
}










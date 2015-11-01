//
// Created by chris_000 on 10/31/2015.
//

#include <stdlib.h>
#include "readFile.h"


BinaryFile* readEntireFile(char* name){
    LogD("Attempting to access file %s", name);
    BinaryFile* binaryFile = NULL;
    FILE* file = openBinaryFile(name, "rb");
    long size = getFileSizeInBytes(file);
    LogD("Successfully opened file with size of %ld bytes", size);
    return binaryFile;
}


long getFileSizeInBytes(FILE* file){
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return size;
}

FILE* openBinaryFile(char* name, char* args){
    FILE* file = fopen(name, args);
    if(!file){
        int errorNum = errno;
        LogE("Error opening file %s. Cannot proceed.", name);
        printError(errorNum);
        getchar();
        exit(0);
    }
}

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
}










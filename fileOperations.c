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

#include "fileOperations.h"
#include <unistd.h>

void iter(const char *key, const char *value, const void *obj){
    LogMV("key: %s val: %s",key, value);
}

StrMap* readConfig() {
    LogI("============================");
    LogI("Loading config file %s", CONFIG_FILE);
    StrMap* map = sm_new(16);
    FILE* fp = fopen(CONFIG_FILE, "r");
    char* key;
    char* val;
    char buffer[256];
    if(fp != NULL){
        while(fgets(buffer, sizeof(buffer), fp) != NULL){
            key = strtok(buffer, "=");
            val = strtok(NULL, "=");
            if(key != NULL && val != NULL && (strcmp(key, "\n") != 0)) {
                LogMV("Key: %s, Val: %s", key, val);
                if(val[strlen(val)-1] == '\n') {
                    LogMV("Key formatting: replacing %c with %c", val[strlen(val)-1], '\0');
                    val[strlen(val) - 1] = '\0';
                }
                sm_put(map, key, val);
            }
        }
    }else{
        LogE("Failed to open config file %s [%x]", CONFIG_FILE, fp);
        LogE("Cannot continue");
        printSummary();
    }
    sm_enum(map,iter,NULL);
    LogMV("Exiting readConfig()");
    LogI("Loaded config successfully");
    LogI("============================\n\n");
    return map;
}


BinaryFile* getBinaryFile(char* name){
    LogMV("Entering getBinaryFile");
    FILE* file = openBinaryFile(name, "r+b"); //TODO Check?
    unsigned long size = getFileSizeInBytes(file);

    BinaryFile* binaryFile = malloc(sizeof(BinaryFile));
    binaryFile->fileName = malloc((strlen(name)+1));
    memcpy(binaryFile->fileName, name, (strlen(name)+1));
    binaryFile->binarySize = size;
    binaryFile->bytes = readAllBytes(file, size);
    binaryFileInfo(binaryFile);
    fclose(file); //TODO add a check for successful close.
    LogMV("Exiting getBinaryFile");
    return binaryFile;
}

void binaryFileInfo(BinaryFile* binaryFile){
    LogI("Completed read of binary file. Stats:");
    LogI("Name: %s",binaryFile->fileName);
    LogI("Number of bytes read: %ld", binaryFile->binarySize +1);
}

unsigned long getFileSizeInBytes(FILE* file){
    fseek(file, 0L, SEEK_END);
    unsigned long size = ftell(file);
    rewind(file);
    return size;
}

FILE* openBinaryFile(char* name, char* args){
    LogMV("Entering openBinaryFile(%s, %s)", name, args);
    FILE* file = fopen(name, args);
    if(!file){
        int errorNum = errno;
        LogE("Error opening file %s. Cannot proceed.", name);
        printError(errorNum);
        if(DEBUG_MODE)getchar();
        exit(0);
    }else{
        LogMV("Successfully opened file %s in mode %s at %p", name, args, file);
    }
    LogMV("Exiting openBinaryFile(%s, %s)", name, args);
    return file;
}


byte* readAllBytes(FILE* file,unsigned long size) {
    LogI("Reading all bytes from file");
    if (!file) {
        LogE("Can't read file %p", file);
        printSummary();
    } else {
        byte *buffer = malloc(size + 1);
        fseek(file, 0, SEEK_SET);
        while (feof(file) == 0) {
            fread(buffer, 1, size, file);
        }
        LogI("Read entire file successfully");
        return buffer;
    }
}

char* getFullyQualifiedPath(char* fileName){
    char* pwd = getcwd(NULL, 0);
    LogI("Trying to open %s\\%s", pwd,fileName);
    char* fullyQualifiedPath = malloc(strlen(pwd) + strlen(fileName)+5);
    memset(fullyQualifiedPath,0,strlen(pwd) + strlen(fileName)+5);
    strcat(fullyQualifiedPath,"\"");
    strcat(fullyQualifiedPath,pwd);
    strcat(fullyQualifiedPath,"\\");
    strcat(fullyQualifiedPath,fileName);
    strcat(fullyQualifiedPath,"\"");
    free(pwd);
    LogV("Build fully qualified path:\n%s", fullyQualifiedPath);
    return (fullyQualifiedPath);
}

Boolean backupFile(char* fileName, char* backupName){
    LogMV("Starting backup. Backuping up %s to %s", fileName, backupName);
    FILE* original = fopen(fileName,"r+b");
    FILE* backup;
    if(original == NULL){
        LogW("Failed to open %s for backup. File will not be backed up.", fileName);
        LogW("To continue anyway, press enter. Otherwise, close the windows and manually backup your file.");
        getchar();
    }else{
        backup = checkForExistingFile(backupName);
    }
    if(!createBackup(original, backup)){
        LogW("The backup failed for some reason.");
        LogW("This could be due to incorrect file names or folder write permissions");
        LogW("To continue anyway, press enter. Otherwise, close the windows and manually backup your file.");
        getchar();
    };
}

/**
 * If a backup exists already, it's not overwritten and a new file is created.
 * e.g. if backup.exe already exists, backup(1).exe is created.
 * If backup(1).exe exists, backup(2).exe is created, etc.
 */
FILE* checkForExistingFile(char* fileName){
    return checkForExistingFileRecurse(fileName, 0);
}

FILE* checkForExistingFileRecurse(char* fileName, int num){
    if( access( fileName, F_OK ) != -1 ) {
        //File exists
        return checkForExistingFileRecurse(fileName, ++num);
    } else {
        //File doesn't exist
        char fileNameBuffer[BUF_SIZE];
        parseFileName(fileName, fileNameBuffer, BUF_SIZE, num);
        return fopen(fileName, "w+b");
    }
}

Boolean createBackup(FILE* original, FILE* backup){
    unsigned long originalSize = getFileSizeInBytes(original);
    byte* bytes = readAllBytes(original,originalSize);
    fwrite(bytes, originalSize,1,backup);
    fclose(original);
    fclose(backup);
    free(bytes);
}

char* parseFileName(char* fileName, char* fileNameBuffer, size_t bufSize, int num){
    char* temp = fileName;
    char* last = NULL;
    //strtok(temp,".");
    //TODO implement this.
    return fileName;
}




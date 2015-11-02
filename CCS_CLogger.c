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



#include <string.h>
#include "CCS_CLogger.h"

long long CCS_CLogger_programStartTime;

long long getTimeMilliseconds() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000LL;
    return milliseconds;
}


void initLogger(){
    CCS_CLogger_programStartTime = getTimeMilliseconds();
//    CCS_LOG_LEVEL = CCS_LOGS_LEVEL_DEFAULT;
}

void makeMessage(FILE *output, const char *tag, const char *func, const int line, const char *format, ...) {
    char arr[CCS_CLogger_BUFFER_SIZE];
    char functionAndLine[strlen(func)+11]; //10 max length of an int, +1;
    memset(functionAndLine, 0,sizeof(functionAndLine));
    sprintf(functionAndLine, "%s:%d",func, line);
    va_list args;
    va_start(args, format);
    long long now =  getTimeMilliseconds() - CCS_CLogger_programStartTime;
    snprintf(arr, CCS_CLogger_BUFFER_SIZE, CCS_CLogger_LOGGER_FORMAT, now, functionAndLine, tag, format);
    vfprintf(output, arr, args);
    va_end(args);
    fflush(output);
}

void setLogLevel(int logLevel){
 //   CCS_LOG_LEVEL = logLevel;
}
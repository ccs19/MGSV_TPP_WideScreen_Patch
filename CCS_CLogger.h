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

/**
 * Author's note: Suitable for small projects that don't want the huge overhead of installing
 * third party logging libraries (e.g. Apache Log4cpp)
 *
 * Capabilities are limited intentionally.
 *
 * Call just like printf
 * e.g. LogE("Oh noes, call tech support! Their number is %s", "unknown");
 *
 * LogLevel | Definition | Description
 *    4         LogI         Info
 *    3         LogD         Debug
 *    2         LogW         Warn
 *    1         LogE         Error
 *
 * Log level 4 enables all logs, log level 1 only enables error logs
 * log level 0 disables all logs.
 *
 */
#ifndef CCS_CLOGGER_H
#define CCS_CLOGGER_H

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include <sys/time.h>


/**Settings**/
#define CCS_LOGS_LEVEL 3


//Max log entry size. If you exceed this, you either need to shorten your logs
//or you should be using a more robust logging system :-)
#define CCS_CLogger_BUFFER_SIZE 1024
#define CCS_CLogger_LOGGER_FORMAT "[%lld] %20s [%s] %s\n"

extern long long getTimeMilliseconds();
extern void initLogger();
extern void makeMessage(FILE *output, const char *tag, const char *func, const int line, const char *format, ...);

/**empty defines if logging disabled**/
#define LogE(fmt, ...)
#define LogW(fmt, ...)
#define LogD(fmt, ...)
#define LogI(fmt, ...)


#if(CCS_LOGS_LEVEL >= 1)
#undef LogE
#define LogE(fmt, ...)  makeMessage(stderr, "ERROR", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if(CCS_LOGS_LEVEL >= 2)
#undef LogW
#define LogW(fmt, ...)  makeMessage(stderr, "WARN", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if(CCS_LOGS_LEVEL >= 3)
#undef LogD
#define LogD(fmt, ...) makeMessage(stdout, "DEBUG", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#if(CCS_LOGS_LEVEL >= 4)
#undef LogI
#define LogI(fmt, ...)  makeMessage(stdout, "INFO", __func__, __LINE__, fmt, ##__VA_ARGS__)
#endif


#endif //CCS_CLOGGER_H



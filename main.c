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

#include <unistd.h>
#include <windows.h>
#include "common.h"
#include "patcher_utility.h"






//Test value: @@@@
//horizontal is 30
//vertical is 9.98694536865365
//hex is 40 40 40 40
//hex to replace: 64 75 64 65
int main() {
    printf("\n\n");
    initLogger();
    beginPatch();
    printSummary();


    /**If I want to add console colors...
    HANDLE  hConsole;
    int k;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // you can loop k higher to see more color choices
    for(k = 0; k < 512; k++)
    {
        SetConsoleTextAttribute(hConsole, k);
        printf("%3d  %s\n", k, "I want to be nice today!");
    }

    SetConsoleTextAttribute(hConsole,7);**/

    return 0;
}


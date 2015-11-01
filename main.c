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

#include "common.h"
#include "patcher_utility.h"




//hex to look for: 66 6f 6f 6c
//hex to replace: 64 75 64 65
int main() {
    //TODO add warning to backup exe
    initLogger();
    beginPatch();
    printSummary();
    return 0;
}


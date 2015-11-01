//
// Created by chris_000 on 11/1/2015.
//

#include "common.h"

void printSummary(){
    printf("\n\n");
    printf("======================================================================================\n");
    printf("||                    MGS V: The Phantom Pain Widescreen Patcher                    ||\n");
    printf("======================================================================================\n");
    printf("|| Site: http://github.com/ccs19/MGSV_TPP_WideScreen_Patch                          ||\n");
    printf("|| Issue Tracker: https://github.com/ccs19/MGSV_TPP_WideScreen_Patch/issues         ||\n");
    printf("======================================================================================\n");
    printf("||                                                                                  ||\n");
    printf("|| If the patch failed for any reason, please create an issue in the issue tracker  ||\n");
    printf("|| Include all the logs printed out and any other info you may think is relevant    ||\n");
    printf("||                                                                                  ||\n");
    printf("======================================================================================\n");
    printf("\n");
    printf("\n\"METAL GEAR SOLID\" is a registered trademark of Konami Digital Entertainment Co., Ltd.\n\n");
    printf("press enter to exit...");
    //getchar();TODO uncomment
    exit(0);
}

void printError(int errorNum){
    char* err = strerror(errorNum);
    LogE("Reason: %s", err);
    LogE("Cannot continue. Press any key to continue...");
    getchar();
    printSummary();
}
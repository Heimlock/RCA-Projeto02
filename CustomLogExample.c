
#include "./commonLibs/CustomStreams.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    printf("AAA\n");
    Log.debug(getpid(), "Debug %s\n", "Debug");
    Log.fine(getpid(), "Fine %s\n", "Fine");
    Log.info(getpid(), "Info %s\n", "Info");
    Log.plain("Plain %s\n", "Plain");
    Log.error(getpid(), "Error %s\n", "Error");
    char* userId = (char*) malloc((9 + 1) * sizeof(char));
    Input.getStr("UserId: ", &userId, (9 + 1) * sizeof(char));
    Log.info(getpid(), "userId %s\n", userId);
    return 0;
}
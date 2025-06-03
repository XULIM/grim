#define NOB_IMPLEMENTATION
#include "src/nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"


int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER))
        return 1;

    Nob_Cmd cmd = {0};

    // Debug build
    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", BUILD_FOLDER"main",
            SRC_FOLDER"main.c", SRC_FOLDER"arena.c", SRC_FOLDER"ppmproc.c",
            "-msse4.2", "-g");

    if (!nob_cmd_run_sync(cmd))
        return 1;
    
    cmd.count = 0;

    return 0;
}

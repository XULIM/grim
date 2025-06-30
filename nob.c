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
    /*
    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", BUILD_FOLDER"main",
            SRC_FOLDER"main.c", SRC_FOLDER"arena.c", SRC_FOLDER"ppmproc.c",
            "-msse4.2", "-g");
    */


    // main
    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", BUILD_FOLDER"main.o",
            "-c", SRC_FOLDER"main.c", "-g", "-lm");
    if (!nob_cmd_run_sync_and_reset(&cmd))
    {
        nob_log(NOB_ERROR, "failed to compile main.c");
        return 1;
    }

    // arena
    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", BUILD_FOLDER"arena.o",
            "-c", SRC_FOLDER"arena.c", "-g");
    if (!nob_cmd_run_sync_and_reset(&cmd))
    {
        nob_log(NOB_ERROR, "failed to compile arena.c");
        return 1;
    }

    // ppmproc
    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o",
            BUILD_FOLDER"ppmproc.o", "-c", SRC_FOLDER"ppmproc.c", "-g",
            "-msse4.2");
    if (!nob_cmd_run_sync_and_reset(&cmd))
    {
        nob_log(NOB_ERROR, "failed to compile ppmproc.c");
        return 1;
    }

    nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o",
            BUILD_FOLDER"pixel.o", "-c", SRC_FOLDER"pixel.c", "-g");
    if (!nob_cmd_run_sync_and_reset(&cmd))
    {
        nob_log(NOB_ERROR, "failed to compile pixel.c");
        return 1;
    }

    // link
    nob_cmd_append(&cmd, "cc", "-o", BUILD_FOLDER"main",
            BUILD_FOLDER"main.o", BUILD_FOLDER"arena.o",
            BUILD_FOLDER"ppmproc.o", BUILD_FOLDER"pixel.o",
            "-lm");
    if (!nob_cmd_run_sync_and_reset(&cmd))
    {
        nob_log(NOB_ERROR, "failed to compile link files");
        return 1;
    }

    cmd.count = 0;

    return 0;
}

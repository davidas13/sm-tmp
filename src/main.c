#include <mpv/client.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool is_url(const char *url)
{
    regex_t regex;

    // Credit: https://stackoverflow.com/a/38608262/8403177
    const char *regex_pattern =
        "^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w "
        "\\.-]*)*\\/?";

    int regex_compile =
        regcomp(&regex, regex_pattern, REG_EXTENDED | REG_NOSUB);

    if (regex_compile != 0)
    {
        fprintf(stderr, "Failed to compile regex pattern.\n");

        return false;
    }

    int regex_execute = regexec(&regex, url, 0, NULL, 0);

    return regex_execute == 0;
}

bool is_filepath(const char *filepath)
{
    struct stat PathInfo;

    if (stat(filepath, &PathInfo) != 0)
    {
        fprintf(stderr, "Failed to stat file.\n");

        return false;
    }

    if (PathInfo.st_mode & S_IFDIR)
    {
        fprintf(stderr, "Filepath is a directory.\n");

        return false;
    }

    return true;
}

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "You must pass a url argument.\n");

        return EXIT_FAILURE;
    }

    const char *filepath_or_yt_url = argv[1];

    if (!is_url(filepath_or_yt_url) && !is_filepath(filepath_or_yt_url))
    {
        fprintf(stderr, "Invalid filepath or url.\n");

        return EXIT_FAILURE;
    }

    mpv_handle *mpv = mpv_create();

    if (!mpv)
    {
        fprintf(stderr, "Failed to initialize mpv instance\n");

        return EXIT_FAILURE;
    }

    if (mpv_initialize(mpv) < 0)
    {
        fprintf(stderr, "Failed to intialize mpv instance.\n");

        mpv_terminate_destroy(mpv);

        return EXIT_FAILURE;
    }

    const char *command[] = {"loadfile", filepath_or_yt_url, NULL};

    if (mpv_command(mpv, command) < 0)
    {
        fprintf(stderr, "Failed to run loadfile command.\n");

        mpv_terminate_destroy(mpv);

        return EXIT_FAILURE;
    }

    printf("Video playing...\n");

    while (true)
    {
        mpv_event *mpv_event = mpv_wait_event(mpv, -1);

        if (mpv_event->event_id == MPV_EVENT_SHUTDOWN)
        {
            break;
        }
    }

    mpv_terminate_destroy(mpv);

    printf("Player close.\n");

    return EXIT_SUCCESS;
}

#include <mpv/client.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool is_url(const char* sz_url)
{
    regex_t regex;

    // Credit: https://stackoverflow.com/a/38608262/8403177
    const char* p_regex_pattern =
        "^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w "
        "\\.-]*)*\\/?";

    int regex_compile =
        regcomp(&regex, p_regex_pattern, REG_EXTENDED | REG_NOSUB);

    if (regex_compile != 0)
    {
        fprintf(stderr, "Failed to compile regex pattern.\n");

        return false;
    }

    int regex_execute = regexec(&regex, sz_url, 0, NULL, 0);

    return regex_execute == 0;
}

bool is_filepath(const char* sz_filepath)
{
    struct stat PathInfo;

    if (stat(sz_filepath, &PathInfo) != 0)
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

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "You must pass a url argument.\n");

        return EXIT_FAILURE;
    }

    const char* sz_filepath_or_yt_url = argv[1];

    if (!is_url(sz_filepath_or_yt_url) && !is_filepath(sz_filepath_or_yt_url))
    {
        fprintf(stderr, "Invalid filepath or url.\n");

        return EXIT_FAILURE;
    }

    mpv_handle* p_mpv = mpv_create();

    if (!p_mpv)
    {
        fprintf(stderr, "Failed to initialize mpv instance\n");

        return EXIT_FAILURE;
    }

    if (mpv_initialize(p_mpv) < 0)
    {
        fprintf(stderr, "Failed to intialize mpv instance.\n");

        mpv_terminate_destroy(p_mpv);

        return EXIT_FAILURE;
    }

    const char* a_command[] = {"loadfile", sz_filepath_or_yt_url, NULL};

    if (mpv_command(p_mpv, a_command) < 0)
    {
        fprintf(stderr, "Failed to run loadfile command.\n");

        mpv_terminate_destroy(p_mpv);

        return EXIT_FAILURE;
    }

    printf("Video playing...\n");

    while (true)
    {
        mpv_event* p_mpv_event = mpv_wait_event(p_mpv, -1);

        if (p_mpv_event->event_id == MPV_EVENT_SHUTDOWN ||
            p_mpv_event->event_id == MPV_EVENT_END_FILE)
        {
            break;
        }
    }

    mpv_terminate_destroy(p_mpv);

    printf("Player close.\n");

    return EXIT_SUCCESS;
}

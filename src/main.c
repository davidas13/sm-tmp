#include "utils.h"
#include <mpv/client.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "You must pass a url argument.\n");

        return EXIT_FAILURE;
    }

    const char* p_sz_filepath_or_url = argv[1];

    if (!is_url(p_sz_filepath_or_url) && !is_filepath(p_sz_filepath_or_url))
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

    mpv_set_option_string(p_mpv, "terminal", "yes");
    mpv_set_option_string(p_mpv, "input-terminal", "yes");
    mpv_set_option_string(p_mpv, "input-default-bindings", "no");
    mpv_set_option_string(p_mpv, "msg-level",
                          "cplayer=no,display-tags=no,input=no");
    mpv_set_option_string(p_mpv, "term-osd-bar", "yes");
    mpv_set_option_string(p_mpv, "term-status-msg", "${filename}");
    mpv_set_option_string(p_mpv, "term-osd-bar-chars", "[━●━]");

    if (mpv_initialize(p_mpv) < 0)
    {
        fprintf(stderr, "Failed to intialize mpv instance.\n");

        mpv_terminate_destroy(p_mpv);

        return EXIT_FAILURE;
    }

    const char* p_a_command[] = {"loadfile", p_sz_filepath_or_url, NULL};

    if (mpv_command(p_mpv, p_a_command) < 0)
    {
        fprintf(stderr, "Failed to run loadfile command.\n");

        mpv_terminate_destroy(p_mpv);

        return EXIT_FAILURE;
    }

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

    return EXIT_SUCCESS;
}

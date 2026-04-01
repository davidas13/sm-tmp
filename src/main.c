#include <mpv/client.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "You must pass a url argument.\n");

        return EXIT_FAILURE;
    }

    const char *yt_url = argv[1];

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

    const char *command[] = {"loadfile", yt_url, NULL};

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

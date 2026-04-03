#include <mpv/client.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool is_url(const char* p_sz_url)
{
    regex_t regex;

    // Credit: https://stackoverflow.com/a/38608262/8403177
    const char* p_sz_regex_pattern =
        "^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w "
        "\\.-]*)*\\/?";

    int regex_compile =
        regcomp(&regex, p_sz_regex_pattern, REG_EXTENDED | REG_NOSUB);

    if (regex_compile != 0)
    {
        fprintf(stderr, "Failed to compile regex pattern.\n");

        return false;
    }

    int regex_execute = regexec(&regex, p_sz_url, 0, NULL, 0);

    return regex_execute == 0;
}

bool is_filepath(const char* p_sz_filepath)
{
    struct stat path_info;

    if (stat(p_sz_filepath, &path_info) != 0)
    {
        fprintf(stderr, "Failed to stat file.\n");

        return false;
    }

    if (path_info.st_mode & S_IFDIR)
    {
        fprintf(stderr, "Filepath is a directory.\n");

        return false;
    }

    return true;
}

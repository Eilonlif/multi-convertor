#include "config.h"
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 256

void remove_spaces(char *s) {
    char *d = s;
    do {
        while (*d == ' ' || *d == '\t' || *d == '\n') {
            ++d;
        }
    } while ((*s++ = *d++));
}

int read_config(config_entry **config_entry_list, int size) {
    char *configuration_file_name = "/Users/eilonlif/CLionProjects/multiConvertor/configurations.txt";
    // reading line by line, max 256 bytes
    char tmp_line[MAX_LENGTH];

    FILE *fp = fopen(configuration_file_name, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s", configuration_file_name);
        return 1;
    }
    const char separator = '=';
    while (fgets(tmp_line, MAX_LENGTH, fp)) {
        remove_spaces(tmp_line);
        if (strlen(tmp_line) > 0) {
            char *const sep_at = strchr(tmp_line, separator);
            if (sep_at != NULL) {
                *sep_at = '\0'; /* overwrite first separator, creating two strings. */
                for (int i = 0; i < size; i++) {
                    if (strcmp((*config_entry_list)[i].key, tmp_line) == 0) {
                        strcpy((*config_entry_list)[i].value, sep_at + 1);
                    }
                }
            }
        }
    }
    return 0;
}
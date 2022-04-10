#ifndef MULTICONVERTOR_CONFIG_H
#define MULTICONVERTOR_CONFIG_H
//#include "config.h"

#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 256

typedef struct config_entry_ {
    char key[MAX_LENGTH];
    char value[MAX_LENGTH];
} config_entry;


void remove_spaces(char *s);

int read_config(config_entry **config_entry_list, int size);

#endif //MULTICONVERTOR_CONFIG_H

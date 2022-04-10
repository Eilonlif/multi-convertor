#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "config.h"


int runcmd(char **command_line_arguments) {
    pid_t child_pid;
    int child_status;

    child_pid = fork();
    if (child_pid == 0) {
        /* This is done by the child process. */
        execv(command_line_arguments[0], command_line_arguments);
        /* If execv returns, it must have failed. */
        printf("Unknown command\n");
        exit(0);
    } else {
        waitpid(child_pid, &child_status, 0);
        return child_status;
    }
}

int convert(char *input_dir_path, char *output_dir_path, char *file_name, char *type_convert_from, char *type_convert_to) {
    char input_file_path[MAX_LENGTH];
    char output_file_path[MAX_LENGTH];
    sprintf(input_file_path, "%s/%s.%s", input_dir_path, file_name, type_convert_from);
    sprintf(output_file_path, "%s/%s.%s", output_dir_path, file_name, type_convert_to);
    char *args[] = {"/usr/local/bin/magick",
                    input_file_path,
                    output_file_path,
                    0};
    printf("command executed:\n%s %s %s\n", args[0], args[1], args[2]);
//    TODO: return status of runcmd and check for error
//    TODO: idk how to do that...
    return runcmd(args);
}

void show_dir_content(char *input_path, char *output_path, char *file_type) {
    const char separator = '.';
    char *sep_at;
    char file_name[MAX_LENGTH];
    char file_ending[MAX_LENGTH];
    struct dirent *dir;
    int convert_ret_code;
    DIR *d = opendir(input_path);
    if (d == NULL) { return; }
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_REG) { // changed from ' != DT_DIR', according to documentation its fine
            sep_at = strchr(dir->d_name, separator);
            if (sep_at != NULL) {
                *sep_at = '\0';
                strcpy(file_name, dir->d_name);
                strcpy(file_ending, sep_at + 1);
                convert_ret_code = convert(input_path, output_path, file_name, file_ending, file_type);
//                if (convert_ret_code == something) {
//                    printf("Conversion failed");
//                    // Note: is it realy that big a deal? why not just print in `runcmd()`?
//                }
            }
        }
    }
    closedir(d);
}

void input_dir(char *input_path, char *output_path) {
    DIR *d = opendir(input_path);
    if (d == NULL) return;
    struct dirent *dir;
    char path_to_convert[MAX_LENGTH];
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
            sprintf(path_to_convert, "%s/%s", input_path, dir->d_name);
            show_dir_content(path_to_convert, output_path, dir->d_name);
        }
    }
    closedir(d);
}

int configs(char **input_path, char **output_path) {
    int size = 2;
    int read_config_status;
    *input_path = (char*) malloc(MAX_LENGTH * sizeof(char));
    *output_path = (char*) malloc(MAX_LENGTH * sizeof(char));

    config_entry *config_entry_list = malloc(size * sizeof(*config_entry_list));
    config_entry input_p;
    config_entry output_p;
    strcpy(input_p.key, "inputDirectoryPath");
    strcpy(output_p.key, "outputDirectoryPath");
    config_entry_list[0] = input_p;
    config_entry_list[1] = output_p;

    read_config_status = read_config(&config_entry_list, size);
    if (read_config_status != 0) {
        return 1;
    }
    strcpy(*input_path, config_entry_list[0].value);
    strcpy(*output_path, config_entry_list[1].value);
    return 0;
}


int main() {
    char *input_path;
    char *output_path;
    int configs_status;
    configs_status = configs(&input_path, &output_path);

    if (configs_status == 0) {
        input_dir(input_path, output_path);
    } else {
        printf("Something went wrong");
    }


    free(input_path);
    free(output_path);
    return 0;
}


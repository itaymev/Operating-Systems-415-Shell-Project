/*
 * command.c
 */

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include "command.h"
#include "string_parser.h"

void listDir() {
    char cwd[1035];
    getcwd(cwd, sizeof(cwd));
    if (sizeof(cwd) == 0) {
        char* cwd_error = "No working current directory.\n";
        write(STDOUT_FILENO, cwd_error, strlen(cwd_error));
        return;
    }

    DIR* dir = opendir(cwd);
    if (dir == NULL) {
        char* dir_null_error = "Error opening directory. Check permissions.\n";
        write(STDOUT_FILENO, dir_null_error, strlen(dir_null_error));
        return;
    }

    struct dirent *dp;

    while ((dp = readdir(dir)) != NULL) {
        write(STDOUT_FILENO, dp->d_name, strlen(dp->d_name));
        char* newline = "\n";
        write(STDOUT_FILENO, newline, strlen(newline));
    }
    closedir(dir);
}

void showCurrentDir() {
    char path[1035];
    if (getcwd(path, sizeof(path)) != NULL) {
        write(STDOUT_FILENO, path, strlen(path));
        write(STDOUT_FILENO, "\n", 1);
    } else {
        write(STDOUT_FILENO, "Error! Failed to run pwd command\n", strlen("Error! Failed to run pwd command\n"));
    }
}

void makeDir(char *dirName) {
    if (mkdir(dirName, 0777) == 0) {
        write(STDOUT_FILENO, "Directory created!\n", strlen("Directory created!\n"));
    } else {
		write(STDOUT_FILENO, "Directory already exists!\n", strlen("Directory already exists!\n"));
    }
}

void changeDir(char *dirName) {
    if (chdir(dirName) == -1) {
        write(STDOUT_FILENO, "Error! Directory does not exist\n", strlen("Error! Directory does not exist\n"));
    }
}

void copyFile(char *sourcePath, char *destinationPath){
	char line[1024];
	int read_path, write_path, n;

	read_path = open(sourcePath, O_RDONLY);
	if (read_path == -1){
		char* err_msg = "CP command failed! Failed to open source file!\n";
		write(1, err_msg, strlen(err_msg));
	}

	if (chdir(destinationPath) != -1){
		command_line tokens = str_filler(sourcePath, "/");
		int i = tokens.num_token;
		if (i > 1){
			sourcePath = tokens.command_list[i-1];
		}
		else {
			sourcePath = tokens.command_list[0];
		}
		write_path = open(sourcePath, O_CREAT | O_WRONLY | O_TRUNC, 0744);
		free_command_line(&tokens);
	}
	else {
		write_path = open(destinationPath, O_CREAT | O_WRONLY | O_TRUNC, 0744);
	}

	if (write_path == -1) {
		char* err_msg = "CP command failed! Failed to open destination file!\n";
		write(1, err_msg, strlen(err_msg));
	}

	while ((n = read(read_path, line, sizeof(line)))>0){
		write(write_path, line, n);
	}

	close(read_path);
	close(write_path);
}

void moveFile(char *sourcePath, char *destinationPath) {
    if (sourcePath != NULL && destinationPath != NULL) {
        if (rename(sourcePath, destinationPath) == 0) {
            write(STDOUT_FILENO, "File moved successfully!\n", strlen("File moved successfully!\n"));
            return;
        } else {
            write(STDERR_FILENO, "Failed to move the file\n", strlen("Failed to move the file\n"));
        }
    } else {
        write(STDERR_FILENO, "Invalid source or destination path\n", strlen("Invalid source or destination path\n"));
    }
}

void deleteFile(char *filename) {
    if (remove(filename) == -1) {
        write(STDOUT_FILENO, "Failed to run rm command\n", strlen("Failed to run rm command\n"));
    } else {
        write(STDOUT_FILENO, "File deleted successfully!\n", strlen("File deleted successfully!\n"));
    }
}

void displayFile(char *filename) {
    FILE *fp;
    char c;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        write(STDOUT_FILENO, "Failed to open file\n", strlen("Failed to open file\n"));
        return;
    }

    while ((c = fgetc(fp)) != EOF) {
        write(STDOUT_FILENO, &c, 1);
    }

    fclose(fp);
    write(STDOUT_FILENO, "\nEnd of file\n", strlen("\nEnd of file\n"));
}

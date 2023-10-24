#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string_parser.h"
#include "command.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		// interactive mode
		char* line = NULL;
		size_t len = 0;
		ssize_t read;

		while (1) {
			printf("Enter a command: ");
			read = getline(&line, &len, stdin);

			if (read == -1) {
				perror("getline error");
				break;
			}

			int stop = 0;
			char* command_delimiter = ";";
            char* sub_command_delimiter = " \n";
            char* command_token = strtok(line, command_delimiter);

            while (command_token != NULL) {
                command_line tokens = str_filler(command_token, sub_command_delimiter);

                if (tokens.command_list[0] == NULL ) {
                    free_command_line(&tokens);
                    printf("Tokens are null\n"); // test print, not necessary
                    break; // this is necessary
                }
				
				// check the tokens, preform command based on token (or dont preform command)
				if (strcmp(tokens.command_list[0], "ls") == 0) {
					listDir();
				} 
				else if (strcmp(tokens.command_list[0], "pwd") == 0) {
					showCurrentDir();
				} 
				else if (strcmp(tokens.command_list[0], "mkdir") == 0) {
					if (tokens.num_token > 1) {
						makeDir(tokens.command_list[1]);
					} 
					else {
						printf("mkdir needs a directory name argument\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "cd") == 0) {
					if (tokens.num_token > 1) {
						changeDir(tokens.command_list[1]);
					} 
					else {
						printf("cd needs a directory name argument\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "cp") == 0) {
					if (tokens.num_token > 2) {
						copyFile(tokens.command_list[1], tokens.command_list[2]);
					} 
					else {
						printf("cp needs source and destination file arguments\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "mv") == 0) {
					if (tokens.num_token > 2) {
						moveFile(tokens.command_list[1], tokens.command_list[2]);
					} 
					else {
						printf("mv needs source and destination file arguments\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "rm") == 0) {
					if (tokens.num_token > 1) {
						deleteFile(tokens.command_list[1]);
					} 
					else {
						printf("rm needs a filename argument\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "cat") == 0) {
					if (tokens.num_token > 1) {
						displayFile(tokens.command_list[1]);
					} 
					else {
						printf("cat needs a filename argument\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "exit") == 0) {
					stop = 1; // since I'm using a double loop I'll need to break twice to exit
					printf("Bye!\n");
					break;
				}
				else {
					printf("Error! Unrecognized command: %s\n", tokens.command_list[0]);
				}

				free_command_line(&tokens); // free memory allocated for tokens

				command_token = strtok(NULL, command_delimiter); // reset command_token, if it is null we will stop the loop
			}
			if (stop == 1){
				break; // here is where I truly exit
			}
		}

		free(line); // free memory allocated for line

	} else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
		// file mode
		const char* inputFile = argv[2];
		const char* outputFile = "output.txt";
		
		freopen(outputFile, "w", stdout); // Redirecting stdout to outputFile

		FILE *file = fopen(inputFile, "r");
		if (file == NULL) {
			perror("Error opening the file");
			return -1;
		}

		char *line = NULL;
		size_t len = 0;
		ssize_t read;
		while ((read = getline(&line, &len, file)) != -1) {
			char* command_delimiter = ";";
            char* sub_command_delimiter = " \n";
            command_line big_tok = str_filler(line, command_delimiter);

            for (int i = 0; big_tok.command_list[i]!=NULL; i++) {
                command_line tokens = str_filler(big_tok.command_list[i], sub_command_delimiter);
				if (tokens.command_list[0] == NULL) {
					continue;
				}
				// checking the tokens and executing commands based on the tokens found in each line
				else if (strcmp(tokens.command_list[0], "ls") == 0) {
					listDir(); // if the first token is ls
				} 
				else if (strcmp(tokens.command_list[0], "pwd") == 0) {
					showCurrentDir(); // if the first token is pwd
				} 
				else if (strcmp(tokens.command_list[0], "mkdir") == 0) {
					if (tokens.num_token > 0) {
						makeDir(tokens.command_list[1]);
					} 
					else {
						printf("mkdir needs a directory name argument\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "cd") == 0) {
					if (tokens.num_token > 1) {
						changeDir(tokens.command_list[1]);
					} 
					else {
						printf("cd needs a directory name argument\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "cp") == 0) {
					if (tokens.num_token > 2) {
						copyFile(tokens.command_list[1], tokens.command_list[2]);
					} 
					else {
						printf("cp needs source and destination file arguments\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "mv") == 0) {
					if (tokens.num_token > 2) {
						moveFile(tokens.command_list[1], tokens.command_list[2]);
					} 
					else {
						printf("mv needs source and destination file arguments\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "rm") == 0) {
					if (tokens.num_token > 1) {
						deleteFile(tokens.command_list[1]);
					} 
					else {
						printf("rm needs a filename argument\n");
					}
				}
				else if (strcmp(tokens.command_list[0], "cat") == 0) {
					if (tokens.num_token > 1) {
						displayFile(tokens.command_list[1]);
					} 
					else {
						printf("cat needs a filename argument\n");
					}
				}
				else {
					printf("Error! Unrecognized command: %s\n", tokens.command_list[0]);
				}

				free_command_line(&tokens); // free memory allocated for tokens
			}
		free_command_line(&big_tok);
		}

		free(line); // free memory allocated for line
		fclose(file);
	} else {
		printf("Usage: %s [-f <filename>] or %s", argv[0], argv[0]);
	}

	return 0;
}


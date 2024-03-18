/* Citations below:
https://stackoverflow.com/questions/22121479/printing-out-the-current-working-directory
https://stackoverflow.com/questions/7786994/c-getline-isnt-waiting-for-input-from-console-when-called-multiple-times
https://www.geeksforgeeks.org/use-fflushstdin-c/
https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-getcwd-get-path-name-working-directory
https://blog.ehoneahobed.com/building-a-simple-shell-in-c-part-1
https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-pathconf-determine-configurable-path-name-variables
https://www.geeksforgeeks.org/c-pointers/
https://manpages.ubuntu.com/manpages/lunar/man3/size_t.3type.html#:~:text=ssize_t%20Used%20for%20a%20count,%5B%2D1%2C%20SSIZE_MAX%5D.
https://www.geeksforgeeks.org/making-linux-shell-c/
https://en.cppreference.com/w/c/program/EXIT_status
https://stackoverflow.com/questions/75943541/a-function-declaration-without-a-prototype-is-deprecated-in-all-versions-of-c
https://www.geeksforgeeks.org/dup-dup2-linux-system-call/
https://www.w3schools.com/php/func_filesystem_file_exists.asp
https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/
https://www.programiz.com/c-programming/library-function/string.h/strcmp
https://www.geeksforgeeks.org/use-fflushstdin-c/
https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_INPUT 1000

void print_prompt(void) {
    char cwd[1000];
    char *val = getcwd(cwd, sizeof(cwd));
    
    if (val == NULL) {
        fprintf(stderr,"Error: invalid command\n");
        exit(EXIT_FAILURE);
    }
    
    printf("[nyush %s]$ ", basename(cwd));
    fflush(stdout);
}

char **parse_command(char *input) {
    
    char **baby_bug = malloc(MAX_INPUT * sizeof(char *));

    if (baby_bug == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *bingo = strtok(input, " \n");
    int b = 0;

    while(bingo) {
        baby_bug[b] = bingo;
        bingo = strtok(NULL, " \n");
        b++;
    }
    baby_bug[b] = NULL;
    return baby_bug;
}

int file_exists(const char *path){
    struct stat st;
    return stat(path, &st) == 0;
}

int compare(const char *str1, const char *str2) {
    for (; *str1 && (*str1 == *str2); str1++, str2++) {
        continue;
    }
    
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}


void handle_redirection(char **baby_bug) {

    for (int i = 0; baby_bug[i] != NULL; i++) {

        if (compare(baby_bug[i], "<") == 0) {

            if (baby_bug[i + 1] != NULL) {

                int f = open(baby_bug[i + 1], O_RDONLY);

                if (f < 0) {
                    fprintf(stderr, "Error: invalid file\n");
                    exit(EXIT_FAILURE);
                }

                dup2(f, STDIN_FILENO);
                close(f);
                baby_bug[i] = NULL;
            } else {
                fprintf(stderr, "Error: invalid command\n");
                exit(EXIT_FAILURE);
            }
        } else if (compare(baby_bug[i], ">") == 0) {
            
            int flags = 0;
            flags |= O_WRONLY;  
            flags |= O_CREAT;
            
            if (compare(baby_bug[i], ">>") == 0) {
                flags |= O_APPEND;
            } else {
                flags |= O_TRUNC;
            }
            
            if (baby_bug[i + 1] != NULL) {
                int f = open(baby_bug[i + 1], flags, 0644);
                if (f < 0) {
                    fprintf(stderr, "Error: invalid file\n");
                    exit(EXIT_FAILURE);
                }
                dup2(f, STDOUT_FILENO);
                close(f);
                baby_bug[i] = NULL;
            } else {
                fprintf(stderr, "Error: invalid file\n");
                exit(EXIT_FAILURE);
            }
        } else if (compare(baby_bug[i], ">>") == 0) {
            
            int flags = 0;
            flags |= O_WRONLY;  
            flags |= O_CREAT;
            
            if (compare(baby_bug[i], ">>") == 0) {
                flags |= O_APPEND;
            } else {
                flags |= O_TRUNC;
            }
            
            if (baby_bug[i + 1] != NULL) {
                int f = open(baby_bug[i + 1], flags, 0644);
                if (f < 0) {
                    fprintf(stderr, "Error: invalid file\n");
                    exit(EXIT_FAILURE);
                }
                dup2(f, STDOUT_FILENO);
                close(f);
                baby_bug[i] = NULL;
            } else {
                fprintf(stderr, "Error: invalid file\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int count_pipes(char **baby_bug) {
    int count = 0;
    for (int i = 0; baby_bug[i] != NULL; i++) {
        if (compare(baby_bug[i], "|") == 0) {
            count++;
        }
    }
    return count;
}

void execute_piped_command(char **baby_bug) {
    int pipes = count_pipes(baby_bug);
    int **pipesize = malloc(pipes * sizeof(int *));

    for (int i = 0; i < pipes; i++) {
        pipesize[i] = malloc(2 * sizeof(int));
        if (pipe(pipesize[i]) < 0) {
            fprintf(stderr,"nyush: pipe error");
            exit(EXIT_FAILURE);
        }
    }

    int start = 0;
    for (int i = 0; i <= pipes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr,"nyush: fork error");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (i < pipes) {
                dup2(pipesize[i][1], STDOUT_FILENO);
            }
            if (i > 0) {
                dup2(pipesize[i - 1][0], STDIN_FILENO);
            }
            
            for (int j = 0; j < pipes; j++) {
                close(pipesize[j][0]);
                close(pipesize[j][1]);
            }

            char **cmd = &baby_bug[start];
            int i = 0;
            while (cmd[i] != NULL && compare(cmd[i], "|") != 0) {
                i++;
            }
            cmd[i] = NULL;

            handle_redirection(cmd);

            if (execvp(cmd[0], cmd) == -1) {
                fprintf(stderr,"nyush: exec error");
                exit(EXIT_FAILURE);
            }
        } else {
            if (i > 0) {
                close(pipesize[i - 1][0]);
            }
            if (i < pipes) {
                close(pipesize[i][1]);
            }
        }
        while (baby_bug[start] != NULL && compare(baby_bug[start], "|") != 0) {
            start++;
        }
        start++;
    }

    for (int i = 0; i <= pipes; i++) {
        int status;
        wait(&status);
    }

    for (int i = 0; i < pipes; i++) {
        free(pipesize[i]);
    }
    free(pipesize);
}

void execute_command(char **baby_bug) {
    pid_t pid;
    int status;

    if (count_pipes(baby_bug) > 0) {
        execute_piped_command(baby_bug);
        return;
    }

    if (compare(baby_bug[0], "cd") == 0) {
            if (baby_bug[1] == NULL || baby_bug[2] != NULL) {
            fprintf(stderr, "Error: invalid command\n");
            return;
            }if (chdir(baby_bug[1]) != 0) {
                switch (errno) {
                    case ENOENT:
                        fprintf(stderr, "Error: invalid directory\n");
                        break;
                    case EACCES:
                        fprintf(stderr, "Error: invalid directory\n");
                        break;
                    default:
                        fprintf(stderr, "Error: invalid command\n");
                        break;
                }
            }
        
        return;
    } else if (compare(baby_bug[0], "exit") == 0) {
        exit(0); 
    } 

    pid = fork();

    if (pid < 0) {
        fprintf(stderr,"Error: invalid command\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {

        handle_redirection(baby_bug);

        if (execvp(baby_bug[0], baby_bug) == -1) {
            switch(errno) {
                case ENOENT:
                    fprintf(stderr, "Error: invalid program\n");
                    break;
                default:
                    fprintf(stderr, "Error: invalid command\n");
                    break;
            }
            exit(EXIT_FAILURE);
        }

    } else {
        wait(&status);
    }
}

int main(int argc, char *argv[]) {

    (void)argc; 
    (void)argv;

    signal(SIGINT, SIG_IGN);    
    signal(SIGQUIT, SIG_IGN);  
    signal(SIGTSTP, SIG_IGN);  

    while (1) {
        char *input = NULL;
        size_t len = 0;
        
        print_prompt();
        
        ssize_t read = getline(&input, &len, stdin);
        if (read == -1) {  
            free(input);
            break;  
        }
        
        char **baby_bug = parse_command(input);

        if (baby_bug[0]) {
            execute_command(baby_bug);
        }

        free(input);
        free(baby_bug);
    }   

    return 0;
}


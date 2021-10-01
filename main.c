#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <stdbool.h>

#define MAXLENGTH 100
#define MAXCOMMANDS 10

bool verifyEmptyInput(char* input);
void printDir();
void execute(char** input);
void handleSpace(char* input, char** parsed);
bool handleInput(char* input, char** parsed);
int verifyCommand(char** parsed);
  
int main() {
    char inputString[MAXLENGTH], *parsedArgs[MAXCOMMANDS];
    bool execFlag = false;
  
    while (true) {
        printDir();
        if (verifyEmptyInput(inputString)) continue;
        execFlag = handleInput(inputString, parsedArgs);
        if (execFlag) execute(parsedArgs);
    }
    return 0;
}

void printDir() {
    char* username = getenv("USER");
    char cwd[1024];
    char hostname[1024];
    getcwd(cwd, sizeof(cwd));
    gethostname(hostname, 1024);
    printf("%s@%s:%s", username, hostname, cwd);
}

bool verifyEmptyInput(char* input) {
    char* line;
    line = readline("$ ");
    if (strlen(line) == 0) return true;
    add_history(line);
    strcpy(input, line);
    return false;
}

bool handleInput(char* input, char** parsed) {  
    handleSpace(input, parsed);
    if (verifyCommand(parsed)) return false;
    return true;
}

void handleSpace(char* input, char** parsed) {
    int i;
    for (i = 0; i < MAXCOMMANDS; i++) {
        parsed[i] = strsep(&input, " ");
        if (parsed[i] == NULL) break;
        if (strlen(parsed[i]) == 0) i--;
    }
}

int verifyCommand(char** parsed) {
    int commndsCount = 2, i, verifier = -1;
    char* commandsList[commndsCount];
  
    commandsList[0] = "exit";
    commandsList[1] = "cd";
  
    for (i = 0; i < commndsCount; i++) {
        if (strcmp(parsed[0], commandsList[i]) == 0) {
            verifier = i;
            break;
        }
    }
  
    switch (verifier) {
    case 0:
        exit(0);
    case 1:
        chdir(parsed[1]);
        return 1;
    default:
        break;
    }
    return 0;
}

void execute(char** input) {
    pid_t pid = fork();  
    if (pid == -1) {
        printf("Couldn't fork\n");
        return;
    } else if (pid == 0) {
        if (execvp(input[0], input) < 0) {
            printf("Error executing command\n");
        }
        exit(0);
    } else {
        wait(NULL); 
        return;
    }
}
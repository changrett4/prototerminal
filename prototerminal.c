#include <stdio.h>
#include <dirent.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>

#define clear() printf("\033[H\033[J")
#define MAX_LEN_LINE 150
#define MAX_ARGS 10



int execute_ls(char* arg ){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(arg)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name);
        }
        closedir(dir);
    } else {
        perror("Error to open directory");
        return 1;
    }
}

void initial_message(){
    clear();
    printf("______________________________________");
    printf("\n\n\tProtoshell - OS");
    printf("\n\n______________________________________");
    printf("\n");
    sleep(1);
}

void take_input(char* buffer){
    fgets(buffer, MAX_LEN_LINE, stdin);
    buffer[strcspn(buffer,"\n")] = '\0'; // remove o caracter '\n' da entrada
}

int execute_commands(int n_commands,char** comandos){
    for(int i=0;i<n_commands;i++){
        if(strcmp(comandos[i], "ls") == 0){
            execute_ls((comandos[i + 1] == NULL) ? "." : comandos[i + 1]); //if ternario que verifica se o ls vem acompanhado de um diretorio ou é o atual
        }

    }
    return 0;
}



int get_separate_commands(char* buffer,char** commands){
    int n_commands = 0;
    char* command = strtok(buffer, " ");

    while (command != NULL) {
        if (n_commands == MAX_ARGS) {
            printf("Error: Max %d args exceeded\n", MAX_ARGS);
            for (int i = 0; i < n_commands; i++) {
                free(commands[i]);
            }
            return -1;
        }
        commands[n_commands] = malloc((strlen(command) + 1) * sizeof(char));
        strcpy(commands[n_commands], command);
        n_commands++;
        command = strtok(NULL, " ");
    }
    return n_commands;

}


void clear_commands(int n_commands,char **commands){
    for(int i=0; i< n_commands;i++){
        free(commands[i]);
    }
}


int main() {
    char buffer[MAX_LEN_LINE];
    int n_commands;
    char **commands= malloc((MAX_ARGS+1)*sizeof (char*)) ;
    initial_message();
    while(1){
        printf("> ");
        take_input(buffer);
        printf("%s\n",buffer);
        n_commands = get_separate_commands(buffer,commands);
        if(n_commands==-1){
            continue;
        }
        execute_commands(n_commands,commands);
        clear_commands(n_commands,commands);
    }
    free(commands);
	return 0;
}

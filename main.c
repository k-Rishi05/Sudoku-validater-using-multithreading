#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

int determineSudokuSize(const char *filename){
    FILE *file = fopen(filename,"r");
    if(file == NULL){
        printf("Error opening the file\n");
        exit(EXIT_FAILURE);
    }
    int n=0;
    char line[1024];    //Buffer to hold one line of file

    if(fgets(line,sizeof(line),file)){
        char *token = strtok(line," ");
        while(token){
            n++;
            token = strtok(NULL," ");   //Moves to next token
        }
    }
    fclose(file);
    return n;
}

int main(){
    char *filename = "sudoku_input.txt";

    //To find out size of sudoku
    int n = determineSudokuSize(filename);
    printf("%d",n);
}
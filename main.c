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
void readSudokuFromFile(const char *filename,int **grid, int n){
    FILE *file = fopen(filename,"r");
    if(file==NULL){
        printf("Error opening the file\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(fscanf(file,"%d",&grid[i][j]) != 1){         // Reads an integer and stores it in the grid
                fprintf(stderr, "Invalid input in file\n");
                fclose(file);  
                exit(EXIT_FAILURE);
            }
        }
    }
}
void printSudoku(int **grid,int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",grid[i][j]);
        }
        printf("\n");
    }
}
int main(){
    char *filename = "sudoku_input.txt";

    //To find out size of sudoku
    int n = determineSudokuSize(filename);
    printf("%d\n",n);

    if(n!=9 && n!=16 && n!=25 && n!=36 && n!=49 && n!=64){
        printf("Enter Sudoku of Valid size\n");
        return -1;
    }

    int **grid = malloc(n*sizeof(int *));
    for(int i=0;i<n;i++){
        grid[i] = malloc(n*sizeof(int));
    }

    //Reading input from sudoku_input.txt
    readSudokuFromFile(filename,grid,n);
    printSudoku(grid,n);
}
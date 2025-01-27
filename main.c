#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct{
    int thread_number;
    int start;
    int end;
    int type;   //  0 - for row, 1 - for column, 2 - for sub_grid
}Thread_info;

int n,k;
int **sudoku;
int flag = 1; //For implementing early termination

FILE* output_file_chunk;
FILE* output_file_mixed;
FILE* time_record;

double get_time_in_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1e6 + tv.tv_usec; // Convert to floating-point microseconds
}

void read_Input_From_File(const char *filename){
    FILE *file = fopen(filename,"r");
    if(file==NULL){
        printf("Error opening the file\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d %d", &k, &n) != 2) {
        fprintf(stderr, "Invalid format in the first line of the file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    sudoku = malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++){
        sudoku[i] = malloc(n * sizeof(int));
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(fscanf(file,"%d",&sudoku[i][j]) != 1){         // Reads an integer and stores it in the grid
                fprintf(stderr, "Invalid input in file\n");
                fclose(file);  
                exit(EXIT_FAILURE);
            }
        }
    }
}

void print_Sudoku(int **grid,int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",grid[i][j]);
        }
        printf("\n");
    }
}

bool check_Row(int row){
    int *hash= calloc(n,sizeof(int));

    for(int i=0;i<n;i++){
        int num= sudoku[row][i];
        if(num<1 || num>n || hash[num-1]==1){
            free(hash);
            return false;
        }
        hash[num-1]=1;
    }
    free(hash);
    return true;

}

bool check_Column(int col){
    int *hash= calloc(n,sizeof(int));

    for(int i=0;i<n;i++){
        int num= sudoku[i][col];
        if(num<1 || num>n || hash[num-1]==1){
            free(hash);
            return false;
        }
        hash[num-1]=1;
    }
    free(hash);
    return true;

}

bool check_Sub_grid(int start_row,int start_col,int grid_size){
    int *hash= calloc(n,sizeof(int));

    for(int i=start_row;i<start_row+grid_size;i++){
        for(int j=start_col;j<start_col+grid_size;j++){
            int num= sudoku[i][j];
            if(num<1 || num>n || hash[num-1]==1){
                free(hash);
                return false;
        }
        hash[num-1]=1;
        }
    }
    free(hash);
    return true;
}

void *validate_chunk(void *args){
    Thread_info *info = (Thread_info*)args;
    int grid_size = (int)sqrt(n);
    // if(flag==0)
    //     return NULL;
    for(int i=info->start;i<info->end;i++){
        bool valid = false;
        if(info->type == 0){
            valid=check_Row(i);
            if(valid)
                fprintf(output_file_chunk,"Thread %d checks Row %d and is Valid\n",info->thread_number,i+1);
            else
                fprintf(output_file_chunk,"Thread %d checks Row %d and is Invalid \n", info->thread_number,i+1);
        }
        else if(info->type == 1){
            valid=check_Column(i);
            if(valid)
                fprintf(output_file_chunk,"Thread %d checks Column %d and is Valid\n",info->thread_number,i+1);
            else
                fprintf(output_file_chunk,"Thread %d checks Column %d and is Invalid\n", info->thread_number,i+1);
        }
        else if(info->type ==2){
            int start_row = (i/grid_size)*grid_size;
            int start_col = (i%grid_size)*grid_size;
            valid=check_Sub_grid(start_row,start_col,grid_size);
            if(valid)
                fprintf(output_file_chunk,"Thread %d checks grid %d and is Valid\n",info->thread_number,i+1);
            else
                fprintf(output_file_chunk,"Thread %d checks grid %d and is Invalid\n", info->thread_number,i+1);
        }

        if(!valid)
            flag=0;
        
    }

    return NULL;
}

void *validate_mixed(void* args){
    Thread_info *info = (Thread_info*)args;
    int grid_size = (int)sqrt(n);
    // if(flag==0)
    //     return NULL;

    if(info->type==0){
        for(int i=info->start;i<n;i=i+(int)k/3){
            bool valid = false;
            valid=check_Row(i);
            if(valid)
                fprintf(output_file_mixed,"Thread %d checks Row %d and is Valid\n",info->thread_number,i+1);
            else
                fprintf(output_file_mixed,"Thread %d checks Row %d and is Invalid \n", info->thread_number,i+1);

            if(!valid)
            flag=0;
        }
    }

    else if(info->type==1){
        for(int i=info->start;i<n;i=i+(int)k/3){
            bool valid = false;
            valid=check_Column(i);
            if(valid)
                fprintf(output_file_mixed,"Thread %d checks Row %d and is Valid\n",info->thread_number,i+1);
            else
                fprintf(output_file_mixed,"Thread %d checks Row %d and is Invalid \n", info->thread_number,i+1);

            if(!valid)
            flag=0;
        }
    }

    else if(info->type==2){
        for(int i=info->start;i<n;i=i+(k-2*(int)k/3)){
            bool valid = false;
            int start_row = (i/grid_size)*grid_size;
            int start_col = (i%grid_size)*grid_size;
            valid=check_Sub_grid(start_row,start_col,grid_size);
            if(valid)
                fprintf(output_file_mixed,"Thread %d checks Row %d and is Valid\n",info->thread_number,i+1);
            else
                fprintf(output_file_mixed,"Thread %d checks Row %d and is Invalid \n", info->thread_number,i+1);

            if(!valid)
            flag=0;
        }
    }

    return NULL;
}

void chunk_Method(){
    pthread_t threads[k];
    Thread_info info[k];
    int number_of_row_threads = k/3;
    int number_of_col_threads = k/3;
    int number_of_sub_grid_threads = k-2*(int)(k/3);
    int row_chunk_size = n/number_of_row_threads;
    int col_chunk_size = n/number_of_col_threads;
    int sub_grid_chunk_size  = n/number_of_sub_grid_threads;
    int grid_chunks = n/((int)sqrt(n));

    //Row Checking

    for(int i=0;i<number_of_row_threads;i++){
        info[i].thread_number = i+1;
        info[i].start = i*row_chunk_size;
        if(i<number_of_row_threads-1)
            info[i].end = (i+1)*row_chunk_size;
        else    
            info[i].end  = n;
        
        info[i].type  = 0;

        pthread_create(&threads[i],NULL,validate_chunk,&info[i]);
    }

    //Column Checking

    for(int i=number_of_row_threads;i<number_of_row_threads+number_of_col_threads;i++){
        info[i].thread_number = i+1;
        info[i].start = (i-number_of_row_threads)*col_chunk_size;
        if(i<number_of_row_threads+number_of_col_threads-1)
            info[i].end = (i-number_of_row_threads+1)*col_chunk_size;
        else    
            info[i].end  = n;
        
        info[i].type  = 1;

        pthread_create(&threads[i],NULL,validate_chunk,&info[i]);
    }

    //Sub_grid checking

    for(int i=number_of_row_threads+number_of_col_threads;i<k;i++){
        info[i].thread_number = i+1;
        info[i].start = (i-number_of_row_threads-number_of_col_threads)*sub_grid_chunk_size;
        if(i<k-1)
            info[i].end = (i-number_of_row_threads-number_of_col_threads+1)*sub_grid_chunk_size;
        else    
            info[i].end  = n;
        
        info[i].type  = 2;

        pthread_create(&threads[i],NULL,validate_chunk,&info[i]);
    }
    for(int i=0;i<k;i++){
        pthread_join(threads[i],NULL);
    }


    if(flag)
        fprintf(output_file_chunk,"\nSudoku is valid\n");
    else
        fprintf(output_file_chunk,"\nSudoku is invalid\n");
}

void Sequential_Method(){
    bool valid = true;
    int grid_size = (int)sqrt(n);
    for(int i=0;i<n;i++){

        //Row Checking
        valid=check_Row(i);
        if(!valid)
            flag=0;
        
        //Column Checking
        valid=check_Column(i);
        if(!valid)
            flag=0;
        
        //Sub_grid checking
        int start_row = (i/grid_size)*grid_size;
        int start_col = (i%grid_size)*grid_size;
        valid = check_Sub_grid(start_row,start_col,grid_size);
        if(!valid)
            flag=0;
        
    }
    // if(flag)
    //     fprintf(output_file_chunk,"\nSudoku is valid\n");
    // else
    //     fprintf(output_file_chunk,"\nSudoku is invalid\n");
}

void mixed_Method(){
    pthread_t threads[k];
    Thread_info info[k];
    int number_of_row_threads = k/3;
    int number_of_col_threads = k/3;
    int number_of_sub_grid_threads = k-2*(int)(k/3);
    int row_chunk_size = n/number_of_row_threads;
    int col_chunk_size = n/number_of_col_threads;
    int sub_grid_chunk_size  = n/number_of_sub_grid_threads;
    int grid_chunks = n/((int)sqrt(n));

    //Row Checking

    for(int i=0;i<number_of_row_threads;i++){
        info[i].thread_number = i+1;
        info[i].start = i;
        info[i].type  = 0;

        pthread_create(&threads[i],NULL,validate_mixed,&info[i]);
    }

    //Column Checking

    for(int i=number_of_row_threads;i<number_of_row_threads+number_of_col_threads;i++){
        info[i].thread_number = i+1;
        info[i].start = (i-number_of_row_threads);
        info[i].type  = 1;

        pthread_create(&threads[i],NULL,validate_mixed,&info[i]);
    }

    //Sub_grid checking

    for(int i=number_of_row_threads+number_of_col_threads;i<k;i++){
        info[i].thread_number = i+1;
        info[i].start = (i-number_of_row_threads-number_of_col_threads);
        info[i].type  = 2;

        pthread_create(&threads[i],NULL,validate_mixed,&info[i]);
    }
    for(int i=0;i<k;i++){
        pthread_join(threads[i],NULL);
    }


    if(flag)
        fprintf(output_file_mixed,"\nSudoku is valid\n");
    else
        fprintf(output_file_mixed,"\nSudoku is invalid\n");
}

int main(int argc, const char *argv[]){
    output_file_chunk = fopen("output_chunk.txt", "w");
    output_file_mixed = fopen("output_mixed.txt", "w");
    time_record = fopen("time_record.txt", "w");
    if (output_file_chunk == NULL) {
        printf("Failed to open output file.\n");
        return EXIT_FAILURE;
    }
    if (output_file_mixed == NULL) {
        printf("Failed to open output file.\n");
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];

    //Reading input from sudoku_input.txt
    read_Input_From_File(filename);

    double start, end;

    start = get_time_in_microseconds();
    mixed_Method();
    end = get_time_in_microseconds();
    fprintf(output_file_mixed,"The total Time taken is %.2f microseconds\n\n", (end - start));
    fprintf(time_record,"%.2f ",(end - start));

    flag=1;
    start = get_time_in_microseconds();
    chunk_Method();
    end = get_time_in_microseconds();
    fprintf(output_file_chunk,"The total Time taken is %.2f microseconds\n", (end - start));
    fprintf(time_record,"%.2f ",(end - start));

    flag = 1; // Reset flag for sequential method
    start = get_time_in_microseconds();
    Sequential_Method();
    end = get_time_in_microseconds();
    fprintf(time_record,"%.2f ", (end - start));

}
**************************************
SUDOKU VALIDATOR USING MULTI-THREADING
**************************************

All the three methods chunk, mixed and sequential are implemented in Assgn1Src-es23btech11033_sudoku.c file.
Early termination is implemented in Assgn1Src-<es23btech11033>_sudoku_termination.c file.

**************
FILE STRUCTURE
**************

Input is taken from the file name mentioned while executing. The input.txt file should be in the same directory

Output is given in 3 different file 
time_record.txt - gives time of execution for mixed, chunk and sequential
output_chunk.txt - Gives Required output format for chunk method
output_mixed.txt - Gives Required output format for mixed method

**********************
GRAPH PLOTTING SCRIPTS
**********************

run_and_plot_exp1.py -> Plots results for Experiment 1 (Effect of increasing Sudoku grid size on performance).

run_and_plot_exp2.py -> Plots results for Experiment 2 (Effect of increasing threads on validation time).


***********************
HOW TO EXECUTE THE CODE 
***********************

******** Without early termination ********

gcc Assgn1Src-es23btech11033_sudoku.c -o sudoku -lm 
./sudoku inp.txt          

******** With early termination ********

gcc Assgn1Src-es23btech11033_sudoku_termination.c -o sudoku -lm 
./sudoku inp.txt


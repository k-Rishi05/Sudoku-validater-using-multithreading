# Sudoku Validator using Multithreading

## Overview

This project implements a **Sudoku Validator** using three different approaches:

- **Chunk-Based Multithreading**
- **Mixed-Threading Approach**
- **Sequential Validation**

Additionally, an **early termination feature** is implemented to improve efficiency when an invalid Sudoku is detected early.

---

## **File Structure**

### **Source Files:**

- `sudoku.c` → Contains implementations of **Chunk-Based, Mixed-Threading, and Sequential** methods.
- `sudoku_with_early_termination.c` → Implements validation with early termination.

### **Input & Output Files:**

- `input.txt` → Contains the Sudoku grid for validation (should be in the same directory as the executable).
- `time_record.txt` → Records execution times for **Mixed, Chunk, and Sequential** methods.
- `output_chunk.txt` → Stores the formatted validation results for the **Chunk-Based** method.
- `output_mixed.txt` → Stores the formatted validation results for the **Mixed-Threading** method.

### **Graph Plotting Scripts:**

- `run_and_plot_exp1.py` → Plots results for **Experiment 1** (Effect of increasing threads on validation time).
- `run_and_plot_exp2.py` → Plots results for **Experiment 2** (Effect of increasing Sudoku grid size on performance).

---

## **How to Compile and Run**

### **Without Early Termination**

```sh
# Compile the code
gcc sudoku.c -o sudoku -lm

# Execute with an input file
./sudoku input.txt
```

### **With Early Termination**

```sh
# Compile the code with early termination
gcc sudoku_with_early_termination.c -o sudoku -lm

# Execute with an input file
./sudoku input.txt
```

---

## **Experimental Setup**

This project includes two key experiments:

1. **Experiment 1:** Evaluates performance based on increasing **thread count** (for a fixed 36×36 Sudoku grid).
2. **Experiment 2:** Evaluates performance based on increasing **Sudoku grid size** (with a fixed thread count of 8).

The results are visualized using Python scripts (`run_and_plot_exp1.py` & `run_and_plot_exp2.py`).

---

## **Results & Observations**

- **Chunk-Based method consistently outperforms Mixed-Threading** due to reduced synchronization overhead and better memory access patterns.
- **Early Termination significantly improves execution time** when an invalid Sudoku is detected early.
- **Sequential validation is the slowest**, making multithreading crucial for larger grids.

---

## **Future Improvements**

- Implement GPU-based validation for further acceleration.
- Optimize thread scheduling to adapt dynamically to different grid sizes.
- Improve memory access patterns for further performance gains.

---

## **Authors**

- Rishikesh Kasireddy

---

## **License**

This project is licensed under the **MIT License**.

---

Happy Coding! 🚀

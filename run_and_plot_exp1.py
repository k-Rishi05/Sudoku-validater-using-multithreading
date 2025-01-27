import matplotlib.pyplot as plt

# Data
sudoku_size = [9, 25, 64, 100, 256, 625, 900]
chunk_time = [159,177,185,158,195,264,418]  # Add your data here
mixed_time = [185,239,218,203,247,339,482]  # Add your data here
sequential_time = [5,12,36,66,173,467,868]  # Add your data here

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(sudoku_size, chunk_time, marker='o', label="Chunk-Based")
plt.plot(sudoku_size, mixed_time, marker='s', label="Mixed-Threading")
plt.plot(sudoku_size, sequential_time, marker='^', label="Sequential")

# Graph customization
plt.title("Sudoku Validation: Methods vs Time (8 threads)", fontsize=16)
plt.xlabel("Sudoku Size (N x N)", fontsize=14)
plt.ylabel("Running Time (microseconds)", fontsize=14)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(fontsize=12)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)

# Display the graph
plt.tight_layout()
plt.show()


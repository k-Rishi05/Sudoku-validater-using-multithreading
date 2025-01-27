import matplotlib.pyplot as plt

# Data
total_threads = [4,8,16,32]
chunk_time = [146,208,333,655]  # Add your data here
mixed_time = [154,241,370,662]  # Add your data here
sequential_time = [35,36,39,36]  # Add your data here

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(total_threads, chunk_time, marker='o', label="Chunk-Based")
plt.plot(total_threads, mixed_time, marker='s', label="Mixed-Threading")
plt.plot(total_threads, sequential_time, marker='^', label="Sequential")

# Graph customization
plt.title("Sudoku Validation: Methods vs Time (36x36 Sudoku)", fontsize=16)
plt.xlabel("Thread Number", fontsize=14)
plt.ylabel("Running Time (microseconds)", fontsize=14)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(fontsize=12)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)

# Display the graph
plt.tight_layout()
plt.show()
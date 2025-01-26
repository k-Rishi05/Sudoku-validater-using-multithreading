import random

# Generate a random 64x64 Sudoku grid
n = 64  # Grid size
sudoku = [[random.randint(1, n) for _ in range(n)] for _ in range(n)]

# Write the grid to a file
filename = "64x64_sudoku.txt"
with open(filename, "w") as f:
    # Write n and k as the first line (assuming k = 16 for testing)
    f.write(f"8 {n}\n")
    # Write the Sudoku grid
    for row in sudoku:
        f.write(" ".join(map(str, row)) + "\n")

filename
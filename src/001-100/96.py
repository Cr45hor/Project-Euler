import pulp
import numpy as np

def solve_sudoku(grid: np.ndarray) -> np.ndarray:
    prob = pulp.LpProblem("Sudoku", pulp.LpMinimize)
    
    # Create binary variables for each cell and digit
    x = pulp.LpVariable.dicts("x", (range(9), range(9), range(1, 10)), cat='Binary')

    # Each cell must contain exactly one digit
    for i in range(9):
        for j in range(9):
            prob += pulp.lpSum(x[i][j][k] for k in range(1, 10)) == 1
    
    # Each digit must appear exactly once in each row
    for i in range(9):
        for k in range(1, 10):
            prob += pulp.lpSum(x[i][j][k] for j in range(9)) == 1
    
    # Each digit must appear exactly once in each column
    for j in range(9):
        for k in range(1, 10):
            prob += pulp.lpSum(x[i][j][k] for i in range(9)) == 1
    
    # Each digit must appear exactly once in each 3x3 subgrid
    for block_i in range(3):
        for block_j in range(3):
            for k in range(1, 10):
                prob += pulp.lpSum(x[i][j][k] for i in range(block_i * 3, (block_i + 1) * 3) for j in range(block_j * 3, (block_j + 1) * 3)) == 1
    
    # Set the initial values from the grid
    for i in range(9):
        for j in range(9):
            if grid[i, j] != 0:
                prob += (x[i][j][grid[i, j]] == 1)
    
    # Solve the problem
    prob.solve(pulp.PULP_CBC_CMD(msg=0))

    # Extract the solution
    solution = np.zeros((9, 9), dtype=int)
    for i in range(9):
        for j in range(9):
            for k in range(1, 10):
                if pulp.value(x[i][j][k]) == 1:
                    solution[i, j] = k
                    break
    
    return solution

fin = open("0096_sudoku.txt", "r")
lines = fin.readlines()

grids = []
for i in range(50):
    grid_str = lines[i * 10 + 1 : (i + 1) * 10]
    grid = np.zeros((9, 9), dtype=int)
    for i in range(9):
        line = grid_str[i].strip()
        for j in range(9):
            grid[i, j] = int(line[j])
    grids.append(grid)

sum = 0
for grid in grids:
    solution = solve_sudoku(grid)
    sum += solution[0, 0] * 100 + solution[0, 1] * 10 + solution[0, 2]
print(sum)
import pulp

# 1. The Guesses and their "Correct" counts
data = [
    ("5616185650518293", 2), ("3847439647293047", 1), ("5855462940810587", 3),
    ("9742855507068353", 3), ("4296849643607543", 3), ("3174248439465858", 1),
    ("4513559094146117", 2), ("7890971548908067", 3), ("8157356344118483", 1),
    ("2615250744386899", 2), ("8690095851526254", 3), ("6375711915077050", 1),
    ("6913859173121360", 1), ("6442889055042768", 2), ("2321386104303845", 0),
    ("2326509471271448", 2), ("5251583379644322", 2), ("1748270476758276", 3),
    ("4895722652190306", 1), ("3041631117224635", 3), ("1841236454324589", 3),
    ("2659862637316867", 2)
]

# 2. Setup the Problem
prob = pulp.LpProblem("Mastermind_Solver", pulp.LpMinimize)

# Variables: x[pos][digit] is 1 if that digit is at that position
choices = range(10)
positions = range(16)
x = pulp.LpVariable.dicts("x", (positions, choices), 0, 1, pulp.LpBinary)

# 3. Constraints
# Rule A: Each position must have exactly one digit
for i in positions:
    prob += pulp.lpSum([x[i][d] for d in choices]) == 1

# Rule B: Apply the guesses
for guess_str, score in data:
    guess_digits = [int(d) for d in guess_str]
    prob += pulp.lpSum([x[i][guess_digits[i]] for i in positions]) == score

# 4. Solve
status = prob.solve(pulp.PULP_CBC_CMD(msg=0))

if pulp.LpStatus[status] == 'Optimal':
    result = ""
    for i in positions:
        for d in choices:
            if pulp.value(x[i][d]) == 1:
                result += str(d)
    print(f"The unique solution is: {result}")
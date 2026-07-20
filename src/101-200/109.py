from itertools import combinations_with_replacement

# All 62 distinct target regions on a dartboard, plus 0 for a "miss" (63 total).
# Note: Python treats duplicate integer values (e.g. S3=3, T1=3) as distinct 
# items in the list, so they are correctly calculated as separate regions.
regions = [0] + [n * m for n in range(1, 21) for m in (1, 2, 3)] + [25, 50]

# The 21 valid finishing doubles (D1-D20, plus inner bullseye D25)
doubles = [n * 2 for n in range(1, 21)] + [50]

valid_checkouts = 0

# combinations_with_replacement yields pairs where order doesn't matter.
# (0, 0) simulates 1-dart checkouts; (0, region) simulates 2-dart checkouts.
for dart1, dart2 in combinations_with_replacement(regions, 2):
    for final_dart in doubles:
        if dart1 + dart2 + final_dart < 100:
            valid_checkouts += 1

print(valid_checkouts)
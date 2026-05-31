from decimal import Decimal, getcontext

getcontext().prec = 200

N = 100

sum_digit = 0
for n in range(1, N + 1):
    nn = Decimal(n)
    sqrt_nn = nn.sqrt()
    sqrt_nn_int_part = int(sqrt_nn)
    sqrt_nn_decimal_part = sqrt_nn - sqrt_nn_int_part
    if sqrt_nn_decimal_part == 0:
        continue

    int_part_str = str(sqrt_nn_int_part)
    decimal_part_str = str(sqrt_nn_decimal_part)[2:]

    sum_digit += sum(int(d) for d in int_part_str + decimal_part_str[:100 - len(int_part_str)])
print(sum_digit)
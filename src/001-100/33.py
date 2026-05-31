def compare_fraction(num1, den1, num2, den2):
    return num1 * den2 == num2 * den1

curious_fractions = set()
for num in range(10, 100):
    for den in range(num + 1, 100):
        num_str = str(num)
        den_str = str(den)
        common_digits = set(num_str) & set(den_str)
        for digit in common_digits:
            if digit == '0':
                continue
            new_num_str = num_str.replace(digit, '', 1)
            new_den_str = den_str.replace(digit, '', 1)
            if new_num_str == '' or new_den_str == '':
                continue
            new_num = int(new_num_str)
            new_den = int(new_den_str)
            if compare_fraction(num, den, new_num, new_den):
                curious_fractions.add((num, den))

print(curious_fractions)
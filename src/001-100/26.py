def decimal_representation(a, b):
    # Assume a < b
    # Return the decimal representation of a/b 
    # Using two parts: the non-repeating part and the repeating part
    assert a < b
    index_digit = 0
    remainder = {a: index_digit}
    decimal_digits = []
    while True:
        index_digit += 1
        a *= 10
        q, r = divmod(a, b)
        decimal_digits.append(q)
        a = r
        if r in remainder:
            # We have a repeating part
            non_repeating_part = decimal_digits[:remainder[r]]
            repeating_part = decimal_digits[remainder[r]:]
            return non_repeating_part, repeating_part
        else:
            remainder[r] = index_digit
    return None, None

reciprocal_list = [(d, decimal_representation(1, d)) for d in range(2, 1000)]
print(max(reciprocal_list, key=lambda x: len(x[1][1])))

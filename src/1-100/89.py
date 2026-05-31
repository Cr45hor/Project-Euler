import roman

roman_digits = {
    "I": 1,
    "V": 5,
    "X": 10,
    "L": 50,
    "C": 100,
    "D": 500,
    "M": 1000
}

res = 0
with open("0089_roman.txt") as f:
    data = f.read().splitlines()
    for numerals in data:
        value = 0
        for i in range(len(numerals)):
            if i + 1 < len(numerals) and roman_digits[numerals[i]] < roman_digits[numerals[i + 1]]:
                value -= roman_digits[numerals[i]]
            else:
                value += roman_digits[numerals[i]]
        minimal_numerals = roman.toRoman(value)
        characters_saved = len(numerals) - len(minimal_numerals)
        res += characters_saved

print(res)
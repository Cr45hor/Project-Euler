digit_words = {
    0: "zero",
    1: "one",
    2: "two",
    3: "three",
    4: "four",
    5: "five",
    6: "six",
    7: "seven",
    8: "eight",
    9: "nine",
}

ten_plus_words = {
    10: "ten",
    11: "eleven",
    12: "twelve",
    13: "thirteen",
    14: "fourteen",
    15: "fifteen",
    16: "sixteen",
    17: "seventeen",
    18: "eighteen",
    19: "nineteen"
}

x10_words = {
    20: "twenty",
    30: "thirty",
    40: "forty",
    50: "fifty",
    60: "sixty",
    70: "seventy",
    80: "eighty",
    90: "ninety"
}

def number_to_words(n):
    if n < 10:
        return digit_words[n]
    
    if n < 20:
        return ten_plus_words[n]

    if n < 100:
        if n in x10_words:
            return x10_words[n]
        else:
            return x10_words[n - n % 10] + '-' + digit_words[n % 10]
        
    if n < 1000:
        assert n >= 100
        r = n % 100
        words = digit_words[n // 100] + ' hundred'
        if r > 0:
            words += ' and ' + number_to_words(r)
        return words
    
    assert n == 1000
    return 'one thousand'

def number_letter_count(n):
    return len(number_to_words(n).replace(' ', '').replace('-', ''))

print(sum([number_letter_count(i) for i in range(1, 1001)]))
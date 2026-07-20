def main(a, b):
    res = 0
    while a > 0:
        res += a
        a, b = -b % a, a
    return res

print(main(1504170715041707, 4503599627370517))
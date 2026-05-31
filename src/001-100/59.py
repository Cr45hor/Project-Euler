import itertools

with open("0059_cipher.txt") as fin:
    cipher = list(map(int, fin.read().split(",")))

    fout = open("0059_decrypted.txt", "w")
    
    # For each combination of three lowercase letters, XOR the cipher with the key and check if the result is valid English text.
    for key in itertools.product(range(97, 123), repeat=3):
        decrypted = [c ^ key[i % 3] for i, c in enumerate(cipher)]
        if all(32 <= c <= 126 for c in decrypted):  # Check if all characters are printable
            # 1. Check for incomplete parentheses
            if decrypted.count(ord('(')) != decrypted.count(ord(')')):
                continue
            if decrypted.count(ord('[')) != decrypted.count(ord(']')):
                continue
            if decrypted.count(ord('{')) != decrypted.count(ord('}')):
                continue
            
            fout.write("Key: " + ''.join(map(chr, key)) + "\n")
            fout.write("Decrypted text: " + ''.join(map(chr, decrypted)) + "\n")
            fout.write("Sum of ASCII values: " + str(sum(decrypted)) + "\n")

    fout.close()

    # key = "exp"
    # decrypted = [c ^ ord(key[i % 3]) for i, c in enumerate(cipher)]
    # print("Decrypted text: " + ''.join(map(chr, decrypted)))
    # print(sum(decrypted))
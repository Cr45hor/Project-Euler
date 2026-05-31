from itertools import permutations
from collections import Counter

def anagram_list(words: list[str]) -> list[tuple[str, str, list[str]]]:
    words_extended = [(w, sorted(w)) for w in words]
    words_extended.sort(key=lambda x: x[1])
    
    anagram_groups = []
    l = 0
    r = 0
    while l < len(words_extended):
        while r < len(words_extended) and words_extended[r][1] == words_extended[l][1]:
            r += 1
        
        if r - l > 1:
            # Create a counter for characters in the representative
            # Then forms a representative string of frequencies of characters in the representative in ascending order of frequency
            f = Counter(words_extended[l][1])
            representative = "".join(f"{count}" for char, count in sorted(f.items(), key=lambda x: x[1]))
            anagram_groups.append((representative, words_extended[l][1], [w[0] for w in words_extended[l:r]]))
        
        l = r
    
    return anagram_groups


fin = open("0098_words.txt", "r")
line = fin.readline()
words = [w.strip('"') for w in line.split(",")]
anagram_words = anagram_list(words)

squares = [str(n * n) for n in range(1, 10**5)]
anagram_squares = anagram_list(squares)

# Separate anagram squares by their length of their representatives
squares_by_length = {}
for representative, key, group in anagram_squares:
    length = len(key)
    if length not in squares_by_length:
        squares_by_length[length] = []
    squares_by_length[length].append((representative, key, group))

# Matching 
res = 0
for representative, key, group in anagram_words:
    length = len(key)
    if length not in squares_by_length:
        continue

    for digit_representative, digit_key, digit_group in squares_by_length[length]:
        # Match the representative of the anagram group with the representative of the square group first
        if representative != digit_representative:
            continue

        # Permute the digits in the square and check if it matches the anagram group
        for perm in permutations(digit_key):
            # Check if the mapping is bijective first
            bijective = True
            for i in range(len(key)):
                for j in range(i + 1, len(key)):
                    if (key[i] == key[j]) != (perm[i] == perm[j]):
                        bijective = False
                        break
            if not bijective:
                continue

            mapping = {key[i]: perm[i] for i in range(len(key))}

            # Apply mapping rule to all strings in the anagram group
            # Then count how many of them are in the digit group
            count = 0
            square_candidates = []
            for w in group:
                mapped = "".join(mapping[c] for c in w)
                if mapped in digit_group:
                    count += 1
                    square_candidates.append(mapped)
            
            if count > 1:
                # print(f"Anagram group: {group}, Square candidates: {square_candidates}")
                res = max(res, max(int(s) for s in square_candidates))
print(res)
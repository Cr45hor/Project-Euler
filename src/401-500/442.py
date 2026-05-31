pow11 = [11 ** n for n in range(1, 19)]

str_pow11 = [str(p) for p in pow11]

# Calculate the longest common substring between each pairs of the strings in str_pow11
def longest_common_substring(s1, s2):
    m = len(s1)
    n = len(s2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    longest = 0
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
                longest = max(longest, dp[i][j])
            else:
                dp[i][j] = 0
    return longest


for i in range(len(str_pow11)):
    for j in range(i + 1, len(str_pow11)):
        length = longest_common_substring(str_pow11[i], str_pow11[j])
        # print(f"Longest common substring between {pow11[i]} and {pow11[j]} is of length {length}")
        if length == len(str_pow11[i]) or length == len(str_pow11[j]):
            print(f"Found a pair: {pow11[i]} and {pow11[j]} with longest common substring of length {length}")
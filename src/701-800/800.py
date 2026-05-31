from scipy.special import lambertw
import math

print(math.log(800800) * 800800)

p = 11
q = 1
while p * math.log(q) + q * math.log(p) < math.log(800800) * 800800:
    q += 1
print(p, q)
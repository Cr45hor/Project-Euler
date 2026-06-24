import numpy as np
import matplotlib.pyplot as plt

N = 4
x, y = np.ogrid[:(2**N), :(2**N)]

x_centered = x - (2**(N-1))
y_centered = y - (2**(N-1))
mask = (x_centered**2 + y_centered**2) <= (2**(N-1))**2

plt.imshow(mask, cmap='gray')
plt.axis('off')
plt.show()
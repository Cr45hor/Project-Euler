import numpy as np

def trial(n: int, p: float):
    rng = np.random.default_rng()
    l = rng.geometric(1 - p)
    samples = rng.binomial(l - 1, 1 - p, n)
    return np.prod(samples)
    

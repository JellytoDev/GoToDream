import numpy as np
import matplotlib.pyplot as plt
import random
from math import factorial

def binomial(k,n,p):
    nck = factorial(n)/(factorial(k)*factorial(n-k))
    pd = nck*p**k*(1-p)**(n-k)
    return pd

x = np.arange(16)
pd1 = np.array([binomial(k, 15, 0.3) for k in range(16)])
print(pd1)
plt.ylim(0, 0.3)
plt.text(12.5, 0.28, 'n, p = 15, 0.3')
plt.bar(x, pd1, color='lightcoral')
plt.show()

total_try = 5000
[[0,0]]

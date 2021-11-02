import matplotlib.pyplot as plt
import numpy as np
import random

total_try = 100000
try_ = 0
innner_try = 0
r1 = np.arange(0.600,0.800,0.004)
for i in r1:
    for _ in range(total_try):
        a = random.random()
        b = random.random()
        r=(a**2+b**2)**0.5
        while r > 1:
            a = random.random()
            b = random.random()
            r=(a**2+b**2)**0.5
        try_+=1
        if r<i:
            innner_try+=1
    print(i," : ",innner_try/try_)
            
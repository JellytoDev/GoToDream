import matplotlib.pyplot as plt
import random
import numpy as np

fre = []
prob = []
n = 0
total_try = 3000

# range는 배열을 만들때 쓰는 함수이다.

for j in range(1,total_try+1):
    i = random.randint(1,6)
    if(i==3):
        n+=1
    fre.append(n/j)
    prob.append(1/6)

plt.plot(range(1,total_try+1),fre,'b',range(1,total_try+1),prob,'r')
plt.legend(["releative frequency","probability"])
plt.show()
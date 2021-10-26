import matplotlib.pyplot as plt
import random
import numpy as np

fre = []
fre2 = []
prob = []
prob2 = []
n1=0
n2=0

total_try = 5000

for j in range(1,total_try+1):
    i = random.randint(1,6)
    if(i==3):
        n1+=1
        i = random.randint(1,6)
        if(i==3):
            n2+=1
    fre.append(n2/j)
    if(n1!=0):
        fre2.append(n2/n1)
    else:
        fre2.append(0)

    prob.append(1/36)
    prob2.append(1/6)
    
plt.subplot(211)
plt.plot(range(1,total_try+1),fre,'b',range(1,total_try+1),prob,'r')
plt.legend(["Relative frequency","Probabilty"])

plt.subplot(212)
plt.plot(range(1,total_try+1),fre2,'b',range(1,total_try+1),prob2,'r')
plt.legend(["Relative frequency","Probabilty"])
plt.show()
import matplotlib.pyplot as plt
import random
import numpy as np
import math

def pmf(list_,n):
    x_range = range(1,5*n)
    n_x = [0]*len(x_range)

    for i in x_range:
        for j in list_:
            if i == j:
                n_x[i-1]+= 1/len(list_)
    return n_x,x_range

total = 500
n = 20
rand_v = []
for i in range(total):
    s = 0
    for j in range(n):
        s+=random.randint(1,5)
    rand_v.append(s)


pmf_s,x_s = pmf(rand_v,n)
m = n*(5+1)/2
sig = n**0.5*(5-1)**2/12
pdf_a = []
x_list = np.linspace(n,n*5,200)
for x_ in x_list:
    pdf = 1/((2*math.pi)**0.5*sig)*math.exp(-(x_-m)**2/(2*sig**2))
    pdf_a.append(pdf)

plt.bar(x_s,pmf_s)
plt.plot(x_list,pdf_a,'r-')
plt.legend(['analy','sim'])
plt.show()
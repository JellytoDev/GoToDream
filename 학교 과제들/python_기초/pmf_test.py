import matplotlib.pyplot as plt
import random
import math

total = 1000
n = 2
rand_v = []

def pmf(_list,n):
    pmf_set = [0]*5
    x_set = []*len(_list)
    for i in _list:
        m = math.floor(i/n)
        x_set.append(m)

        if m==1:
            pmf_set[0] += 1/len(_list)
        if m==2:
            pmf_set[1] += 1/len(_list)
        if m==3:
            pmf_set[2] += 1/len(_list)
        if m==4:
            pmf_set[3] += 1/len(_list)
        if m==5:
            pmf_set[4] += 1/len(_list)

    #print(_list)
    #print(x_set)

    return pmf_set
        
        
for i in range(total):
    s=0
    for j in range(n):
        s+= random.randint(1,5)
    rand_v.append(s)


pmf_set = pmf(rand_v,n)
pmf_prob = [0.2]*5
x_set = [x for x in range(1,6)]

plt.bar(x_set,pmf_set)
plt.plot(x_set,pmf_prob,'r')
plt.show()
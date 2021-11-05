import matplotlib.pyplot as plt
import random

total = 1000
n = 200
rand_v = []

def pmf_f(_list):
    max = _list[0]
    min = _list[0]

    for i in _list:
        if i>max:
            max = i
        if i<min:
            min = i
    pmf_set = [0]*(max-min+1)
    x_set = [x for x in range(min,max+1)]
    for i in _list:
        index = i-min
        pmf_set[index] += 1/len(_list)


    return pmf_set,x_set

for i in range(total):
    s = 0
    for j in range(n):
        s+= random.randint(1,5)
    rand_v.append(s)

pmf,x = pmf_f(rand_v)

print(len(x),len(pmf))

plt.bar(x,pmf)
plt.show()



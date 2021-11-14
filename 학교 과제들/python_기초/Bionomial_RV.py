import matplotlib.pyplot as plt
import random
import math

def pmf(list_,n_):
    x_range = range(0,n_+1)
    n_x=[0]*len(x_range)
    for i in x_range:
        for j in list_:
            if i== j:
                n_x[i]+= 1/len(list_)

    return n_x
u_suc = []
n = 5
p=0.4
total_try = 5000
for _ in range(total_try):
    n_s = 0
    for _ in range(n):
        rand_i = random.random()
        if rand_i <= p:
            n_s += 1
        
    u_suc.append(n_s)
pmf_s = pmf(u_suc,n)


pmf_a=[]
for k in range(0,n+1):
    prob = (math.factorial(n)/math.factorial(n-k)/math.factorial(k)*p**k*(1-p)**(n-k))
    pmf_a.append(prob)

plt.figure(1)
plt.bar(range(0,n+1),pmf_a,width=0.2)
plt.plot(range(0,n+1),pmf_s,'r.')
plt.xlabel('x')
plt.ylabel('pmf')
plt.legend(['anal','slim'])
plt.show()
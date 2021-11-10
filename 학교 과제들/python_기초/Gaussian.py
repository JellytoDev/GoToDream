import random
import numpy as np
import matplotlib.pyplot as plt
import math
def pdf_f(x,n):
    max_v = 4
    min_v = -4
    dx = (max_v - min_v)/n
    x_l = [min_v + x*dx for x in range(n+1)]
    p_set = [0]*len(x_l)

    for i in range(len(x)):
        for j in range(n):
            if x_l[j] <= x[i] < x_l[j+1]:
                p_set[j] +=1 / len(x)/dx
                break
    return p_set,x_l


n=20000
m = 0.0
sig = 1.0
x = [random.gauss(m,sig) for _ in range(n)]
pdf_set,x_set = pdf_f(x,30)

pdf_a = []
x_list = np.linspace(-4,4,100)

for x_ in x_list:
    pdf = 1/((2*math.pi)**0.5*sig)*math.exp(-(x_-m)**2/(2*sig**2))
    pdf_a.append(pdf)

plt.figure(1)
plt.plot(x_set[0:(len(x_set)-1)],pdf_set[0:(len(x_set)-1)],'r.')
plt.plot(x_list,pdf_a,'b-')
plt.xlabel('x')
plt.ylabel('pdf')
plt.legend(['sim','analy'])
plt.show()
            

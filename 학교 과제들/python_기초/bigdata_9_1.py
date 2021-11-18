import random
import numpy as np
import matplotlib.pyplot as plt
import math

x=np.linspace(0,4,15)
sigma=1
error=np.random.normal(0,sigma,len(x))
y=1+2*x+error

gauss = lambda x, m : 1/(2*math.pi)**0.5*math.exp(-(x-m)**2/2)
a = np.linspace(2,4,20)
b = np.linspace(1,3,20)
max_v = -1000000

for i in range(len(a)):
    a_h = a[i]
    for k in range(len(b)):
        b_h = b[k]
        likelihood_f = 0
        for j in range(len(x)):
            x_data = x[j]
            m = b_h + a_h*x_data
            likelihood_f += math.log10(gauss(y[j],m))
        if likelihood_f > max_v:
            max_a = a_h
            max_b = b_h
            max_v = likelihood_f
    
print(max_a,max_b)
y_h = max_b+max_a*x

plt.figure(1)
plt.plot(x,y,'b.',label="real data")
plt.plot(x,y_h,'r-',label="estimation")
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.show()

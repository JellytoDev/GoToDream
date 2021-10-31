
# Pr[x <= X <= x=h] = fx(x) -h
# fx(x) = Pr/h

# pdf_f 는 데이터를 분류하고 
#
#
import random
import matplotlib.pyplot as plt

def pdf_f(x,n):
    max_v = x[0]
    min_v = x[0]

    for i in range(len(x)):
        if(max_v < x[i]):
            max_v = i
        if(min_v > x[i]):
            min_v = i
    
    print(max_v,min_v,n)

    dx = (max_v - min_v)/n

    print(dx)

    x_1 = [min_v+0.1+x*dx for x in range(n)]
    p_set = [0]*len(x_1)

    for i in range(len(x)):
        for j in range(len(x_1)-1):
            if x_1[j] <= x[i] <x_1[j+1]:
                p_set[j]+=1/len(x)/dx
                break
    
    return p_set,x_1

n=20000
a = 5
b = 3
x = [(a-b) * random.random() + b for _ in range(n)]

print(x)

pdf_set,x_set = pdf_f(x,20)
pdf_set_a =[0.5]*len(x_set)

print(pdf_set)

plt.figure(1)
plt.plot(x_set[0:len(x_set)-1],pdf_set[0:len(x_set)-1],'r.')
plt.plot(x_set[0:len(x_set)-1],pdf_set_a[0:len(x_set)-1],'b-')
plt.xlim([b-0.1,a])
plt.ylim([0,0.8])
plt.legend(['sim','analy'])
plt.show()
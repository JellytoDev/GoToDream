
# 5000번 횟수만큼 반복 -> 1과 0 성공 횟수를 나눠서

import matplotlib.pyplot as plt
import random

total_try = 5000
pmf_bar_s = [0]*2

for _ in range(total_try):
    i = random.randint(0,1)
    if i==0:
        pmf_bar_s[0] += 1
    else:
        pmf_bar_s[1] += 1

pmf_bar_s[0] = pmf_bar_s[0]/total_try    
pmf_bar_s[1] = pmf_bar_s[1]/total_try    


pmf_bar_a = [0.5,0.5]

plt.bar(range(0,2),pmf_bar_s,width=0.2)
plt.plot(range(0,2),pmf_bar_a,'r.')
plt.legend(["sim","analy"])
plt.show()
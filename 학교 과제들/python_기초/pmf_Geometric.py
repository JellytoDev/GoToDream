# pmf 함수는 첫번째 성공의 시도 횟수별로 확률을 분류하는 작업을 한다.
# main 문에서는 total_try 만큼의 시도 횟수 데이터를 수집한다.
# 3번째 문에서는 공식을 이용하여 pmf를 찾아낸다. (시도횟수별 성공 확률)

# pmf_a => 우리가 임상으로 했을때 공식 활용해서 알아낸 확률(pmf)
# pmf_s => 실제 데이터를 가지고 분석했을 때 알아낸 확률(pmf)

import matplotlib.pyplot as plt
import random

def pmf(_list):
    n_x = [0]*len(range(1,11))
    for i in range(1,11):
        for j in _list:
            if(i==j):
                n_x[i-1] += 1/len(_list)
    return n_x

suc = []

total_try = 5000

for _ in range(total_try):
    i = random.randint(0,2)
    n = 1
    while(i!=1):
        n+=1
        i = random.randint(0,2)
    suc.append(n)

pmf_s = pmf(suc)

pmf_a = []

for k in range(1,11):
    prob=(2/3)**(k-1)*(1/3)
    pmf_a.append(prob)

print(len(pmf_s))

plt.bar(range(1,11),pmf_s)
plt.plot(range(1,11),pmf_a,'r.')
plt.legend(["analy","sim"])
plt.show()
import numpy as np

x = [1,2,3,4,5]
y = [2,4,3,1,1]

np.corrcoef(x, y)

np.corrcoef(x, y)[0, 1]

cov = np.cov(x, y)[0, 1]  # 공분산
xsd = np.std(x, ddof=1)   # x의 표본표준편차
ysd = np.std(y, ddof=1)   # y의 표본표준편차

cov / ( xsd * ysd )

print(cov)
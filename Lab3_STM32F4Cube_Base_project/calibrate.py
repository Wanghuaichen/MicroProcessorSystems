from sklearn import linear_model
import csv

X = [[1,2,3], [4,5,6], [7,8,9]]
y = [[1,0,0], [0,1,0], [0,0,1]]
model = linear_model.LinearRegression()

model.fit(X, y)
print model.coef_
print model.intercept_
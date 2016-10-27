from sklearn import linear_model
import csv

X = []
y = [] 

with open('accelcal.csv', 'rb') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        x_thing = []
        y_thing = []
        print ', '.join(row)
        x_thing.append(row[0])
        y_thing.append(row[1])
        x_thing.append(row[3])
        y_thing.append(row[4])
        x_thing.append(row[5])
        y_thing.append(row[6])
        X.append(x_thing)
        Y.append(y_thing)
        

#X = [[1,2,3], [4,5,6], [7,8,9]]
#y = [[1,0,0], [0,1,0], [0,0,1]]
model = linear_model.LinearRegression()

model.fit(X, y)
print model.coef_
print model.intercept_

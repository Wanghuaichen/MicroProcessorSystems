from sklearn import linear_model
import csv

X = []
Y = [] 

with open('accelcal.csv', 'r') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        x_thing = []
        y_thing = []
        print (', '.join(row))
        x_thing.append(float(row[0]))
        y_thing.append(float(row[1]))
        x_thing.append(float(row[2]))
        y_thing.append(float(row[3]))
        x_thing.append(float(row[4]))
        y_thing.append(float(row[5]))
        X.append(x_thing)
        Y.append(y_thing)
        

X = [[1,2,3], [4,5,6], [7,8,9]]
y = [[1,0,0], [0,1,0], [0,0,1]]
model = linear_model.LinearRegression()

model.fit(X, y)
print (model.coef_)
print (model.intercept_)

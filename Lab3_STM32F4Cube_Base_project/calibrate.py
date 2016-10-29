from sklearn import linear_model
import csv

X = []
y = [] 

with open('accelcal.csv', 'r') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
    for row in spamreader:
        x_thing = []
        y_thing = []
        print (', '.join(row))
        x_thing.append(float(row[0]))
        x_thing.append(float(row[1]))
        x_thing.append(float(row[2]))
        y_thing.append(float(row[3]))
        y_thing.append(float(row[4]))
        y_thing.append(float(row[5]))
        X.append(x_thing)
        y.append(y_thing)
        
model = linear_model.LinearRegression()

model.fit(X, y)
print (model.coef_)
print (model.intercept_)

# SMCC
**A simple compiler.**

## Try your first sm program

```
$ export GLOG_minloglevel=3
$ export PATH=$PATH:$PWD/build
```
```
$ cat test/add.sm
x = [0.74537051, -0.57792566,  0.24190366, -0.59080913,  0.38430951,
      0.69148547,  0.05937001, -0.05682391, -0.52289163,  0.92410055]
y = [-0.11405156, -0.53349643,  0.90432695,  0.40619441, -0.74228098,
      -0.29065844,  0.95552332, -0.91361309, -0.82101839, -0.80439142]

size = 10
sum = 0

i = 0
while i < size:
  sub = x[i] - y[i]
  sum = sum + sub * sub
  i = i + 1

mse = sum / size
print("MSE:", mse)
```
```
$ smcc test/add.sm
MSE: 0.902096 
```
```
$ python test/add.sm 
MSE: 0.9020956226268566
```
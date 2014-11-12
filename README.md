# Random Forest Prediction
It is light random forest for prediction. The implementation is exact as the [Piotr Dollar's](https://github.com/pdollar/toolbox). In this directory include two files:

- forest.hpp: does random forest prediction (label and probabilities)
- convertPiotrRF2Cpp.m: convert [Piotr Dollar's](https://github.com/pdollar/toolbox) random forest model into *.txt so that the previous C++ file could read it.

This implementation requires much lower amoung memory for random forest model, does not like OpenCV version. This version requires x100 lesser than OpenCV

# How to use

From Matlab:
````sh
X = [randn(100,2); randn(100,2)];
y = [ones(100,1); 2*ones(100,1)]; % just 2 labels (1 and 2)
model = forestTrain(X,y,'M', 20); % here, trains a random forest of 20 trees
convertPiotrRF3Cpp(model, 'model.txt'); % here, does conversion
````

From C++:
````sh
#include "forest.hpp"

...
Forest forest;
forest.read("model.txt"); // read random forest model

float X[2] = {0.1, 0.2};
float *probabilities = new float[2]; // just 2 labels
memset(probabilities, 0, sizeof(float) * 2);

int label = forest.predict(X, probabilities);
// probabilities[0] is probability of label 1
// probabilities[1] is probability of label 2

...
````


[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/lolongcovas/random-forest-prediction/trend.png)](https://bitdeli.com/free "Bitdeli Badge")


# Fake-neural-network
A very simple neural network made with C++.  
It can be used to classify handwritten numbers.  
I'm not sure if this is truly a neural network, but it can really work.  
In addition, I used MNIST dataset to train the network.

## How to use
1. Clone the repository.  
```git clone https://github.com/MagainBoss/Fake-neural-network.git```  
2. Install the required libraries.  
```pip install numpy tensorflow```  
3. Preprocess dataset.  
```python data.py```  
4. Compile the code.  
```g++ main.cpp -o main.exe -O3```  
5. Run the code.  
```main.exe```  

## Parameters
In file `para.txt`, the four lines are the number of `training data`, number of `testing data`, `epoches` and `learning rate`.  
You can modify the model size in file `main.cpp` by changging the value of `size` variable.  

## Pretrained models
I put two Pretrained models in the `models` folder. These two models each have one hidden layer with 500 neurons and a learning rate of 0.01.
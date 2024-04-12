#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <cstring>
#include <ctime>

double learning_rate = 0.01;
int epoch = 10;
int data_size = 0;
int test_size = 0;

inline double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

inline double sigmoid_derivative(double x)
{
    return x * (1.0 - x);
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-0.5, 0.5);

const int size_x = 28;
const int size_y = 28;

const int size = 500;
int result;
short input[size_x * size_y];
double weights1[size][size_x * size_y];
double hidden[size];
double weights2[10][size];
double output[10];
double hidden_error[size];
double output_error[10];

void init_weights()
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size_x * size_y; j++)
            weights1[i][j] = dis(gen);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < size; j++)
            weights2[i][j] = dis(gen);
}

void init_input(int count, bool test = false)
{
    char data_file[35];
    sprintf(data_file, "./data/data%d.txt", count);
    if (test)
        sprintf(data_file, "./data/data%d_test.txt", count);
    std::ifstream dat(data_file);
    dat >> result;
    dat.get();
    for (int i = 0; i < size_x; i++)
    {
        for (int j = 0; j < size_y; j++)
        {
            char c = dat.get();
            input[i * size_x + j] = (c == '*') ? 1 : 0;
        }
        dat.get();
    }
    dat.close();
}

void save_weights(int ep)
{
    std::ofstream out("weights.txt");
    out << ep << std::endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size_x * size_y; j++)
            out << weights1[i][j] << " ";
        out << std::endl;
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < size; j++)
            out << weights2[i][j] << " ";
        out << std::endl;
    }
    out.close();
}

int load_weights()
{
    std::ifstream in("weights.txt");
    int ep;
    in >> ep;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size_x * size_y; j++)
            in >> weights1[i][j];
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < size; j++)
            in >> weights2[i][j];
    }
    in.close();
    return ep;
}

void train()
{
    int i = 0;
    std::ifstream weights("weights.txt");
    if (weights)
        i = load_weights() + 1;
    else
        init_weights();
    weights.close();
    for (; i < epoch; i++)
    {
        int start_time = std::time(0);
        std::cout << "Epoch: " << i + 1 << std::endl;
        for (int j = 0; j < data_size; j++)
        {
            init_input(j + 1);

            for (int n = 0; n < size; n++)
            {
                for (int m = 0; m < size_x * size_y; m++)
                    hidden[n] += weights1[n][m] * input[m];
                hidden[n] = sigmoid(hidden[n]);
            }
            for (int n = 0; n < 10; n++)
            {
                for (int m = 0; m < size; m++)
                    output[n] += weights2[n][m] * hidden[m];
                output[n] = sigmoid(output[n]);
            }
            for (int n = 0; n < 10; n++)
                output_error[n] = output[n] - ((n == result - 1) ? 1 : 0);
            for (int n = 0; n < size; n++)
            {
                hidden_error[n] = 0;
                for (int m = 0; m < 10; m++)
                    hidden_error[n] += weights2[m][n] * output_error[m];
                hidden_error[n] *= sigmoid_derivative(hidden[n]);
            }
            for (int n = 0; n < 10; n++)
            {
                for (int m = 0; m < size; m++)
                    weights2[n][m] -= learning_rate * output_error[n] * hidden[m];
            }
            for (int n = 0; n < size; n++)
            {
                for (int m = 0; m < size_x * size_y; m++)
                    weights1[n][m] -= learning_rate * hidden_error[n] * input[m];
            }

            memset(hidden, sizeof(hidden), 0);
            memset(output, sizeof(output), 0);
            memset(hidden_error, sizeof(hidden_error), 0);
            memset(output_error, sizeof(output_error), 0);
        }
        save_weights(i);
        std::cout << "Time used: " << std::time(0) - start_time << "s" << std::endl;
    }
    std::cout << "Training finished." << std::endl;
}

void test()
{
    std::cout << "Testing..." << std::endl;
    int correct = 0;
    int start_time = std::time(0);
    for (int i = 0; i < test_size; i++)
    {
        init_input(i + 1, true);

        for (int n = 0; n < size; n++)
        {
            for (int m = 0; m < size_x * size_y; m++)
                hidden[n] += weights1[n][m] * input[m];
            hidden[n] = sigmoid(hidden[n]);
        }
        for (int n = 0; n < 10; n++)
        {

            for (int m = 0; m < size; m++)
                output[n] += weights2[n][m] * hidden[m];
            output[n] = sigmoid(output[n]);
        }
        int max = 0;
        for (int n = 1; n < 10; n++)
            if (output[n] > output[max])
                max = n;
        if (max == result - 1)
            correct++;

        memset(hidden, sizeof(hidden), 0);
        memset(output, sizeof(output), 0);
    }
    std::cout << "Accuracy: " << correct / (double)test_size * 100 << "%" << std::endl;
    std::cout << "Time used: " << std::time(0) - start_time << "s" << std::endl;
}

int main()
{
    std::ifstream para("./para.txt");
    para >> data_size >> test_size >> epoch >> learning_rate;
    std::cout << "data_size: " << data_size << " test_size: " << test_size << " epoch: " << epoch << " learning_rate: " << learning_rate << " model_size: " << size << std::endl;
    para.close();
    train();
    test();
    return 0;
}
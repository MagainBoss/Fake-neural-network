#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <cstring>
#include <ctime>
#include <vector>
#include <future>
#include <thread>
#include <queue>

double learning_rate = 0.001;
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

// 矩阵乘法 因矩阵B始终为一列，因此进行了简化
std::vector<std::vector<double>> matrixMultiply(const std::vector<std::vector<double>> &matA, const std::vector<std::vector<double>> &matB, int startRow, int endRow)
{
    int colsA = matA[0].size();
    std::vector<std::vector<double>> result(endRow - startRow, std::vector<double>(1, 0));

    for (int i = startRow; i < endRow; i++)
    {
        for (int j = 0; j < colsA; j++)
        {
            result[i - startRow][0] += matA[i][j] * matB[j][0];
        }
    }
    return result;
}

int numThreads = std::thread::hardware_concurrency();
const std::vector<std::vector<double>> matrixCalculate(const std::vector<std::vector<double>> &matA, const std::vector<std::vector<double>> &matB)
{
    int numRowsA = matA.size();
    int numColsA = matA[0].size();

    std::vector<std::vector<double>> result(numRowsA, std::vector<double>(1, 0));

    std::queue<std::future<std::vector<std::vector<double>>>> futures;

    int rowsPerThread = numRowsA / numThreads;
    int remainingRows = numRowsA % numThreads;
    int realThreads = numThreads;
    if (rowsPerThread == 0)
    {
        realThreads = remainingRows;
        remainingRows = 0;
        rowsPerThread = 1; // 任务太少，每个线程只负责一个任务
    }
    else
        realThreads = numRowsA / rowsPerThread;
    for (int i = 0; i < realThreads; i++)
    {
        int startRow = i * rowsPerThread;
        int endRow = i * rowsPerThread + rowsPerThread + (i == (realThreads - 1) ? remainingRows : 0);

        futures.push(std::async(std::launch::async, matrixMultiply, std::cref(matA), std::cref(matB), startRow, endRow));
    }

    for (int i = 0; futures.empty() == false; i++)
    {
        std::vector<std::vector<double>> subResult = futures.front().get();
        for (int j = 0; j < subResult.size(); j++)
            result[j + i * rowsPerThread][0] = subResult[j][0];
        futures.pop();
    }
    return result;
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-0.5, 0.5);

const int size_x = 28;
const int size_y = 28;

int size = 500;
int result;
std::vector<std::vector<double>> input(size_x *size_y, std::vector<double>(1, 0));
std::vector<std::vector<double>> weights1(size, std::vector<double>(size_x *size_y, 0));
std::vector<std::vector<double>> hidden(size, std::vector<double>(1, 0));
std::vector<std::vector<double>> weights2(10, std::vector<double>(size, 0));
std::vector<std::vector<double>> output(10, std::vector<double>(1, 0));
std::vector<double> hidden_error(size, 0);
std::vector<double> output_error(10, 0);

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
            input[i * size_x + j][0] = (c == '*') ? 1 : 0;
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
        i = load_weights();
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
            hidden = matrixCalculate(std::cref(weights1), std::cref(input));
            for (int n = 0; n < size; n++)
                hidden[n][0] = sigmoid(hidden[n][0]);
            output = matrixCalculate(std::cref(weights2), std::cref(hidden));
            for (int n = 0; n < 10; n++)
                output[n][0] = sigmoid(output[n][0]);
            for (int n = 0; n < 10; n++)
                output_error[n] = output[n][0] - ((n == result) ? 1 : 0);
            for (int n = 0; n < size; n++)
            {
                hidden_error[n] = 0;
                for (int m = 0; m < 10; m++)
                    hidden_error[n] += weights2[m][n] * output_error[m];
                hidden_error[n] *= sigmoid_derivative(hidden[n][0]);
            }
            for (int n = 0; n < 10; n++)
                for (int m = 0; m < size; m++)
                    weights2[n][m] -= learning_rate * output_error[n] * hidden[m][0];
            for (int n = 0; n < size; n++)
                for (int m = 0; m < size_x * size_y; m++)
                    weights1[n][m] -= learning_rate * hidden_error[n] * input[m][0];

            hidden.assign(size, std::vector<double>(1, 0));
            output.assign(10, std::vector<double>(1, 0));
            hidden_error.assign(size, 0);
            output_error.assign(10, 0);
        }
        save_weights(i + 1);
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

        hidden = matrixCalculate(weights1, input);
        for (int n = 0; n < size; n++)
            hidden[n][0] = sigmoid(hidden[n][0]);
        output = matrixCalculate(weights2, hidden);
        for (int n = 0; n < 10; n++)
            output[n][0] = sigmoid(output[n][0]);
        int max = 0;
        for (int n = 1; n < 10; n++)
            if (output[n] > output[max])
                max = n;
        if (max == result)
            correct++;

        hidden.assign(size, std::vector<double>(1, 0));
        output.assign(10, std::vector<double>(1, 0));
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
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <cstring>
#include <ctime>
#include <vector>
#include <thread>
#include <future>
#include <queue>

double learning_rate = 0.01;
int episodes = 10000;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-0.5, 0.5);
std::uniform_real_distribution<> dis2(-learning_rate, learning_rate);

class Environment_2048
{
private:
    int board_size = 4;
    std::vector<int> board;
    std::vector<bool> empty_pos;
    int score;
    int maxNum = 0;
    int step = 0;

    bool checkFull()
    {
        bool allFull = true;
        for (int i = 0; i < board_size * board_size; i++)
        {
            empty_pos[i] = board[i] > 0 ? false : true;
            maxNum = maxNum > board[i] ? maxNum : board[i];
            allFull &= (!empty_pos[i]);
        }
        return allFull;
    }

    bool canMerge()
    {
        for (int i = 0; i < board_size - 1; i++)
        {
            for (int j = 0; j < board_size - 1; j++)
            {
                if (board[i * board_size + j] == 0)
                    continue;
                if (board[i * board_size + j] == board[i * board_size + j + 1])
                    return true;
                else if (board[i * board_size + j] == board[i * board_size + j + board_size])
                    return true;
            }
        }
        return false;
    }

    void addNumber()
    {
        short pos = 0;
        std::uniform_int_distribution<> dis_int(0, board_size * board_size - 1);
        do
        {
            pos = dis_int(gen);
        } while (empty_pos[pos] == false);
        int num = (dis(gen) > 0) ? 1 : 2;
        board[pos] = num;
    }

public:
    Environment_2048(int size = 4)
    {
        board_size = size;
        board.assign(board_size * board_size, 0);
        empty_pos.assign(board_size * board_size, true);
        score = 0;
        maxNum = 0;
        addNumber();
        addNumber();
    }
    void restart()
    {
        board.assign(board_size * board_size, 0);
        empty_pos.assign(board_size * board_size, true);
        score = 0;
        maxNum = 0;
        step = 0;
        addNumber();
        addNumber();
    }
    void printBoard() const
    {
        for (int i = 0; i < board_size; i++)
        {
            for (int j = 0; j < board_size; j++)
            {
                std::cout << board[i * board_size + j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::vector<int> getBoard() const
    {
        return board;
    }
    int returnScore() const
    {
        return score;
    }
    int returnStep() const
    {
        return step;
    }
    int returnMaxNum()
    {
        return maxNum;
    }
    bool move(int direction)
    {
        // 0: up, 1: down, 2: left, 3: right
        if (direction == 0)
        {
            for (int i = 0; i < board_size; i++)
            {
                bool ifMerge = false;
                do
                {
                    ifMerge = false;
                    for (int j = 0; j < board_size - 1; j++) // 整形
                    {
                        if (board[j * board_size + i] == 0)
                        {
                            int k = j;
                            do
                            {
                                k++;
                            } while (k < board_size && board[k * board_size + i] == 0);
                            if (k < board_size)
                            {
                                board[j * board_size + i] = board[k * board_size + i];
                                board[k * board_size + i] = 0;
                            }
                        }
                    }
                    for (int j = 0; j < board_size - 1; j++) // 合并
                    {
                        if (board[j * board_size + i] != 0 && board[j * board_size + i] == board[j * board_size + i + board_size])
                        {
                            score += board[j * board_size + i];
                            board[j * board_size + i] *= 2;
                            board[j * board_size + i + board_size] = 0;
                            ifMerge = true;
                            break;
                        }
                    }
                } while (ifMerge);
            }
        }
        else if (direction == 1)
        {
            for (int i = 0; i < board_size; i++)
            {
                bool ifMerge = false;
                do
                {
                    ifMerge = false;
                    for (int j = board_size - 1; j > 0; j--) // 整形
                    {
                        if (board[j * board_size + i] == 0)
                        {
                            int k = j;
                            do
                            {
                                k--;
                            } while (k >= 0 && board[k * board_size + i] == 0);
                            if (k >= 0)
                            {
                                board[j * board_size + i] = board[k * board_size + i];
                                board[k * board_size + i] = 0;
                            }
                        }
                    }
                    for (int j = board_size - 1; j > 0; j--) // 合并
                    {
                        if (board[j * board_size + i] != 0 && board[j * board_size + i] == board[j * board_size + i - board_size])
                        {
                            score += board[j * board_size + i];
                            board[j * board_size + i] *= 2;
                            board[j * board_size + i - board_size] = 0;
                            ifMerge = true;
                            break;
                        }
                    }
                } while (ifMerge);
            }
        }
        else if (direction == 2)
        {
            for (int i = 0; i < board_size; i++)
            {
                bool ifMerge = false;
                do
                {
                    ifMerge = false;
                    for (int j = 0; j < board_size - 1; j++) // 整形
                    {
                        if (board[i * board_size + j] == 0)
                        {
                            int k = j;
                            do
                            {
                                k++;
                            } while (k < board_size && board[i * board_size + k] == 0);
                            if (k < board_size)
                            {
                                board[i * board_size + j] = board[i * board_size + k];
                                board[i * board_size + k] = 0;
                            }
                        }
                    }
                    for (int j = 0; j < board_size - 1; j++) // 合并
                    {
                        if (board[i * board_size + j] != 0 && board[i * board_size + j] == board[i * board_size + j + 1])
                        {
                            score += board[i * board_size + j];
                            board[i * board_size + j] *= 2;
                            board[i * board_size + j + 1] = 0;
                            ifMerge = true;
                            break;
                        }
                    }
                } while (ifMerge);
            }
        }
        else
        {
            for (int i = 0; i < board_size; i++)
            {
                bool ifMerge = false;
                do
                {
                    ifMerge = false;
                    for (int j = board_size - 1; j > 0; j--) // 整形
                    {
                        if (board[i * board_size + j] == 0)
                        {
                            int k = j;
                            do
                            {
                                k--;
                            } while (k >= 0 && board[i * board_size + k] == 0);
                            if (k >= 0)
                            {
                                board[i * board_size + j] = board[i * board_size + k];
                                board[i * board_size + k] = 0;
                            }
                        }
                    }
                    for (int j = board_size - 1; j > 0; j--) // 合并
                    {
                        if (board[i * board_size + j] != 0 && board[i * board_size + j] == board[i * board_size + j - 1])
                        {
                            score += board[i * board_size + j];
                            board[i * board_size + j] *= 2;
                            board[i * board_size + j - 1] = 0;
                            ifMerge = true;
                            break;
                        }
                    }
                } while (ifMerge);
            }
        }
        if (checkFull())
        {
            // printBoard();
            std::cout << "Game Over!  Score: " << score << "   MaxNum: " << maxNum << std::endl;

            return true;
        }
        addNumber();
        step++;
        if (checkFull() && (!canMerge()))
        {
            // printBoard();
            std::cout << "Game Over!  Score: " << score << "   MaxNum: " << maxNum << std::endl;

            return true;
        }
        return false;
    }
};

inline double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

std::vector<std::vector<double>> matrixMultiply(const std::vector<std::vector<double>> &matA, const std::vector<std::vector<double>> &matB, int startRow, int endRow)
{
    int colsA = matA[0].size();
    std::vector<std::vector<double>> result(endRow - startRow, std::vector<double>(1, 0));

    for (int i = startRow; i < endRow; ++i)
        for (int j = 0; j < colsA; ++j)
            result[i - startRow][0] += matA[i][j] * matB[j][0];
    return result;
}

int numThreads = std::thread::hardware_concurrency();
const std::vector<std::vector<double>> matrixCalculate(const std::vector<std::vector<double>> &matA, const std::vector<std::vector<double>> &matB)
{
    int numRowsA = matA.size();

    std::vector<std::vector<double>> result(numRowsA, std::vector<double>(1, 0));

    std::queue<std::future<std::vector<std::vector<double>>>> futures;

    int rowsPerThread = numRowsA / numThreads;
    int remainingRows = numRowsA % numThreads;
    int realThreads = numThreads;
    if (rowsPerThread == 0)
    {
        realThreads = remainingRows;
        remainingRows = 0;
        rowsPerThread = 1;
    }
    else
        realThreads = numRowsA / rowsPerThread;
    for (int i = 0; i < realThreads; ++i)
    {
        int startRow = i * rowsPerThread;
        int endRow = i * rowsPerThread + rowsPerThread + (i == (realThreads - 1) ? remainingRows : 0);

        futures.push(std::async(std::launch::async, matrixMultiply, std::cref(matA), std::cref(matB), startRow, endRow));
    }

    for (int i = 0; futures.empty() == false; ++i)
    {
        std::vector<std::vector<double>> subResult = futures.front().get();
        for (int j = 0; j < subResult.size(); ++j)
            result[j + i * rowsPerThread][0] = subResult[j][0];
        futures.pop();
    }
    return result;
}

int board_size = 4;

int size = 500;
int result;
int input_size = board_size * board_size + 2; // 0~board_size*board_size-1:board, input_size-2: step, input_size-1: score;
std::vector<std::vector<double>> input(input_size, std::vector<double>(1, 0));
std::vector<std::vector<double>> hidden1(size, std::vector<double>(1, 0));
std::vector<std::vector<double>> hidden2(size, std::vector<double>(1, 0));
std::vector<std::vector<double>> output;

struct NeurNet
{
    std::vector<std::vector<double>> weights1;
    std::vector<std::vector<double>> weights2;
    std::vector<std::vector<double>> weights3;
} father, son[15];

void init()
{
    father.weights1.assign(size, std::vector<double>(input_size, 0));
    father.weights2.assign(size, std::vector<double>(size, 0));
    father.weights3.resize(4, std::vector<double>(size, 0));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < input_size; j++)
            father.weights1[i][j] = dis(gen);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            father.weights2[i][j] = dis(gen);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < size; j++)
            father.weights3[i][j] = dis(gen);
    for (int t = 0; t < 15; t++)
    {
        son[t].weights1.assign(size, std::vector<double>(input_size, 0));
        son[t].weights2.assign(size, std::vector<double>(size, 0));
        son[t].weights3.assign(4, std::vector<double>(size, 0));
    }
}

void save_weights(int ep)
{
    std::ofstream out("weights.txt");
    out << ep << std::endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < input_size; j++)
            out << father.weights1[i][j] << " ";
        out << std::endl;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            out << father.weights2[i][j] << " ";
        out << std::endl;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < size; j++)
            out << father.weights3[i][j] << " ";
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
        for (int j = 0; j < input_size; j++)
            in >> father.weights1[i][j];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            in >> father.weights2[i][j];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < size; j++)
            in >> father.weights3[i][j];
    in.close();
    return ep;
}

void transmit()
{
    for (int t = 0; t < 10; t++)
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < input_size; j++)
                son[t].weights1[i][j] = father.weights1[i][j] + dis2(gen);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                son[t].weights2[i][j] = father.weights2[i][j] + dis2(gen);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < size; j++)
                son[t].weights3[i][j] = father.weights3[i][j] + dis2(gen);
    }
    for (int t = 10; t < 13; t++)
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < input_size; j++)
                son[t].weights1[i][j] = father.weights1[i][j] + dis2(gen) * 10;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                son[t].weights2[i][j] = father.weights2[i][j] + dis2(gen) * 10;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < size; j++)
                son[t].weights3[i][j] = father.weights3[i][j] + dis2(gen) * 10;
    }
    for (int t = 13; t < 15; t++)
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < input_size; j++)
                son[t].weights1[i][j] = father.weights1[i][j] + dis2(gen) * 100;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                son[t].weights2[i][j] = father.weights2[i][j] + dis2(gen) * 100;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < size; j++)
                son[t].weights3[i][j] = father.weights3[i][j] + dis2(gen) * 100;
    }
}

int evaluate(const NeurNet &model)
{
    Environment_2048 env;
    std::vector<int> board = env.getBoard();
    bool end = false;
    while (!end)
    {
        board = env.getBoard();
        for (int n = 0; n < board_size; n++)
            for (int m = 0; m < board_size; m++)
                input[n * board_size + m][0] = sigmoid(board[n * board_size + m]);
        input[input_size - 2][0] = sigmoid(env.returnStep());
        input[input_size - 1][0] = sigmoid(env.returnScore());
        hidden1.assign(size, std::vector<double>(1, 0));
        hidden2.assign(size, std::vector<double>(1, 0));
        output.assign(4, std::vector<double>(1, 0));
        hidden1 = matrixCalculate(std::cref(model.weights1), std::cref(input));
        for (int n = 0; n < size; n++)
            hidden1[n][0] = sigmoid(hidden1[n][0]);
        hidden2 = matrixCalculate(std::cref(model.weights2), std::cref(hidden1));
        for (int n = 0; n < size; n++)
            hidden2[n][0] = sigmoid(hidden2[n][0]);
        output = matrixCalculate(std::cref(model.weights3), std::cref(hidden2));
        for (int n = 0; n < 4; n++)
            output[n][0] = sigmoid(output[n][0]);
        int choice = 0;
        for (int n = 1; n < 4; n++)
            choice = output[n][0] > output[choice][0] ? n : choice;
        end = env.move(choice);
    }
    return env.returnScore();
}

void train()
{
    init();
    int i = 0;
    std::ifstream weights("weights.txt");
    if (weights)
        i = load_weights();
    weights.close();
    std::ofstream score_log("score_log.txt", std::ofstream::app);
    for (; i < episodes; i++)
    {
        int start = time(0);
        std::cout << "Episode: " << i + 1 << std::endl;

        int next = -1;
        int maxScore = 0;
        maxScore += evaluate(std::cref(father));
        maxScore += evaluate(std::cref(father));
        maxScore += evaluate(std::cref(father));
        maxScore += evaluate(std::cref(father));
        maxScore /= 4;
        transmit();

        for (int j = 0; j < 15; j++)
        {
            int score = evaluate(std::cref(son[j]));
            score += evaluate(std::cref(son[j]));
            score += evaluate(std::cref(son[j]));
            score += evaluate(std::cref(son[j]));
            score /= 4;
            if (score > maxScore)
            {
                maxScore = score;
                next = j;
            }
        }
        if (next != -1)
        {
            father = son[next];
        }
        std::cout << "MaxScore: " << maxScore << std::endl;
        if (next != -1)
            std::cout << "Selected the next generation: " << next + 1 << std::endl;
        else
            std::cout << "Selected the father generation." << std::endl;

        save_weights(i + 1);
        score_log << i + 1 << " " << maxScore << std::endl;
        std::cout << "Cost: " << time(0) - start << "s" << std::endl
                  << std::endl;
    }
    std::cout << "Training finished." << std::endl;
}

int main()
{
    std::ifstream para("./para.txt");
    std::string tmp;
    para >> tmp >> board_size >> tmp >> episodes >> tmp >> size >> tmp >> learning_rate;
    std::cout << "episodes: " << episodes << " learning_rate: " << learning_rate << " model_size: " << size << std::endl;
    para.close();
    train();
    std::cout << "Evaluating the father generation." << std::endl;
    std::cout << "Score: " << evaluate(std::cref(father)) << std::endl;
    return 0;
}
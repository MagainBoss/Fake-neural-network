#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <cstring>
#include <ctime>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-0.5, 0.5);

class Environment_2048
{
private:
    int board_size = 4;
    std::vector<int> board;
    std::vector<bool> empty_pos;
    int score;
    int maxNum = 0;

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
                {
                    return true;
                }
                else if (board[i * board_size + j] == board[i * board_size + j + board_size])
                {
                    return true;
                }
            }
        }
        return false;
    }

    void addNumber()
    {
        if (checkFull())
            return;
        short pos = 0;
        std::uniform_int_distribution<> dis_int(0, board_size * board_size - 1);
        do
        {
            pos = dis_int(gen);
        } while (empty_pos[pos] == false);
        int num = (dis(gen) > 0) ? 1 : 2;
        board[pos] = num;
    }

    void restart()
    {
        board.assign(board_size * board_size, 0);
        empty_pos.assign(board_size * board_size, true);
        score = 0;
        maxNum = 0;
        addNumber();
        addNumber();
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
    void randomGenerate()
    {
        std::uniform_int_distribution<> dis_int(0, 10);
        do
        {
            board.assign(board_size * board_size, 1);
            for (int i = 0; i < board_size * board_size; i++)
            {
                int num = dis_int(gen);
                while (num > 0)
                {
                    board[i] *= 2;
                    num--;
                }
            }
        } while (!canMerge());
        score = 0;
        maxNum = 0;
    }
    std::vector<int> getBoard() const
    {
        return board;
    }
    int returnScore() const
    {
        return score;
    }
    bool move(int direction)
    {
        if (checkFull() && (!canMerge()))
        {
            printBoard();
            std::cout << "Game Over!  Score: " << score << "   MaxNum: " << maxNum << std::endl;
            restart();

            return true;
        }
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
        addNumber();
        return false;
    }
    int returnMaxNum()
    {
        return maxNum;
    }
} env;

double learning_rate = 0.01;
int episodes = 1000;

inline double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

inline double sigmoid_derivative(double x)
{
    return x * (1.0 - x);
}

int size = 500;
int result;
std::vector<int> input;
std::vector<std::vector<double>> weights1;
std::vector<double> hidden1(size);
std::vector<std::vector<double>> weights2;
std::vector<double> hidden2(size);
std::vector<std::vector<double>> weights3;
double output[4];
std::vector<double> hidden1_error(size);
std::vector<double> hidden2_error(size);
double output_error[4];

void init(int board_size)
{
    input.assign(board_size * board_size, 0);
    weights1.resize(size);
    for (int i = 0; i < size; i++)
    {
        weights1[i].assign(board_size * board_size, 0);
    }
    hidden1.assign(size, 0);
    weights2.resize(size);
    for (int i = 0; i < size; i++)
    {
        weights2[i].assign(size, 0);
    }
    hidden2.assign(size, 0);
    weights3.resize(4);
    for (int i = 0; i < 4; i++)
    {
        weights3[i].assign(size, 0);
    }
    for (int i = 0; i < size; i++)
        for (int j = 0; j < board_size * board_size; j++)
            weights1[i][j] = dis(gen);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            weights2[i][j] = dis(gen);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < size; j++)
            weights3[i][j] = dis(gen);
    hidden1_error.assign(size, 0);
    hidden2_error.assign(size, 0);
}

void save_weights(int ep, int board_size)
{
    std::ofstream out("weights.txt");
    out << ep + 1 << std::endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < board_size * board_size; j++)
            out << weights1[i][j] << " ";
        out << std::endl;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            out << weights2[i][j] << " ";
        out << std::endl;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < size; j++)
            out << weights3[i][j] << " ";
        out << std::endl;
    }
    out.close();
}

int load_weights(int board_size)
{
    std::ifstream in("weights.txt");
    int ep;
    in >> ep;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < board_size * board_size; j++)
            in >> weights1[i][j];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            in >> weights2[i][j];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < size; j++)
            in >> weights3[i][j];
    in.close();
    return ep;
}

void test(int board_size)
{
    std::cout << "Testing..." << std::endl;
    Environment_2048 test;
    int start_time = std::time(0);
    bool end = false;
    std::uniform_int_distribution<> dis_int(0, 3);
    while (!end)
    {
        hidden1.assign(size, 0);
        hidden2.assign(size, 0);
        memset(output, sizeof(output), 0);
        std::vector<int> board = test.getBoard();
        test.printBoard();
        for (int n = 0; n < board_size; n++)
        {
            for (int m = 0; m < board_size; m++)
            {
                input[n * board_size + m] = (board[n * board_size + m] == 0) ? 0 : board[n * board_size + m];
            }
        }
        for (int n = 0; n < size; n++)
        {
            for (int m = 0; m < board_size * board_size; m++)
            {
                hidden1[n] += sigmoid(weights1[n][m] * input[m]);
                hidden1[n] = sigmoid(hidden1[n]);
            }
        }
        for (int n = 0; n < size; n++)
        {
            for (int m = 0; m < size; m++)
            {
                hidden2[n] += sigmoid(weights2[n][m] * hidden1[m]);
                hidden2[n] = sigmoid(hidden2[n]);
            }
        }
        for (int n = 0; n < 4; n++)
        {
            for (int m = 0; m < size; m++)
            {
                output[n] += sigmoid(weights2[n][m] * hidden2[m]);
                output[n] = sigmoid(output[n]);
            }
        }
        int choice = 0;
        for (int n = 1; n < 4; n++)
        {
            choice = output[n] > output[choice] ? n : choice;
        }
        if (output[choice] < dis(gen) + 0.5)
            choice = dis_int(gen);
        end = test.move(choice);
    }
    std::cout << "Time used: " << std::time(0) - start_time << "s" << std::endl;
}

void train(int board_size)
{
    init(board_size);
    int i = 1;
    std::ifstream weights("weights.txt");
    if (weights)
        i = load_weights(board_size) + 1;
    weights.close();
    for (; i < episodes; i++)
    {
        double start_time = std::time(0);
        double total_loss = 0;
        if (i % 1000 == 999)
            std::cout << "Episode: " << i + 1 << std::endl;
        env.randomGenerate();
        std::vector<int> board = env.getBoard();
        for (int n = 0; n < board_size; n++)
        {
            for (int m = 0; m < board_size; m++)
            {
                input[n * board_size + m] = (board[n * board_size + m] == 0) ? 0 : board[n * board_size + m];
            }
        }
        for (int n = 0; n < size; n++)
        {
            for (int m = 0; m < board_size * board_size; m++)
            {
                hidden1[n] += sigmoid(weights1[n][m] * input[m]);
                hidden1[n] = sigmoid(hidden1[n]);
            }
        }
        for (int n = 0; n < size; n++)
        {
            for (int m = 0; m < size; m++)
            {
                hidden2[n] += sigmoid(weights2[n][m] * hidden1[m]);
                hidden2[n] = sigmoid(hidden2[n]);
            }
        }
        for (int n = 0; n < 4; n++)
        {
            for (int m = 0; m < size; m++)
            {
                output[n] += sigmoid(weights2[n][m] * hidden2[m]);
                output[n] = sigmoid(output[n]);
            }
        }
        int choice = 0;
        for (int n = 1; n < 4; n++)
        {
            choice = output[n] > output[choice] ? n : choice;
        }
        bool end = env.move(choice);
        int reward = env.returnScore();
        for (int n = 0; n < 4; n++)
        {
            if (n == choice)
                /*if (end)
                    output_error[n] = output[n] - (0.25 - 1.0 / ((double)reward + 8.0) - 1.0 / ((double)env.returnMaxNum() + 8.0));
                else
                    output_error[n] = output[n] - (1.0 - 1.0 / ((double)reward + 8.0 / 3.0) - 1.0 / ((double)env.returnMaxNum() + 8.0 / 3.0));*/
                output_error[n] = output[n] - (reward / double(2 * (reward + 1)));
            /*else if (end)
                output_error[n] = output[n] - (0.25 + 1.0 / ((double)reward + 8.0 / 3.0) + 1.0 / ((double)env.returnMaxNum() + 8.0 / 3.0));*/
            else
                output_error[n] = output[n] - (1 - reward / double(2 * (reward + 1)));
        }
        total_loss = output_error[choice] * output_error[choice];
        for (int n = 0; n < size; n++)
        {
            hidden2_error[n] = 0;
            for (int m = 0; m < 4; m++)
                hidden2_error[n] += weights3[m][n] * output_error[m];
            hidden2_error[n] *= sigmoid_derivative(hidden2[n]);
        }
        for (int n = 0; n < size; n++)
        {
            hidden1_error[n] = 0;
            for (int m = 0; m < size; m++)
                hidden1_error[n] += weights2[m][n] * hidden2_error[m];
            hidden1_error[n] *= sigmoid_derivative(hidden1[n]);
        }
        for (int n = 0; n < 4; n++)
            for (int m = 0; m < size; m++)
                weights3[n][m] -= learning_rate * output_error[n] * hidden2[m];
        for (int n = 0; n < size; n++)
            for (int m = 0; m < size; m++)
                weights2[n][m] -= learning_rate * hidden2_error[n] * hidden1[m];
        for (int n = 0; n < size; n++)
            for (int m = 0; m < board_size * board_size; m++)
                weights1[n][m] -= learning_rate * hidden1_error[n] * input[m];

        hidden1.assign(size, 0);
        hidden2.assign(size, 0);
        memset(output, sizeof(output), 0);
        hidden1_error.assign(size, 0);
        hidden2_error.assign(size, 0);
        memset(output_error, sizeof(output_error), 0);
        if (i % 1000 == 999)
        {
            save_weights(i, board_size);
            std::cout << "Time used: " << (double)std::time(0) - start_time << "s/episode" << std::endl;
            std::cout << "Total loss: " << total_loss << std::endl;
            // test(board_size);
        }
    }
    std::cout << "Training finished." << std::endl;
}

int main()
{
    int board_size = 4;
    std::ifstream para("./para.txt");
    std::string tmp;
    para >> tmp >> board_size >> tmp >> episodes >> tmp >> size >> tmp >> learning_rate;
    std::cout << "episodes: " << episodes << " learning_rate: " << learning_rate << " model_size: " << size << std::endl;
    para.close();
    train(board_size);
    test(board_size);
    return 0;
}
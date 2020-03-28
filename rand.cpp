#include<iostream>
#include<random>
#include<chrono>
#include<cmath>

unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);

int randint(int start, int end)
{
    std::uniform_int_distribution<int> dist(start,end);
    return dist(generator);
}

double randnormal(double value, double dev)
{
    std::normal_distribution<double> dist(value, dev);
    return abs(dist(generator));
}
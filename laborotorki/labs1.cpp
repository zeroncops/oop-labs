#include <iostream>
#include <locale.h>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <random>
#include <climits>

using namespace std;

// генерация случайной матрицы
vector<vector<int>> generateMatrix(int n, int minCost, int maxCost, mt19937_64& gen) {
    uniform_int_distribution<int> dist(minCost, maxCost);
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            matrix[i][j] = (i == j) ? 0 : dist(gen);
    return matrix;
}

struct ExactResult {
    int minCost = INT_MAX;
    int maxCost = INT_MIN;
    vector<int> bestRoute;
    vector<int> worstRoute;
    double timeMs = 0.0;
};

struct HeuristicResult {
    int cost = INT_MAX;
    vector<int> route;
    double timeMs = 0.0;
};

// точный метод - полный перебор
ExactResult solveExact(const vector<vector<int>>& matrix, int startCity) {
    int n = matrix.size();
    vector<int> cities;
    for (int i = 0; i < n; ++i)
        if (i != startCity) cities.push_back(i);

    auto t1 = chrono::high_resolution_clock::now();
    ExactResult res;

    do {
        int cost = 0;
        int cur = startCity;
        vector<int> route;
        route.push_back(startCity);

        for (int nxt : cities) {
            cost += matrix[cur][nxt];
            cur = nxt;
            route.push_back(nxt);
        }
        cost += matrix[cur][startCity];
        route.push_back(startCity);

        if (cost < res.minCost) {
            res.minCost = cost;
            res.bestRoute = route;
        }
        if (cost > res.maxCost) {
            res.maxCost = cost;
            res.worstRoute = route;
        }
    } while (next_permutation(cities.begin(), cities.end()));

    auto t2 = chrono::high_resolution_clock::now();
    res.timeMs = chrono::duration<double, milli>(t2 - t1).count();
    return res;
}


#pragma once
#include <string>
#include <bitset>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>

using namespace std;

class Attacker
{
public:

	///<summary></summary>
	Attacker();

	///<summary></summary>
	~Attacker();

	string BreakMerkleHellmanKnapsack(vector<unsigned long long> pubKey, vector<string> cipherText);

private:
	int originalN = 0;
	int originalM = 0;
	vector<vector<double>> a;

	vector<vector<double>> LLLReduce(vector<vector<double>> matrix);
	vector<vector<double>> DoGramSchmidt(vector<vector<double>> matrix);

	double DotProduct(vector<double> a, vector<double> b);
	vector<double> MultiplyVector(vector<double> a, double b);
	vector<double> SubtractVector(vector<double> a, vector<double> b);
	vector<double> AddVector(vector<double> a, vector<double> b);
	bool IsReducedBasis(vector<vector<double>> matrix, vector<vector<double>> matrix1, vector<vector<double>> currentA);
	bool CheckForZeros(vector<double> v);
	vector<double> GetSolution(vector<vector<double>> matrix, vector<vector<double>> matrix1);
};


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
	Attacker();
	~Attacker();

	string BreakMerkleHellmanKnapsack(vector<unsigned long long> pubKey, vector<string> cipherText);

private:
	int originalN = 0;
	int originalM = 0;
	vector<vector<float>> a;

	vector<vector<float>> LLLReduce(vector<vector<float>> matrix);
	vector<vector<float>> DoGramSchmidt(vector<vector<float>> matrix);

	float DotProduct(vector<float> a, vector<float> b);
	vector<float> MultiplyVector(vector<float> a, float b);
	vector<float> SubtractVector(vector<float> a, vector<float> b);
	vector<float> AddVector(vector<float> a, vector<float> b);
	bool IsReducedBasis(vector<vector<float>> matrix, vector<vector<float>> matrix1, vector<vector<float>> currentA);
	bool CheckForZeros(vector<float> v);
	vector<float> GetSolution(vector<vector<float>> matrix, vector<vector<float>> matrix1);
};


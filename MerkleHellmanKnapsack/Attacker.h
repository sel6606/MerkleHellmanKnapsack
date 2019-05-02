#pragma once
#include <string>
#include <bitset>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>

using namespace std;

///<summary>Class that contains functions for attacking the Merkle-Hellman Knapsack Cryptosystem. Written using https://github.com/wazy/MHKC as reference</summary>
class Attacker
{
public:

	///<summary>Constructor</summary>
	Attacker();

	///<summary>Destructor</summary>
	~Attacker();

	///<summary>Attempts to break the Merkle-Hellman Knapsack system</summary>
	///<param name="pubKey">The public key that was used to encrypt the text</param>
	///<param name="cipherText">The encrypted text</param>
	///<returns>The decrypted text. If the decryption was only partially successful, it will show '?' for unknown characters.</returns>
	string BreakMerkleHellmanKnapsack(vector<unsigned long long> pubKey, vector<string> cipherText);

private:
	int originalN = 0;
	int originalM = 0;
	vector<vector<double>> a;

	///<summary>Performs LLL Reduction on the provided matrix</summary>
	///<param name="matrix>The matrix to reduce</param>
	///<returns>The reduced matrix</returns>
	vector<vector<double>> LLLReduce(vector<vector<double>> matrix);

	///<summary>Function that performs Gram-Schmidt orthogonalization without normalizing</summary>
	///<param name="matrix">The matrix to use</param>
	///<returns>The reduced matrix</returns>
	vector<vector<double>> DoGramSchmidt(vector<vector<double>> matrix);

	///<summary>Function that calculates the dot product of two equally sized vectors</summary>
	///<param name="a">The first vector</param>
	///<param name="b">The second vector</param>
	///<returns>The dot product</returns>
	double DotProduct(vector<double> a, vector<double> b);

	///<summary>Function that multiplies each element of the vector by a scalar value</summary>
	///<param name="a">The vector to scale</param>
	///<param name="b">The scalar</param>
	///<returns>The scaled vector</returns>
	vector<double> MultiplyVector(vector<double> a, double b);
	
	///<summary>Function that calculates the difference of two equally sized vectors</summary>
	///<param name="a">The first vector</param>
	///<param name="b">The second vector</param>
	///<returns>The result of the subtraction operation</returns>
	vector<double> SubtractVector(vector<double> a, vector<double> b);

	///<summary>Function that adds two equally sized vectors together</summary>
	///<param name="a">The first vector</param>
	///<param name="b">The second vector</param>
	///<returns>The result of the addition operation</returns>
	vector<double> AddVector(vector<double> a, vector<double> b);

	///<summary>Determine whether or not we have a reduced basis</summary>
	///<param name="matrix">The original matrix</param>
	///<param name="matrix1">The reduced matrix</param>
	///<param name="currentA">The 'a' value from the most recent Gram-Schmidt process</param>
	///<returns>Whether or not we have a reduced basis</returns>
	bool IsReducedBasis(vector<vector<double>> matrix, vector<vector<double>> matrix1, vector<vector<double>> currentA);
	
	///<summary>Determine if there are any non-zero values in the columns to the right of the original matrix</summary>
	///<param name="v">The row to check</param>
	///<returns>Whether or not there are zeros</returns>
	bool CheckForZeros(vector<double> v);

	///<summary>Attempts to find a solution in the provided basis</summary>
	///<param name="matrix">The original matrix</param>
	///<param name="matrix1">The reduced matrix</param>
	///<returns>The solution. If no solution is found, an empty vector is returned</returns>
	vector<double> GetSolution(vector<vector<double>> matrix, vector<vector<double>> matrix1);
};


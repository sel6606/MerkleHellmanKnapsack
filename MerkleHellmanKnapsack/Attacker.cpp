#include "pch.h"
#include "Attacker.h"


Attacker::Attacker()
{
}


Attacker::~Attacker()
{
}

string Attacker::BreakMerkleHellmanKnapsack(vector<unsigned long long> pubKey, vector<string> cipherText)
{
	originalN = 1;
	originalM = pubKey.size();

	vector<double> cText;

	string retVal = "";

	//Convert the ciphertext back into number values
	for (int i = 0; i < cipherText.size(); i++)
	{
		cText.push_back(stof(cipherText[i]));
	}

	//Run the algorithm for each piece of ciphertext
	for (int iterations = 0; iterations < cText.size(); iterations++)
	{
		int matrixSize = originalM + 1;

		vector<vector<double>> matrix(matrixSize);

		//Set up identity matrix
		for (int i = 0; i < matrixSize; i++)
		{
			vector<double> temp;
			for (int j = 0; j < originalM; j++)
			{
				temp.push_back(0);
			}

			if (i != matrixSize - 1)
			{
				temp[i] = 1;
			}

			matrix[i] = temp;
		}

		//Copy public key into the matrix
		for (int i = 0; i < matrixSize - 1; i++)
		{
			matrix[i].push_back(pubKey[i]);
		}

		//Copy the ciphertext into the matrix
		matrix[matrixSize - 1].push_back(-cText[iterations]);

		vector<vector<double>> matrixCopy = matrix;

		//Reduce the copy of the matrix
		matrixCopy = LLLReduce(matrixCopy);
		vector<double> solution = GetSolution(matrix, matrixCopy);

		
		//If we did not find a solution, attempt to try again with a different basis
		if (solution.size() == 0)
		{
			matrixCopy = matrix;

			double y = sqrt(matrixCopy.size()) * 0.5f;

			for (int i = 0; i < matrixCopy[0].size() - 1; i++)
			{
				matrixCopy[matrixCopy.size() - 1][i] = 0.5f;
			}

			for (int i = 0; i < matrixCopy.size(); i++)
			{
				matrixCopy[i][matrixCopy[0].size() - 1] = matrixCopy[i][matrixCopy[0].size() - 1] * y;
			}

			matrixCopy = LLLReduce(matrixCopy);
			solution = GetSolution(matrix, matrixCopy);
		}


		//Convert the solution (if found) to an ASCII representation
		std::bitset<8> bits;

		if (solution.size() != 0)
		{
			for (int i = 0; i < 8; i++)
			{
				if (solution[i] == 1)
				{
					bits[7 - i] = true;
				}
				else
				{
					bits[7 - i] = false;
				}
			}
			retVal += char(bits.to_ulong());
		}
		else
		{
			//If we were unable to decrypt, represent it with a question mark
			retVal += '?';
		}


	}

	//Return a string representing the decrypted message
	return retVal;
}

vector<vector<double>> Attacker::LLLReduce(vector<vector<double>> matrix)
{
	int mSize = matrix.size();

	//Keep track of the number of iterations
	int count = 0;

	//Initial Gram-Schmidt reduction
	vector<vector<double>> gramSchmidt = DoGramSchmidt(matrix);

	bool done = false;
	bool isReduced = false;

	//Continue doing Gram-Schmidt until we are done reducing or we've gone through too many iterations
	while (!done || count > 2000)
	{
		for (int i = 1; i <= mSize - 1; i++)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (abs(a[j][i]) > 0.5f)
				{
					matrix[i] = SubtractVector(matrix[i], MultiplyVector(matrix[j], floor(a[j][i] + 0.5f)));
				}
			}
		}

		count++;

		gramSchmidt = DoGramSchmidt(matrix);

		if (IsReducedBasis(matrix, gramSchmidt, a))
		{
			done = true;
			isReduced = true;
		}
		else
		{
			gramSchmidt = DoGramSchmidt(matrix);
		}
	}
	return matrix;
}

vector<vector<double>> Attacker::DoGramSchmidt(vector<vector<double>> matrix)
{
	
	int i, j, k;

	int n = matrix.size();

	a.clear();

	for (i = 0; i < n; i++)
	{
		vector<double> temp;
		for (j = 0; j < n; j++)
		{
			temp.push_back(0);
		}

		a.push_back(temp);
	}

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < i; ++j)
		{
			double scalingFactor = round(DotProduct(matrix[j], matrix[i]) / DotProduct(matrix[j], matrix[j]));

			a[j][i] = scalingFactor;

			for (k = 0; k < n; k++)
			{
				matrix[i][k] -= scalingFactor * matrix[j][k];
			}
		}
	}

	return matrix;
}

double Attacker::DotProduct(vector<double> a, vector<double> b)
{
	double sum = 0;

	for (int i = 0; i < a.size(); i++)
	{
		sum += (a[i] * b[i]);
	}
	return sum;
}

vector<double> Attacker::MultiplyVector(vector<double> a, double b)
{
	vector<double> retVal;

	for (int i = 0; i < a.size(); i++)
	{
		retVal.push_back(b * a[i]);
	}
	return retVal;
}

vector<double> Attacker::SubtractVector(vector<double> a, vector<double> b)
{
	vector<double> retVal;

	for (int i = 0; i < a.size(); i++)
	{
		retVal.push_back(a[i] - b[i]);
	}
	return retVal;
}

vector<double> Attacker::AddVector(vector<double> a, vector<double> b)
{
	vector<double> retVal;

	for (int i = 0; i < a.size(); i++)
	{
		retVal.push_back(a[i] + b[i]);
	}
	return retVal;
}

bool Attacker::IsReducedBasis(vector<vector<double>> matrix, vector<vector<double>> matrix1, vector<vector<double>> currentA)
{
	vector<double> temp;

	for (int j = 0; j < matrix.size() - 1; j++)
	{
		temp = AddVector(matrix1[j], MultiplyVector(matrix1[j + 1], currentA[j][j + 1]));

		//Calculate the left hand side of the equation
		double lhs = DotProduct(temp, temp);

		//Calculate the right hand side of the equation
		double rhs = DotProduct(matrix1[j], matrix1[j]) * 0.75f;

		if (lhs < rhs)
		{
			temp = matrix[j];
			matrix[j] = matrix[j + 1];
			matrix[j + 1] = temp;
			return false;
		}
	}

	return true;
}

bool Attacker::CheckForZeros(vector<double> v)
{
	for (int i = originalM; i < v.size(); i++)
	{
		if (v[i] != 0)
		{
			return false;
		}
	}
	return true;
}

vector<double> Attacker::GetSolution(vector<vector<double>> matrix, vector<vector<double>> matrix1)
{
	for (int i = 0; i < matrix1.size(); i++)
	{
		vector<double> vectorToCheck = matrix1[i];
		bool vectorInRangePositive = true;
		bool vectorInRangeNegative = true;

		for (int j = 0; j < originalM; j++)
		{
			double n = vectorToCheck[j];

			if ((n != 0) && (n != 1))
			{
				vectorInRangePositive = false;
				break;
			}
		}

		for (int j = 0; j < originalM; j++)
		{
			double n = vectorToCheck[j];

			if ((n != 0) && (n != -1))
			{
				vectorInRangeNegative = false;
				break;
			}
		}

		if (!CheckForZeros(vectorToCheck))
		{
			vectorInRangePositive = false;
			vectorInRangeNegative = false;
		}

		if (vectorInRangePositive || vectorInRangeNegative)
		{
			return matrix1[i];
		}
	}
	return vector<double>();
}

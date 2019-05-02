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

	vector<float> cText;

	//Convert the ciphertext back into number values
	for (int i = 0; i < cipherText.size(); i++)
	{
		cText.push_back(stof(cipherText[i]));
	}

	int matrixSize = originalM + 1;

	vector<vector<float>> matrix(matrixSize);

	//Set up identity matrix
	for (int i = 0; i < matrixSize; i++)
	{
		vector<float> temp;
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

	matrix[matrixSize - 1].push_back(-cText[0]);

	vector<vector<float>> matrixCopy = matrix;

	//Reduce the copy of the matrix
	matrixCopy = LLLReduce(matrixCopy);
	vector<float> solution = GetSolution(matrix, matrixCopy);


	if (solution.size() == 0)
	{
		matrixCopy = matrix;

		float y = sqrt(matrixCopy.size()) * 0.5f;

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

	
	return string();
}

vector<vector<float>> Attacker::LLLReduce(vector<vector<float>> matrix)
{
	int mSize = matrix.size();

	int count = 0;

	vector<vector<float>> gramSchmidt = DoGramSchmidt(matrix);

	bool done = false;
	bool isReduced = false;

	while (!done)
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

vector<vector<float>> Attacker::DoGramSchmidt(vector<vector<float>> matrix)
{
	int n = matrix.size();
	int m = matrix[0].size();

	vector<vector<float>> matrix1 = matrix;

	a.clear();

	for (int i = 0; i < m; i++)
	{
		vector<float> temp;
		for (int j = 0; j < n; j++)
		{
			temp.push_back(0);
		}

		a.push_back(temp);
	}

	for (int i = 1; i <= n - 1; i++)
	{
		matrix1[i] = matrix[i];
		for (int j = 0; j <= i - 1; j++)
		{
			a[j][i] = (DotProduct(matrix1[j], matrix[i])) / (DotProduct(matrix1[j], matrix1[j]));
			matrix1[i] = SubtractVector(matrix1[i],(MultiplyVector(matrix1[j], a[j][i])));
		}
	}
	return matrix1;
}

float Attacker::DotProduct(vector<float> a, vector<float> b)
{
	float sum = 0;

	for (int i = 0; i < a.size(); i++)
	{
		sum += (a[i] * b[i]);
	}
	return sum;
}

vector<float> Attacker::MultiplyVector(vector<float> a, float b)
{
	vector<float> retVal;

	for (int i = 0; i < a.size(); i++)
	{
		retVal.push_back(b * a[i]);
	}
	return retVal;
}

vector<float> Attacker::SubtractVector(vector<float> a, vector<float> b)
{
	vector<float> retVal;

	for (int i = 0; i < a.size(); i++)
	{
		retVal.push_back(a[i] - b[i]);
	}
	return retVal;
}

vector<float> Attacker::AddVector(vector<float> a, vector<float> b)
{
	vector<float> retVal;

	for (int i = 0; i < a.size(); i++)
	{
		retVal.push_back(a[i] + b[i]);
	}
	return retVal;
}

bool Attacker::IsReducedBasis(vector<vector<float>> matrix, vector<vector<float>> matrix1, vector<vector<float>> currentA)
{
	vector<float> temp;

	for (int j = 0; j < matrix.size() - 1; j++)
	{
		temp = AddVector(matrix1[j], MultiplyVector(matrix1[j + 1], currentA[j][j + 1]));
		float lhs = DotProduct(temp, temp);
		float rhs = DotProduct(matrix1[j], matrix1[j]) * 0.75f;

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

bool Attacker::CheckForZeros(vector<float> v)
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

vector<float> Attacker::GetSolution(vector<vector<float>> matrix, vector<vector<float>> matrix1)
{
	for (int i = 0; i < matrix1.size(); i++)
	{
		vector<float> vectorToCheck = matrix1[i];
		bool vectorInRangePositive = true;
		bool vectorInRangeNegative = true;

		for (int j = 0; j < originalM; j++)
		{
			int n = vectorToCheck[j];

			if ((n != 0) && (n != 1))
			{
				vectorInRangePositive = false;
				break;
			}
		}

		for (int j = 0; j < originalM; j++)
		{
			int n = vectorToCheck[j];

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
	return vector<float>();
}

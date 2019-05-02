#include "pch.h"
#include "Encrypter.h"


Encrypter::Encrypter()
{
}


Encrypter::~Encrypter()
{
}

vector<string> Encrypter::EncryptMessage(string message)
{
	vector<string> retVal;
	vector<unsigned long long> sequence;
	unsigned long long sum = 0;

	//First, generate the private key sequence (w)
	for (int i = 0; i < 8; i++)
	{
		unsigned long long increase = 1;
		unsigned long long newValue = sum + increase;
		sum += newValue;
		sequence.push_back(newValue);
	}

	privateKeyW = sequence;
	sumW = sum;

	//Generate the private Q and R values
	GeneratePrivateKey();

	//Generate the public key values
	GeneratePublicKey();

	//Encrypt each letter of the message individually (due to limitations in how big the numbers can get)
	for(int i = 0; i < message.length(); i++)
	{
		string encryption = message.substr(i,1);

		//Get the binary representation of this character
		string binaryRepresentation = ASCIIToBinary(encryption);

		unsigned long long cipherText = 0;

		//Loop through the binary representation
		for (int j = 0; j < binaryRepresentation.length(); j++)
		{
			//If the binary has a '1' at this location, then add the corresponding public key to the ciphertext
			if (binaryRepresentation[j] == '1')
			{
				cipherText += publicKey[j];
			}
		}

		//Add the ciphertext for this letter to the list
		retVal.push_back(to_string(cipherText));
	}

	//Return all the ciphertext values we generated
	return retVal;
}

string Encrypter::DecryptMessage(vector<string> cipherText)
{
	long long x, y;

	//Calculate the gcd of R and Q using the extended euclidean algorithm
	//x and y will be used later
	unsigned long long g = gcdExtended(privateKeyR, privateKeyQ, &x, &y);

	string retVal;

	//Loop through each ciphertext
	for (int i = 0; i < cipherText.size(); i++)
	{
		//Create a string that is initially all 0's
		string decrypted(privateKeyW.size(), '0');

		//If gcd(R,Q) is not 1, we will not be able to decrypt the message
		if (g == 1)
		{
			//Calculate the inverse using the x value we previously found
			//We add privateKeyQ before doing the modulus in order to deal with potential negative values
			//Because privateKeyQ % privateKeyQ will evaluate to zero, it will not change the outcome of the calculation
			unsigned long long inverse = (x + privateKeyQ) % privateKeyQ;

			//Convert the ciphertext into an unsigned long long
			unsigned long long cText = stoull(cipherText[i]);

			//Calculate the value we will be decomposing
			unsigned long long nextStep = (cText * inverse) % privateKeyQ;

			//Continue until the decomposed value is at 0
			while (nextStep != 0)
			{
				//Loop through the private key sequence
				for (int j = 0; j < privateKeyW.size(); j++)
				{
					//We are looking for the largest value of W that is less than or equal to nextStep
					//If we find a value that is greater than nextStep, that means the previous value will be the largest value less than or equal to it
					if (privateKeyW[j] > nextStep)
					{
						//If we are already at the first value, there are no more values less than nextStep
						if (j == 0)
						{
							nextStep = 0;
							break;
						}

						//Set the binary value at this location to 1, then subtract the private key value from next step
						decrypted[j - 1] = '1';
						nextStep = nextStep - privateKeyW[j - 1];
						break;
					}

					//If we got to the last value without finding anything greater than next step, the last value is the greatest value less than next step
					if (j == privateKeyW.size() - 1)
					{
						decrypted[j] = '1';
						nextStep = nextStep - privateKeyW[j];
						break;
					}
				}
			}

			//Add the decrypted character to the full string
			retVal += decrypted;
		}
	}

	return retVal;
}

void Encrypter::GeneratePrivateKey()
{
	//Seed the random number generator
	srand(time(NULL));

	//Get a random private key 'Q' that is bigger than the sum of the elements in the superincreasing sequence
	privateKeyQ = rand() % 5 + sumW;

	//Get a random private key 'R' that is within the range [1,Q)
	privateKeyR = rand() % privateKeyQ;

	//If 'Q' and 'R' are not coprime, keep generating new 'R' values until they are
	while (gcd(privateKeyQ, privateKeyR) != 1)
	{
		privateKeyR = rand() % privateKeyQ;
	}
}

void Encrypter::GeneratePublicKey()
{
	//Multiply each element in the private sequence by R mod Q to generate the public key
	for (int i = 0; i < privateKeyW.size(); i++)
	{
		publicKey.push_back((privateKeyW[i] * privateKeyR) % privateKeyQ);
	}
}

string Encrypter::ASCIIToBinary(string str)
{
	string binary = "";
	int len = str.length();

	//Loop through each letter in the string
	for (int i = 0; i < len; ++i)
	{
		//Each character is represented by 8 bits
		string temp = bitset<8>(str[i]).to_string();

		binary += temp;
	}
	return binary;
}

unsigned long long Encrypter::gcd(unsigned long long a, unsigned long long b)
{
	//If b is 0, then a is the gcd
	if (b == 0)
	{
		return a;
	}

	//Return the gcd of b and a modulus b
	return gcd(b, a % b);
}

long long Encrypter::gcdExtended(long long a, long long b, long long * x, long long * y)
{
	// Base Case 
	if (a == 0)
	{
		*x = 0, *y = 1;
		return b;
	}

	long long x1;
	long long y1;

	//Recursively call gcdExtended
	long long gcd = gcdExtended(b % a, a, &x1, &y1);

	// Update x and y using the results of the recursive call 
	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

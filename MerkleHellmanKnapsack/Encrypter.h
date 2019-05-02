#pragma once
#include <string>
#include <bitset>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>

using namespace std;

///<summary>Class that encrypts and decrypts using the Merkle-Hellman Knapsack system</summary>
class Encrypter
{
public:

	///<summary>Default constructor</summary>
	Encrypter();

	///<summary>Destructor</summary>
	~Encrypter();

	///<summary>Encrypt the specified string using Merkle-Hellman Knapsack encryption</summary>
	///<param name="message">The message to encrypt</param>
	///<returns>A vector of encrypted ciphertext values. Each entry is one character of the message</returns>
	vector<string> EncryptMessage(string message);

	///<summary>Decrypt the provided ciphertext using the Merkle-Hellman knapsack system</summary>
	///<param name="cipherText">A vector of encrypted ciphertext values. Each entry is one character of the message</param>
	///<returns>The decrypted message</returns>
	string DecryptMessage(vector<string> cipherText);

	//The sequence B=(B_1,B_2,...,B_n)
	//where B_i = r*w_i (mod q)
	vector<unsigned long long> publicKey;

private:

	///<summary>Generates private key values for encrypting</summary>
	void GeneratePrivateKey();

	///<summary>Generates the public key using the private key values</summary>
	void GeneratePublicKey();

	///<summary>Convert the specified ASCII text to binary</summary>
	///<param name="str">The text to convert</param>
	///<returns>The binary representation of the string</returns>
	string ASCIIToBinary(string str);

	///<summary>Calculate the gcd of the specified positive integers</summary>
	///<param name="a">The first value</param>
	///<param name="b">The second value</param>
	///<returns>The gcd of the two values</returns>
	unsigned long long gcd(unsigned long long a, unsigned long long b);

	///<summary>Calculate the gcd of the specified integers using the Extended Euclidean Algorithm</summary>
	///<param name="a">The first value</param>
	///<param name="b">The second value</param>
	///<param name="x">A pointer to the location to store the value x such that a * x + b * y = gcd(a,b)</param>
	///<param name="y">A pointer to the location to store value y such that a * x + b * y = gcd(a,b)</param>
	///<returns>GCD(a,b)</returns>
	long long gcdExtended(long long a, long long b, long long * x, long long * y);

	//Superincreasing sequence of nonzero natural numbers
	//The size of the sequence is the number of bits of the message
	vector<unsigned long long> privateKeyW;

	//The sum of all the elements in the private key 'W'
	unsigned long long sumW = 0;

	//Random integer where q > sum of the superincreasing sequence
	unsigned long long privateKeyQ;

	//Random integer such r and q are coprime
	unsigned long long privateKeyR;

	///<summary>Converts the provided vector into a string for printing</summary>
	///<param name="toPrint">The vector to convert</param>
	///<returns>A string representing the values of the vector</returns>
	string PrintVector(vector<unsigned long long> toPrint);

};


// MerkleHellmanKnapsack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Encrypter.h"

int main()
{
    std::cout << "Hello World!\n"; 
	Encrypter * e = new Encrypter();

	string s = e->DecryptMessage(e->EncryptMessage("hello how are you"));

	int i = 0;
}
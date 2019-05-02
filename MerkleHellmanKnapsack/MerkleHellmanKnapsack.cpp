// MerkleHellmanKnapsack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Encrypter.h"
#include "Attacker.h"

int main()
{
    std::cout << "Hello World!\n"; 


	Encrypter * e = new Encrypter();
	Attacker * a = new Attacker();

	vector<string> s = e->EncryptMessage("a");

	a->BreakMerkleHellmanKnapsack(e->publicKey, s);

	string d = e->DecryptMessage(s);

	int i = 0;
}
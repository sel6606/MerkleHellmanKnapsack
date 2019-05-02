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

	//vector<string> s = e->EncryptMessage("hello how are you");

	vector<string> s = e->EncryptMessage("I WAS I THINK WELL EDUCATED FOR THE STANDARD OF THE DAY MY SISTER AND I HAD A GERMAN GOVERNESS A VERY SENTIMENTAL CREATURE SHE TAUGHT US THE LANGUAGE OF FLOWERS A FORGOTTEN STUDY NOWADAYS BUT MOST CHARMING A YELLOW TULIP FOR INSTANCE MEANS HOPELESS LOVE WHILE A CHINA ASTER MEANS I DIE OF JEALOUSY AT YOUR FEET");

	std::cout << "\n\nAttempting to break encryption:\n";
	std::cout << a->BreakMerkleHellmanKnapsack(e->publicKey, s) << "\n";

	string d = e->DecryptMessage(s);

	int i = 0;
}
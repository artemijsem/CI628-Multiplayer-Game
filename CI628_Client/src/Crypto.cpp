#include <iostream>
#include <vector>
#include <set>
#include <string>
#include "Crypto.h"
#include <numeric>


void Crypto::encryptDecrypt(char* data)
{
	

	for (int i = 0; i < strlen(data); i++)
	{
		data[i] = data[i] ^ secret_key;
	}
	
}


void Crypto::setSecretKey(char key)
{
	secret_key = key;
	keyIsSet = true;

	std::cout << std::endl << std::endl << "SECRET KEY: " << secret_key << std::endl << std::endl;
}

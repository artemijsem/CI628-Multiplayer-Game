
/* 
* RSA Algorithm for enxryption implementation
*/
#include <iostream>
#include <vector>
#include <set>
#include <string>

class Crypto {

private:

    char secret_key;
    bool keyIsSet = false;
public:

    void encryptDecrypt(char * data);
    void setSecretKey(char key);
    bool isKeySet() { return keyIsSet; }

};
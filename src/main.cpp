#include <iostream>
#include "BlockingQueue.hpp"
#include "Looper.hpp"
#include "cryptography.hpp"

int main()
{
    CryptoPP::AutoSeededRandomPool rng1;
    CryptoPP::AutoSeededRandomPool rng2;
    CryptoPP::AutoSeededRandomPool rng3;
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey public_key;

    Cryptography::generate_public_private_key(public_key, private_key, rng1);

    std::string message = "In a hole in the ground there lived a hobbit";

    std::string encrypted = Cryptography::asymmetric_encrypt(public_key, message, rng2);

    std::cout << encrypted << std::endl;

    std::string decrypted = Cryptography::asymmetric_decrypt(private_key, encrypted, rng3);

    std::cout << decrypted << std::endl;
    return 0;
}
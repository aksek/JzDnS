#pragma once

#include<string>

#include<cryptopp/pubkey.h>
#include<cryptopp/secblock.h>
#include<cryptopp/osrng.h>
#include<cryptopp/rsa.h>

class Cryptography {
public:
    static void load_public_key(CryptoPP::PublicKey &key, const std::string &file);
    static void load_private_key(CryptoPP::PrivateKey &key, const std::string &file);
    static void save_public_key_file(const CryptoPP::PublicKey &key, const std::string &file);
    static void save_private_key_file(const CryptoPP::PrivateKey &key, const std::string &file);
    static void generate_public_private_key(CryptoPP::RSA::PublicKey &public_key, CryptoPP::RSA::PrivateKey &private_key,CryptoPP::AutoSeededRandomPool &rng);
    static std::string asymmetric_encrypt(const CryptoPP::PublicKey &key, const std::string &message, CryptoPP::AutoSeededRandomPool &rng);
    static std::string asymmetric_decrypt(const CryptoPP::PrivateKey &key, const std::string &message, CryptoPP::AutoSeededRandomPool &rng);
    //static void generate_symmetric_key(CryptoPP::SecByteBlock &key, CryptoPP::SecByteBlock &iv);
    //static std::string symmetric_encrypt(const std::string &message, const CryptoPP::SecByteBlock &key, const CryptoPP::SecByteBlock &iv);
    //static std::string symmetric_decrypt(const std::string &message, const CryptoPP::SecByteBlock &key, const CryptoPP::SecByteBlock &iv);
};

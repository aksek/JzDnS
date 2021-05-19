#pragma once

#include<string>

#include<cryptopp/pubkey.h>

class Cryptography {
    void load_public_key(CryptoPP::PublicKey &key, const std::string &file);
    void load_private_key(CryptoPP::PrivateKey &key, const std::string &file);
public:
    void generate_private_public_key(CryptoPP::PrivateKey &private_key, CryptoPP::PublicKey &public_key);
    void save_public_key_file(const CryptoPP::PublicKey &key, const std::string &file);
    void save_private_key_file(const CryptoPP::PrivateKey &key, const std::string &file);
    std::string asymetric_encrypt(const CryptoPP::PublicKey &key, const std::string &message);
    std::string asymetric_decrypt(const CryptoPP::PrivateKey &key, const std::string &message);
    std::string symetric_encrypt(const std::string &message);
    std::string symetric_decrypt(const std::string &message);
};
#pragma once

#include<string>

#include<cryptopp/pubkey.h>
#include<cryptopp/secblock.h>

class Cryptography {
public:
    void load_public_key(CryptoPP::PublicKey &key, const std::string &file);
    void load_private_key(CryptoPP::PrivateKey &key, const std::string &file);
    void generate_private_public_key(CryptoPP::PrivateKey &private_key, CryptoPP::PublicKey &public_key);
    void save_public_key_file(const CryptoPP::PublicKey &key, const std::string &file);
    void save_private_key_file(const CryptoPP::PrivateKey &key, const std::string &file);
    std::string asymetric_encrypt(const CryptoPP::PublicKey &key, const std::string &message);
    std::string asymetric_decrypt(const CryptoPP::PrivateKey &key, const std::string &message);
    void generate_symetric_key(CryptoPP::SecByteBlock &key);
    std::string symetric_encrypt(const std::string &message, const CryptoPP::SecByteBlock &key, const CryptoPP::SecByteBlock &iv);
    std::string symetric_decrypt(const std::string &message, const CryptoPP::SecByteBlock &key, const CryptoPP::SecByteBlock &iv);
};
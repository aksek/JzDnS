#include<cryptopp/files.h>
#include<cryptopp/hex.h>
#include<cryptopp/filters.h>
#include<cryptopp/osrng.h>
#include<cryptopp/integer.h>
#include<cryptopp/asn.h>
#include<cryptopp/oids.h>
#include<cryptopp/cryptlib.h>
#include<cryptopp/rsa.h>
//#include<cryptopp/chacha.h>

#include<iostream>

#include"cryptography.hpp"

using namespace CryptoPP;

void Cryptography::load_public_key(PublicKey &key, const std::string &file) {
    FileSource source(file.c_str(), true);
    key.Load(source);
}

void Cryptography::load_private_key(PrivateKey &key, const std::string &file) {
    FileSource source(file.c_str(), true);
    key.Load(source);
}

void Cryptography::save_public_key_file(const PublicKey &key, const std::string &file) {
    FileSink sink(file.c_str());
    key.Save(sink);
}

void Cryptography::save_private_key_file(const PrivateKey &key, const std::string &file) {
    FileSink sink(file.c_str());
    key.Save(sink);
}

void Cryptography::generate_public_private_key(RSA::PublicKey &public_key, RSA::PrivateKey &private_key, AutoSeededRandomPool &rng) {
    CryptoPP::InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 3072);

    public_key = CryptoPP::RSA::PublicKey(params);
    private_key = CryptoPP::RSA::PrivateKey(params);
}

std::string Cryptography::asymmetric_encrypt(const PublicKey &key, const std::string &message, AutoSeededRandomPool &rng) {
    RSAES<OAEP<SHA256> >::Encryptor e(key);
    std::string cipher;

    StringSource ss1(message, true,
        new PK_EncryptorFilter(rng, e,
            new StringSink(cipher)
        ) 
    ); 
    return cipher;
}

std::string Cryptography::asymmetric_decrypt(const PrivateKey &key, const std::string &message, AutoSeededRandomPool &rng) {

    RSAES<OAEP<SHA256> >::Decryptor d(key);
    std::string recovered;

    StringSource ss2(message, true,
        new PK_DecryptorFilter(rng, d,
            new StringSink(recovered)
        ) 
    );
    return recovered;
}
/*
void Cryptography::generate_symmetric_key(SecByteBlock &key, SecByteBlock &iv) {
    key = SecByteBlock(32);
    iv = SecByteBlock(8);

    AutoSeededRandomPool rng;
    rng.GenerateBlock(key, key.size());
    rng.GenerateBlock(iv, iv.size());
}

std::string Cryptography::symmetric_encrypt(const std::string &message, const SecByteBlock &key, const SecByteBlock &iv) {
    ChaCha::Encryption enc;
    enc.SetKeyWithIV(key, key.size(), iv, iv.size());

    std::string cipher;
    cipher.resize(message.size());
    enc.ProcessData((byte*)&cipher[0], (const byte*)message.data(), message.size());

    return cipher;
}

std::string Cryptography::symmetric_decrypt(const std::string &message, const SecByteBlock &key, const SecByteBlock &iv) {
    ChaCha::Decryption dec;
    dec.SetKeyWithIV(key, key.size(), iv, iv.size());

    std::string recover;
    recover.resize(message.size());
    dec.ProcessData((byte*)&recover[0], (const byte*)message.data(), message.size());

    return recover;
}
*/

#include<cryptopp/files.h>
#include<cryptopp/hex.h>
#include<cryptopp/filters.h>
#include<cryptopp/osrng.h>
#include<cryptopp/integer.h>
#include<cryptopp/asn.h>
#include<cryptopp/oids.h>
#include<cryptopp/cryptlib.h>
#include<cryptopp/rsa.h>

#include"cryptography.hpp"

using namespace CryptoPP;

void Cryptography::load_public_key(PublicKey &key, const std::string &file) {
    FileSink sink(file.c_str(), true);
    key.Load(sink);
}

void Cryptography::load_private_key(PrivateKey &key, const std::string &file) {
    FileSource source(file.c_str(), true);
    key.Load(source);
}

void Cryptography::generate_private_public_key(CryptoPP::PrivateKey &private_key, CryptoPP::PublicKey &public_key) {
    AutoSeededRandomPool rng;

    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 3072);

    private_key = RSA::PrivateKey(params);
    public_key = RSA::PublicKey(params);
}

void Cryptography::save_public_key_file(const PublicKey &key, const std::string &file) {
    FileSink sink(file.c_str());
    key.Save(sink);
}

void Cryptography::save_private_key_file(const PrivateKey &key, const std::string &file) {
    FileSink sink(file.c_str());
    key.Save(sink);
}

std::string Cryptography::asymetric_encrypt(const PublicKey &key, const std::string &message) {
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Encryptor e(key);
    std::string cipher;

    StringSource ss1(message, true,
        new PK_EncryptorFilter(rng, e,
            new StringSink(cipher)
        ) 
    ); 
    return cipher;
}

std::string Cryptography::asymetric_decrypt(const PrivateKey &key, const std::string &message) {
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Decryptor d(key);
    std::string recovered;

    StringSource ss2(message, true,
        new PK_DecryptorFilter(rng, d,
            new StringSink(recovered)
        ) 
    );
    return recovered;
}

std::string Cryptography::symetric_encrypt(const std::string &message) {

}

std::string Cryptography::symetric_decrypt(const std::string &message) {

}


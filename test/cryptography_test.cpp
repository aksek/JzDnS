#define BOOST_TEST_MODULE cryptography_test
#include <boost/test/included/unit_test.hpp>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

#include "../src/cryptography.hpp"

using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(generate_private_public_test) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey public_key;

    Cryptography::generate_public_private_key(public_key, private_key, rng);

    BOOST_CHECK(private_key.Validate(rng, 3));
    BOOST_CHECK(public_key.Validate(rng, 3));
}

BOOST_AUTO_TEST_CASE(save_private_public_key_test) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey public_key;

    Cryptography::generate_public_private_key(public_key, private_key, rng);

    Cryptography::save_private_key_file(private_key, "./private_key.pem");
    Cryptography::save_public_key_file(public_key, "./public_key.pem");
}

BOOST_AUTO_TEST_CASE(load_private_public_key_test) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey public_key;

    Cryptography::load_private_key(private_key, "./private_key.pem");
    Cryptography::load_public_key(public_key, "./public_key.pem");

    BOOST_CHECK(private_key.Validate(rng, 3));
    BOOST_CHECK(public_key.Validate(rng, 3));
}

BOOST_AUTO_TEST_CASE(encrypt_decrypt_test) {
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey public_key;

    Cryptography::generate_public_private_key(public_key, private_key, rng);

    std::string message = "In a hole in the ground there lived a hobbit";

    std::string encrypted = Cryptography::asymmetric_encrypt(public_key, message, rng);

    BOOST_CHECK(encrypted != message);

    std::string decrypted = Cryptography::asymmetric_decrypt(private_key, encrypted, rng);

    BOOST_CHECK_EQUAL(decrypted, message);
}

BOOST_AUTO_TEST_CASE(generate_symmetric_key) {
    CryptoPP::SecByteBlock key, iv;
    Cryptography::generate_symmetric_key(key, iv);
    BOOST_CHECK(key.SizeInBytes() != 0);
    BOOST_CHECK(iv.SizeInBytes() != 0);
}

BOOST_AUTO_TEST_CASE(symmetric_encryption) {
    CryptoPP::SecByteBlock key, iv;
    Cryptography::generate_symmetric_key(key, iv);

    std::string message = "The story so far:\n"
                          "In the beginning the Universe was created.\n"
                          "This has made a lot of people very angry and been widely regarded as a bad move.";
    std::string encrypted = Cryptography::symmetric_encrypt(message, key, iv);
    BOOST_CHECK(encrypted != message);
    std::string decrypted = Cryptography::symmetric_decrypt(encrypted, key, iv);
    BOOST_CHECK_EQUAL(decrypted, message);
}
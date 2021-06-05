//
// Created by aniela on 6/4/21.
//

#define BOOST_TEST_MODULE looper_test
#include <boost/test/included/unit_test.hpp>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include "../src/authorization.hpp"
#include "../src/UserBase.h"
#include "../src/Looper.hpp"
#include "../src/cryptography.hpp"


using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(login_test) {
    Riddle riddle(0, "why", "because");

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey public_key;
    Cryptography::load_public_key(public_key, "./public_key.pem");

    CryptoPP::RSA::PublicKey user_public_key;
    CryptoPP::RSA::PrivateKey user_private_key;
    Cryptography::generate_public_private_key(user_public_key, user_private_key, rng);


    UserBase userBase;
    userBase.addUser(User("a", User::UserType::NORMAL, user_public_key));

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);

    QueueMap userQueues(&authorization);
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);

    SerializeContent serializer;
    auto content = serializer.serializeString("a");

    std::string encrypted = Cryptography::asymmetric_encrypt(public_key, content.first, rng);

    auto encrypted_content = std::make_pair(encrypted, content.second);

    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");
    looper.getDispatcher()->post(Message(MessageType::Login, "a", encrypted_content));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::OK);

    looper.stop();
}

BOOST_AUTO_TEST_CASE(register_test) {
    Riddle riddle(0, "why", "because");

    UserBase userBase;

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);

    QueueMap userQueues(&authorization);
    BlockingQueue<Message> queue;
    userQueues.add_user("b", &queue);

    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    Message message(MessageType::Login, "b");

    SerializeContent serializer;

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey public_key;

    Cryptography::generate_public_private_key(public_key, private_key, rng);

    auto content = serializer.serializePublicKey(std::make_pair("b", public_key));


    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");

    looper.getDispatcher()->post(Message(MessageType::Register, "b", content));
    Message result = userQueues.pop("b");

    BOOST_CHECK(result.getMessageType() == MessageType::OK);

    looper.stop();
}
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

    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");
    looper.getDispatcher()->post(Message(MessageType::Login, "a", content));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::OK);

    looper.stop();
}

BOOST_AUTO_TEST_CASE(register_test) {
    Riddle riddle(0, "why", "because");

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey public_key;
    Cryptography::load_public_key(public_key, "./public_key.pem");

    CryptoPP::RSA::PublicKey user_public_key;
    CryptoPP::RSA::PrivateKey user_private_key;
    Cryptography::generate_public_private_key(user_public_key, user_private_key, rng);


    UserBase userBase;

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);

    QueueMap userQueues(&authorization);
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);

    SerializeContent serializer;
    auto content = serializer.serializePublicKey(std::make_pair("a", user_public_key));


    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");
    looper.getDispatcher()->post(Message(MessageType::Register, "a", content));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::OK);

    looper.stop();
}

BOOST_AUTO_TEST_CASE(get_riddle_test) {
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

    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");
    looper.getDispatcher()->post(Message(MessageType::Get_current_problem, "a"));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::Problem);
    auto result_content = result.getContent();
    std::string result_deserialized = std::get<std::string>(serializer.deserialize(MessageType::Problem, result_content));
    std::string decrypted = Cryptography::asymmetric_decrypt(user_private_key, result_deserialized, rng);
    BOOST_CHECK_EQUAL(decrypted, "why");
    looper.stop();
}

BOOST_AUTO_TEST_CASE(solution_test) {
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
    userQueues.add_user("b", &queue);

    SerializeContent serializer;
    auto content = serializer.serializeString("because");
    std::string encrypted = Cryptography::asymmetric_encrypt(public_key, content.first, rng);


    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");
    looper.getDispatcher()->post(Message(MessageType::Solution, "a", encrypted, content.second));

    Message result = userQueues.pop("a");
    BOOST_CHECK(result.getMessageType() == MessageType::Correct);

    Message side_result = userQueues.pop("b");
    BOOST_CHECK(result.getMessageType() == MessageType::Round_over);

    looper.stop();
}
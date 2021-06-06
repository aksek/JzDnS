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
    std::string decrypted = Cryptography::asymmetric_decrypt(user_private_key, result_content.first, rng);
    std::string result_deserialized = std::get<std::string>(serializer.deserialize(MessageType::Problem, decrypted, result_content.second));
    BOOST_CHECK_EQUAL(result_deserialized, "why");
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

BOOST_AUTO_TEST_CASE(add_new_problem) {
    Riddle riddle(0, "why", "because");

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey public_key;
    Cryptography::load_public_key(public_key, "./public_key.pem");

    CryptoPP::RSA::PublicKey user_public_key;
    CryptoPP::RSA::PrivateKey user_private_key;
    Cryptography::generate_public_private_key(user_public_key, user_private_key, rng);

    UserBase userBase;
    userBase.addUser(User("a", User::UserType::ADMIN, user_public_key));

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);

    QueueMap userQueues(&authorization);
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);

    Riddle r(1, "cosik", "cosik2");

    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");

    SerializeContent serializer;
    auto content = serializer.serializePairStringString(std::pair<std::string, std::string>("cosik", "cosik2"));
    std::string crypted = Cryptography::asymmetric_encrypt(public_key, content.first, rng);
    content.first = crypted;
    looper.getDispatcher()->post(Message(MessageType::New_problem, "a", content));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::OK);
    auto result_content = result.getContent();
    std::string result_deserialized = std::get<std::string>(serializer.deserialize(MessageType::OK, result_content));
    std::string decrypted = Cryptography::asymmetric_decrypt(user_private_key, result_deserialized, rng);
    BOOST_CHECK_EQUAL(decrypted, "1");
    looper.stop();
}

BOOST_AUTO_TEST_CASE(get_all_problems) {
    Riddle riddle(0, "why", "because");
    Riddle riddle1(0, "cosik", "cosik4");
    Riddle riddle2(0, "cosik1", "cosik6");
    Riddle riddle3(0, "cosik12", "cosik81");
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey public_key;
    Cryptography::load_public_key(public_key, "./public_key.pem");

    CryptoPP::RSA::PublicKey user_public_key;
    CryptoPP::RSA::PrivateKey user_private_key;
    Cryptography::generate_public_private_key(user_public_key, user_private_key, rng);

    UserBase userBase;
    userBase.addUser(User("a", User::UserType::ADMIN, user_public_key));

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    riddleBase.addRiddle(riddle1);
    riddleBase.addRiddle(riddle2);
    riddleBase.addRiddle(riddle3);

    std::vector<std::tuple<int, std::string, std::string>> veryCleverTuple;

    std::map<int, Riddle> veryFunnyMap = riddleBase.getAllRiddles();

    for(auto &it : veryFunnyMap) {
        veryCleverTuple.push_back(make_tuple(it.second.getId(), it.second.getRiddleContent(), it.second.getAnswer()));
    }
    SerializeContent serializer;

    auto veryCleverTupleAfterNightOfCodingTIN = serializer.serializeVector(veryCleverTuple);
    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);

    QueueMap userQueues(&authorization);
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);


    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");


    looper.getDispatcher()->post(Message(MessageType::Get_all_problems, "a"));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::Problems);
    auto result_content = result.getContent();
    std::string decrypted = Cryptography::asymmetric_decrypt(user_private_key, result_content.first, rng);
    result_content.first = decrypted;
    auto result_deserialized = std::get<std::vector<std::tuple<int, std::string, std::string>>>(serializer.deserialize(MessageType::Problems, result_content));

    BOOST_CHECK_EQUAL(decrypted, "\"[{\\\"firstValue\\\":1,\\\"secondValue\\\":\\\"why\\\",\\\"thirdValue\\\":\\\"because\\\"},{\\\"firstValue\\\":2,\\\"secondValue\\\":\\\"cosik\\\",\\\"thirdValue\\\":\\\"cosik4\\\"},{\\\"firstValue\\\":3,\\\"secondValue\\\":\\\"cosik1\\\",\\\"thirdValue\\\":\\\"cosik6\\\"},{\\\"firstValue\\\":4,\\\"secondValue\\\":\\\"cosik12\\\",\\\"thirdValue\\\":\\\"cosik81\\\"}]\"");
    looper.stop();
}

BOOST_AUTO_TEST_CASE(delete_riddle_test) {
    Riddle riddle(0, "why", "because");
    Riddle riddle1(0, "cosik", "cosik4");
    Riddle riddle2(0, "cosik1", "cosik6");
    Riddle riddle3(0, "cosik12", "cosik81");
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey public_key;
    Cryptography::load_public_key(public_key, "./public_key.pem");

    CryptoPP::RSA::PublicKey user_public_key;
    CryptoPP::RSA::PrivateKey user_private_key;
    Cryptography::generate_public_private_key(user_public_key, user_private_key, rng);

    UserBase userBase;
    userBase.addUser(User("a", User::UserType::ADMIN, user_public_key));

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    riddleBase.addRiddle(riddle1);
    riddleBase.addRiddle(riddle2);
    riddleBase.addRiddle(riddle3);
    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);

    QueueMap userQueues(&authorization);
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);


    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");

    SerializeContent serializer;
    auto content = serializer.serializeInt(1);
    std::string crypted = Cryptography::asymmetric_encrypt(public_key, content.first, rng);
    content.first = crypted;
    looper.getDispatcher()->post(Message(MessageType::Delete_problem, "a", content));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::OK);
    auto result_content = result.getContent();
    std::string decrypted = Cryptography::asymmetric_decrypt(user_private_key, result_content.first, rng);
    result_content.first = decrypted;
    auto result_deserialized = std::get<int>(serializer.deserialize(MessageType::OK, result_content));

    BOOST_CHECK_EQUAL(result_deserialized, 1);
    looper.stop();
}

BOOST_AUTO_TEST_CASE(update_riddle) {
    Riddle riddle(0, "why", "because");
    Riddle riddle1(0, "cosik", "cosik4");
    Riddle riddle2(0, "cosik1", "cosik6");
    Riddle riddle3(0, "cosik12", "cosik81");
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey public_key;
    Cryptography::load_public_key(public_key, "./public_key.pem");

    CryptoPP::RSA::PublicKey user_public_key;
    CryptoPP::RSA::PrivateKey user_private_key;
    Cryptography::generate_public_private_key(user_public_key, user_private_key, rng);

    UserBase userBase;
    userBase.addUser(User("a", User::UserType::ADMIN, user_public_key));

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    riddleBase.addRiddle(riddle1);
    riddleBase.addRiddle(riddle2);
    riddleBase.addRiddle(riddle3);
    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);

    QueueMap userQueues(&authorization);
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);


    Looper looper(&userQueues, &authorization, &riddleModule, &adminModule);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");

    SerializeContent serializer;
    auto content = serializer.serializeTuple(std::tuple<int, std::string, std::string>(1, "anna", "harietta"));
    std::string crypted = Cryptography::asymmetric_encrypt(public_key, content.first, rng);
    content.first = crypted;
    looper.getDispatcher()->post(Message(MessageType::Update, "a", content));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::OK);
    auto result_content = result.getContent();
    std::string decrypted = Cryptography::asymmetric_decrypt(user_private_key, result_content.first, rng);
    result_content.first = decrypted;
    auto result_deserialized = std::get<int>(serializer.deserialize(MessageType::OK, result_content));

    BOOST_CHECK_EQUAL(result_deserialized, 1);
    looper.stop();
}
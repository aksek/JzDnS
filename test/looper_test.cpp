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

    QueueMap userQueues;
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);
    UserBase userBase;

    userBase.addUser(User("a", User::UserType::NORMAL));
    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    RiddleService riddleService(&riddleBase);
    AdminService adminService(&riddleBase);

    Looper looper(&userQueues, &authorization, &riddleService, &adminService);
    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
    BOOST_TEST_CHECKPOINT( "Looper running");
    SerializeContent serializer;
    auto content = serializer.serializeString("a");
    looper.getDispatcher()->post(Message(MessageType::Login, "a", content));
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::Login_OK);

    looper.stop();
}

BOOST_AUTO_TEST_CASE(register_test) {
    Riddle riddle(0, "why", "because");

    QueueMap userQueues;
    BlockingQueue<Message> queue;
    userQueues.add_user("b", &queue);
    UserBase userBase;

    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    RiddleService riddleService(&riddleBase);
    AdminService adminService(&riddleBase);

    Looper looper(&userQueues, &authorization, &riddleService, &adminService);
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
    Message result = userQueues.pop("a");

    BOOST_CHECK(result.getMessageType() == MessageType::Login_OK);

    looper.stop();
}
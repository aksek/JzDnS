#include <iostream>
#include "BlockingQueue.hpp"
#include "Looper.hpp"
#include "cryptography.hpp"

int main()
{
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


//    BOOST_TEST_CHECKPOINT( "Looper initiated");
    looper.run();
//    BOOST_TEST_CHECKPOINT( "Looper running");

    looper.getDispatcher()->post(Message(MessageType::Register, "b", content));

    Message result = userQueues.pop("b");

//    BOOST_CHECK(result.getMessageType() == MessageType::Login_OK);

    looper.stop();
    return 0;
}
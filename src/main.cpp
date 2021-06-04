#include <iostream>
#include "BlockingQueue.hpp"
#include "Looper.hpp"

int main()
{
    std::cout << "Hello world!";
    Riddle riddle(0, "why", "because");

    QueueMap userQueues;
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);
    UserBase userBase;
//    CryptoPP::RSA::PublicKey key();
    userBase.addUser(User("a", User::UserType::NORMAL));
    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    riddleBase.addRiddle(riddle);
    RiddleService riddleService(&riddleBase);
    AdminService adminService(&riddleBase);

    Looper looper(&userQueues, &authorization, &riddleService, &adminService);
    looper.run();
    Message message(MessageType::Login, "a");

    SerializeContent serializer;
    auto content = serializer.serializeString("a");
    looper.getDispatcher()->post(Message(MessageType::Login, "a", content));
    Message result = userQueues.pop("a");
    std::cout << "user id of the returned message: " <<  result.getUserID() << std::endl;
    looper.stop();
    return 0;
}
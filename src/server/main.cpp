#include <iostream>
#include "../BlockingQueue.hpp"
#include "Looper.hpp"
#include "../cryptography.hpp"
#include "ServerModule.hpp"

int main()
{
    UserBase userBase;
    RiddleBase riddleBase;
    riddleBase.loadBaseFromDisk("riddleBase");
//    QueueMap userQueues;
    BlockingQueue<Message> queue;

    CryptoPP::RSA::PublicKey admin_public_key;
    Cryptography::load_public_key(admin_public_key, "admin_public_key.pem");
    User admin("Admin", User::UserType::ADMIN, admin_public_key);
    userBase.addUser(admin);
    Authorization authorization(&userBase);
//    userQueues.setAuthorization(&authorization);

    RiddleModule riddleModule(&riddleBase);
    AdminModule adminModule(&riddleBase);
    ServerModule serverModule(&authorization);


    Looper looper(&authorization, &riddleModule, &adminModule, &serverModule);

    looper.run();

    std::cout << "To terminate Server press q ";

    while(getchar() != 'q');

    looper.stop();

    return 0;
}
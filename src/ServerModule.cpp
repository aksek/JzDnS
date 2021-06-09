#include"ServerModule.hpp"

struct Info
{
    int socket;
    sockaddr_in address;
};

bool ServerModule::post(Message &&aMessage) {
    if (not running()) {
        return false;
    }

    try {
        if (user_version[aMessage.getUserID()] == IP_Version::IPv4) {
            mMessagesIPv4.push(aMessage);
        } else if (user_version[aMessage.getUserID()] == IP_Version::IPv6) {
            mMessagesIPv6.push(aMessage);
        }
    } catch (...) {
        return false;
    }
    return true;
}

void ServerModule::ConnectionHandler::handle_connection_receive()
{
    char buffer[BUFFER_SIZE];

    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);

    while(!sM.itsTimeToSayGoodbye.load()) {
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        int ret = select(sockfd + 1, &readfds, NULL, NULL, &tv);

        if(ret > 0) {
            recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);

            Message message((std::string(buffer)));
            std::string username = message.getUserID();

            if(version == IP_Version::IPv4)
            {
                if (!sM.user_address_IPv4.count(username)) {
                    sM.user_address_IPv4.insert(std::make_pair(username, cliaddr));
                    sM.user_version.insert(std::make_pair(message.getUserID(), IP_Version::IPv4));
                }
            }
            else
            {
                if (!sM.user_address_IPv6.count(username)) {
                    sM.user_address_IPv6.insert(std::make_pair(username, cliaddr));
                    sM.user_version.insert(std::make_pair(message.getUserID(), IP_Version::IPv6));
                }
            }
            sM.logger.write("Received message");

            if (!sM.userQueues->isInMap(username)) sM.userQueues->add_user(username, new BlockingQueue<Message>());

                sM.looper->getDispatcher()->post(Message((std::string(buffer))));

                bzero(buffer, sizeof(buffer));

        }
        if(ret == 0)
            continue;

    }
}

void ServerModule::ConnectionHandler::handle_connection_send() {
    char buffer[BUFFER_SIZE];

    while(!sM.itsTimeToSayGoodbye.load()) {
        Message next(MessageType::OK, "");
        if (version == IP_Version::IPv4) {

            if (!sM.mMessagesIPv4.tryWaitAndPop(next, 2000)) {
                sM.logger.write("Timeout");
                continue;
            }
        } else if (version == IP_Version::IPv6) {

            if (!sM.mMessagesIPv6.tryWaitAndPop(next, 2000)) {
                sM.logger.write("Timeout");
                continue;
            }
        }

        if (next.getUserID().empty()) {
            continue;
        }

        //przetwarzanie do kogo mamy wyslac
        sM.logger.write("Sending message: " + next.getMessageTypeString());

        std::string serialized_message = next.serialize();

        sockaddr_in cliaddr;
        if (version == IP_Version::IPv4) {
            cliaddr = sM.user_address_IPv4[next.getUserID()];

        } else if (version == IP_Version::IPv6) {
            cliaddr = sM.user_address_IPv6[next.getUserID()];

        } else continue;

        sendto(sockfd, serialized_message.c_str(), serialized_message.length(),
               MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
               sizeof(cliaddr));
    }
}

ServerModule::ConnectionHandler::ConnectionHandler(ServerModule& s, IP_Version v)
: sM(s)
, version(v) {

}


void ServerModule::ConnectionHandler::joinThreadToSend()
{
    threadToSend.join();
}

void ServerModule::ConnectionHandler::joinThreadToReceive()
{
    threadToReceive.join();
}

ServerModule::~ServerModule() {
    /*for(auto &handler : connectionHandlers)
    {
        handler.get().joinThreadToSend();
        handler.get().joinThreadToReceive();
    }
*/
//    connectionHandlers.clear();
    abortAndJoin();
}

void ServerModule::setLooper(Looper *aLooper) {
    looper = aLooper;
}

bool ServerModule::run() {
    try
    {
//        mThread = std::thread(&ServerModule::runFunc, this);

        handlerForIPv4.run();
        handlerForIPv6.run();

    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ServerModule::running() const {
    return mRunning.load();
}

void ServerModule::stop() {
    abortAndJoin();
}

void ServerModule::abortAndJoin() {
    itsTimeToSayGoodbye.store(true);
    handlerForIPv4.stop();
    handlerForIPv6.stop();
}

void ServerModule::ConnectionHandler::stop() {
    close(sockfd);
    if (threadToReceive.joinable()) {
        threadToReceive.join();
    }
    if (threadToSend.joinable()) {
        threadToSend.join();
    }
}


std::shared_ptr<ServerModule::Dispatcher> ServerModule::getDispatcher() {
    return mDispatcher;
}

ServerModule::Dispatcher::Dispatcher(ServerModule &aServer)
: mAssignedServerModule(aServer){

}

bool ServerModule::Dispatcher::post(Message &&aMessage) {
    return mAssignedServerModule.post(std::move(aMessage));
}
bool ServerModule::Dispatcher::post_to_all(Message &&aMessage) {
    for (auto user_v : mAssignedServerModule.user_version) {
        Message message(aMessage.getMessageType(), user_v.first, aMessage.getContent());
        if (!mAssignedServerModule.post(std::move(message))) return false;
    }
    return true;
}

void ServerModule::ConnectionHandler::run() {

    if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        sM.logger.write("socket creation failed");
        sM.itsTimeToSayGoodbye.store(true);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    //tworzenie adresu
    // servaddr.sin_family = (version == IP_Version::IPv4) ? AF_INET : AF_INET6; // IPv4
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    int very_true = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&very_true,sizeof(int));
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
              sizeof(servaddr)) < 0 )
    {
        sM.logger.write("bind failed");
        sM.logger.write("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        sM.itsTimeToSayGoodbye.store(true);
    }

    threadToSend = std::thread(&ServerModule::ConnectionHandler::handle_connection_send, this);
    threadToReceive = std::thread(&ServerModule::ConnectionHandler::handle_connection_receive, this);
}


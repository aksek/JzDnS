//
// Created by aniela on 5/22/21.
//

#include "authorization.hpp"
#include "cryptography.hpp"
#include "Looper.hpp"

#include <utility>


Authorization::Authorization(UserBase *user_base)
: mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
, mRunning(false)
, mAbortRequested(false)
, mMessages()
, base(user_base)
{
    serializer = new SerializeContent;
}

bool Authorization::authorize(std::string username) {
    User *user = base->getUser(std::move(username));

    if (user == nullptr) {
        return false;
    }

    return true;
}

bool Authorization::authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key) {
    User *user = base->getUser(username);

    if (user == nullptr) {
        if (base->addUser(User(username, User::UserType::NORMAL, public_key)) == 0) {
            return true;
        } else return false;
    }
    return true;
}

CryptoPP::RSA::PublicKey Authorization::getKey(std::string username) {
    User *user = base->getUser(std::move(username));

    if (user == nullptr) {
        return CryptoPP::RSA::PublicKey();
    }

    return user->getPublicKey();
}

void Authorization::handleLogin(ValueContent content, std::string user) {
    if (!std::holds_alternative<std::string>(content)) {
        looper->getDispatcher()->post(Message(MessageType::Retransmit, user));
        return;
    }
    std::string username = std::get<std::string>(content);
    sendResponse(user);
}

void Authorization::sendResponse(std::string &user) {
    int user_type_code = 0;
    User::UserType user_type = base->getUser(user)->getUserType();
    if (authorize(user)) {
        if (user_type == User::ADMIN) {
            user_type_code = 2;
        } else {
            user_type_code = 1;
        }
    }
    auto result = serializer->serializeBool(user_type_code);
    looper->getDispatcher()->post(Message(MessageType::OK, user, result));
}

void Authorization::handleRegister(ValueContent content, std::string user) {
    if (!std::holds_alternative<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content)) {
        looper->getDispatcher()->post(Message(MessageType::Retransmit, user));
        return;
    }
    auto username_key = std::get<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content);

    sendResponse(user);
}


void Authorization::runFunc() {
    mRunning.store(true);

    while(false == mAbortRequested.load()) {
        Message next(MessageType::OK, "");

        if (!mMessages.tryWaitAndPop(next, 2000)) {
//            mRunning.store(false);
            continue;
        }


        MessageType type = next.getMessageType();
        ValueContent content = serializer->deserialize(type, next.getContent());

        switch(type) {
            case MessageType::Login:
                handleLogin(content, next.getUserID());
            case MessageType::Register:
                handleRegister(content, next.getUserID());
            default:
                break;
        }
    }
}

bool Authorization::post(Message &&aMessage) {
    if (not running()) {
        return false;
    }

    try {
        mMessages.push(aMessage);
    } catch (...) {
        return false;
    }
    return true;
}

Authorization::~Authorization() {
    abortAndJoin();
}

bool Authorization::run() {
    try
    {
        mThread = std::thread(&Authorization::runFunc, this);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool Authorization::running() const {
    return mRunning.load();
}

void Authorization::stop() {
    abortAndJoin();
}

void Authorization::abortAndJoin() {
    mAbortRequested.store(true);
    if (mThread.joinable()) {
        mThread.join();
    }
}

std::shared_ptr<Authorization::Dispatcher> Authorization::getDispatcher() {
    return mDispatcher;
}

void Authorization::setLooper(Looper *aLooper) {
    looper = aLooper;
}


Authorization::Dispatcher::Dispatcher(Authorization &aAuthorization) : mAssignedAuthorization(aAuthorization) {}

bool Authorization::Dispatcher::post(Message &&aMessage) {
    return mAssignedAuthorization.post(std::move(aMessage));
}

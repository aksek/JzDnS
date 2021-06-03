//
// Created by aniela on 5/22/21.
//

#include "authorization.hpp"
#include "cryptography.hpp"
#include "Looper.hpp"

#include <utility>


Authorization::Authorization(UserBase *user_base, Looper* looper)
: mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
, mRunning(false)
, mAbortRequested(false)
, mMessages()
, looper(looper)
, base(user_base)
, serializer()
{}

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

void Authorization::handleLogin(ValueContent content) {
    if (!std::holds_alternative<std::string>(content)) {
        looper->getDispatcher()->post(Message(MessageType::Retransmit));
        return;
    }
    std::string username = std::get<std::string>(content);

    if (authorize(username)) {
        looper->getDispatcher()->post(Message(MessageType::Login_OK));

    } else {
        looper->getDispatcher()->post(Message(MessageType::Login_error));
    }
}

void Authorization::handleRegister(ValueContent content) {
    if (!std::holds_alternative<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content)) {
        looper->getDispatcher()->post(Message(MessageType::Retransmit));
        return;
    }
    auto username_key = std::get<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content);
    if (authorize(username_key.first, username_key.second)) {
        looper->getDispatcher()->post(Message(MessageType::Login_OK));

    } else {
        looper->getDispatcher()->post(Message(MessageType::Login_error));
    }
}


void Authorization::runFunc() {
    mRunning.store(true);

    while(false == mAbortRequested.load()) {
        Message next(MessageType::OK);
        mMessages.waitAndPop(next);

        MessageType type = next.getMessageType();
        ValueContent content = serializer->deserialize(type, next.getContent());

        switch(type) {
            case MessageType::Login:
                handleLogin(content);
            case MessageType::Register:
                handleRegister(content);
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


Authorization::Dispatcher::Dispatcher(Authorization &aAuthorization) : mAssignedAuthorization(aAuthorization) {}

bool Authorization::Dispatcher::post(Message &&aMessage) {
    return mAssignedAuthorization.post(std::move(aMessage));
}

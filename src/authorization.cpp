//
// Created by aniela on 5/22/21.
//

#include "authorization.hpp"
#include "cryptography.hpp"

#include <utility>


Authorization::Authorization(UserBase *user_base, SerializeContent *serializer)
: mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
, mRunning(false)
, mAbortRequested(false)
, mMessages()
, base(user_base)
, serializer(serializer)
{}

CryptoPP::RSA::PublicKey Authorization::authorize(std::string username) {
    User *user = base->getUser(std::move(username));

    if (user == nullptr) {
        return CryptoPP::RSA::PublicKey();
    }

    return user->getPublicKey();
}

CryptoPP::RSA::PublicKey Authorization::authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key) {
    User *user = base->getUser(username);

    if (user == nullptr) {
        if (base->addUser(User(username, User::UserType::NORMAL, public_key)) == 0) {
            return public_key;
        } else return CryptoPP::RSA::PublicKey();
    }
    return user->getPublicKey();
}

void Authorization::runFunc() {
    mRunning.store(true);

    while(false == mAbortRequested.load()) {
        Message next(MessageType::OK);
        mMessages.waitAndPop(next);

        MessageType type = next.getMessageType();
        ValueContent content = serializer->deserialize(type, next.getContent());
//TODO fill this in v
        switch(type) {
            case MessageType::Login:
            case MessageType::Register:
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

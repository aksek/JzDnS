//
// Created by aniela on 6/4/21.
//

#define BOOST_TEST_MODULE looper_test
#include <boost/test/included/unit_test.hpp>

#include "../src/authorization.hpp"
#include "../src/UserBase.h"
#include "../src/Looper.hpp"


using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(setup_test) {
    QueueMap userQueues;
    BlockingQueue<Message> queue;
    userQueues.add_user("a", &queue);
    UserBase userBase;
    Authorization authorization(&userBase);
    RiddleBase riddleBase;
    RiddleService riddleService(&riddleBase);
    AdminService adminService(&riddleBase);

    Looper looper(&userQueues, &authorization, &riddleService, &adminService);
    looper.run();
    sleep(2);
    looper.stop();
}
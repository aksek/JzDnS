//
// Created by aniela on 6/9/21.
//

#include <iostream>
#include <thread>
#include <unistd.h>
#include "Admin.h"
#include"BlockingQueueAdmin.hpp"

using namespace std;

int main() {

    Admin a(150, 100000);
    a.run();

    return 0;
}
//
// Created by Spencer Dixon on 6/28/20.
//

#pragma once

#include <SD/Bytes.h>

#define PASS(msg) printf("[ " GREEN "PASS" RESET " ] " msg "\n")

#define TEST_ASSERT(condition, msg) \
    ASSERT(condition); \
    PASS(msg);


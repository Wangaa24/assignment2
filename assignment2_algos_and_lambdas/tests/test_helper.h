//
// Created by Saxion on 2018-12-30.
//

#ifndef ALGOS_TEST_HELPER_H
#define ALGOS_TEST_HELPER_H

#include <random>
#include "task.h"

struct test_helper{
    static auto assignees() {
        return std::vector<std::string>{"alice", "bob", "cindy", "daniel", "eve"};
    }

//    int id;
//    std::string name;
//    int priority;
//    double cost;
//    time_type deadline;
//    std::set<std::string> assignees;

    template <typename _Cont>
    static void random_shuffle(_Cont& container) {
        std::shuffle(std::begin(container), std::end(container), std::mt19937(std::random_device()()));
    }

    static long long random(long long a, long long b){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<long long> dis(a, b);
        return dis(gen);
    }

    static auto now(){
        return saxion::task::clock_type::now();
    }

    static saxion::task::time_difference_type day(){
        return std::chrono::hours(24);
    }

    static saxion::task::time_difference_type hour(){
        return std::chrono::hours(1);
    }

    static saxion::task::time_difference_type minute(){
        return std::chrono::minutes(1);
    }

    static auto tasks() {

        auto names = assignees();
        auto now = test_helper::now();
        auto day = test_helper::day();
        auto hour = test_helper::hour();
        auto minute = test_helper::minute();


        auto tasks =  std::vector<saxion::task>{
                { 1001, "task a", 10, 80.0, now + 1 * day, { names[0], names[2], names[3] }},
                { 1002, "task b", 4,  70.0, now + 7 * day, {names[1], names[0] }},
                { 1003, "task c", 1,  15.0, now + 3 * day, {names[0], names[3] }},
                { 1004, "task d", 5,  25.0, now + minute, {names[3], names[0] }},
                { 1005, "task e", 7,  30.0, now - 1 * day, {names[1], names[1], "frank" }},
                { 1006, "task f", 4,  70.0, now - 3 * day, {names[1], names[2] }},
                { 1007, "task g", 3,  55.0, now + 2 * day, {names[2], names[3], names[1] }},
                { 1008, "task h", 8,  50.0, now + 49 * hour, {names[0], names[3] }},
                { 1009, "task i", 6,   5.0, now + 40 * hour, {names[2], names[3] }},
                { 1010, "task j", 4,  35.0, now + 2 * day, {names[3], names[0] }},
                { 1011, "task k", 6,  30.0, now + 18 * hour, {names[2], names[0], "gia" }},
                { 1012, "task l", 7,  80.0, now + 3 * day, {names[1], names[0] }},
                { 1013, "task m", 2,  20.0, now + 10 * hour, {names[0], names[1] }},
                { 1014, "task n", 9,  35.0, now - 10 * minute, {names[1], names[1] }},
                { 1015, "task o", 4,  85.0, now + 36 * hour, {names[0], names[3], names[1], "gia" }},
                { 1016, "task p", 3,  65.0, now + 3 * day, {names[3], names[0] }},
                { 1017, "task q", 5,  95.0, now + 5 * day, {names[3], names[3] }},
                { 1018, "task r", 6,  10.0, now - 10 * day, {names[0], names[4], names[1] }},
                { 1019, "task s", 7,  45.0, now + 4 * day, {names[1], names[0] }},
                { 1020, "task t", 3,  35.0, now + 23 * hour, {names[2], names[0] }},

        };

        random_shuffle(tasks);

        return tasks;
    }

    static auto empty_task(){
        return saxion::task{1100, "task x", 1, 100.0, now() - day(), {} };
    }

};

#endif //ALGOS_TEST_HELPER_H

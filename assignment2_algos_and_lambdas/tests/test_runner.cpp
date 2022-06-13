//
// Created by Saxion on 2018-12-30.
//

#include <gtest/gtest.h>
#include <map>
#include <algorithm>

int main(int argc, char** argv) {

    std::map<std::string, int> points{
            {"has_all_tasks_assigned", 1},
            {"has_task_with_deadline_afer", 1},
            {"remove_asignee_from_all", 2},
            {"extend_deadlines", 1},
            {"count_tasks_with_deadlines_before", 1},
            {"add_assignee_to_task", 2},
            {"get_tasks_with_priority", 1},
            {"extract_tasks_with_deadline_before", 2},
            {"list_sorted_by_prio", 2},
            {"remove_all_finished", 1},
            {"get_nth_to_complete", 1},
            {"get_first_n_to_complete", 1},
            {"cost_burndown", 3},
            {"cheapest_and_most_expensive", 1},
            {"total_cost", 1},
            {"total_cost_of", 2},
            {"separate_by_deadline", 1},
            {"estimate_workload", 2},
            {"average_cost_of_prio", 2}
    };

    ::testing::InitGoogleTest(&argc, argv);

    ::testing::UnitTest& unit_test = *::testing::UnitTest::GetInstance();

//    ::testing::TestEventListeners& listeners = unit_test.listeners();

    auto ret_val = RUN_ALL_TESTS();
    // print detailed output diagnostics


    auto& out = std::cout;

    auto sum{0};

    for (auto i = 0; i < unit_test.total_test_case_count(); ++i) { // for each suite
        const auto& test_case = *unit_test.GetTestCase(i);
        for (auto j = 0; j < test_case.total_test_count(); ++j) { // for each test case in a suite
            const auto& test_info = *test_case.GetTestInfo(j);

            if (test_info.result()->Passed()){
                sum += points[test_info.name()];
            }

        }
    }

    out << "\n\nYou've scored (approximately) " << (sum > 25? 25 : sum) << " points.\n";

    return ret_val;
}
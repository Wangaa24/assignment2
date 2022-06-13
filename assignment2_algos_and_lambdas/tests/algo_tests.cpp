//
// Created by Saxion on 2018-12-30.
//

#include <algorithm>
#include <gtest/gtest.h>
#include "algos.h"
#include "test_helper.h"


TEST(algorithms, has_all_tasks_assigned) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();
    ASSERT_TRUE(algos.has_all_tasks_assigned(std::begin(tasks), std::end(tasks))) << "All tasks are assigned";
    tasks.emplace_back(test_helper::empty_task());
    ASSERT_FALSE(algos.has_all_tasks_assigned(std::begin(tasks), std::end(tasks))) << "One task is not assigned";
}

TEST(algorithms, has_task_with_deadline_afer) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto last_deadline = std::max_element(tasks.begin(), tasks.end(), [](auto& lhs, auto& rhs) {
        return lhs.deadline < rhs.deadline;
    })->deadline;

    ASSERT_FALSE(algos.has_task_with_deadline_afer(std::begin(tasks), std::end(tasks), last_deadline))
                                << "No task has a deadline greater than _last_deadline_";
    tasks.emplace_back(test_helper::empty_task());
    tasks.back().deadline = last_deadline + test_helper::hour();
    test_helper::random_shuffle(tasks);
    ASSERT_TRUE(algos.has_task_with_deadline_afer(std::begin(tasks), std::end(tasks), last_deadline))
                                << "One task has a deadline greater than _last_deadline_";
}

TEST(algorithms, remove_asignee_from_all) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();
    auto names = test_helper::assignees();
    test_helper::random_shuffle(names);
    auto name = names.front();

    std::vector<std::set<std::string>> assignees;
    for (auto& task : tasks) {
        assignees.emplace_back(task.assignees.begin(), task.assignees.end());
    }

    //check if removing a dummy changes anything
    algos.remove_asignee_from_all(std::begin(tasks), std::end(tasks), "dummy");

    auto assignee = assignees.begin();
    for (auto& task : tasks) {
        ASSERT_EQ(*assignee, task.assignees) << "removing dummy shouldn't change anything";
        ++assignee;
    }

    algos.remove_asignee_from_all(std::begin(tasks), std::end(tasks), name);

    assignee = assignees.begin();
    for (auto& task : tasks) {
        if (std::find(assignee->begin(), assignee->end(), name) == assignee->end()) {
            ASSERT_EQ(*assignee, task.assignees) << "removing a name from this set shouldn't change anything";
        } else {
            assignee->erase(name);
            ASSERT_EQ(*assignee, task.assignees) << "a name wasn't properly removed";
        }
        ++assignee;

    }
}

TEST(algorithms, extend_deadlines) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    //pick priority
    auto prio = tasks[5].priority;
    auto extension = test_helper::minute() * 275;

    std::vector<saxion::task::time_type> deadlines_extended;
    for (auto& task : tasks) {
        deadlines_extended.emplace_back(task.priority == prio ? task.deadline + extension : task.deadline);
    }

    algos.extend_deadlines(tasks.begin(), tasks.end(), prio, extension);

    auto deadline = deadlines_extended.begin();
    for (auto& task: tasks) {
        ASSERT_EQ(task.deadline, *deadline) << "The deadline is apparently wrong";
        ++deadline;
    }
}

TEST(algorithms, count_tasks_with_deadlines_before) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto deadline = test_helper::now();

    auto count = 0;
    for (auto& task: tasks){
        if (task.deadline < deadline){
            ++count;
        }
    }

    auto acount = algos.count_tasks_with_deadlines_before(tasks.begin(), tasks.end(), deadline);

    ASSERT_EQ(count, acount) << "The returned count should be " << count;

    deadline += test_helper::day() * 2;

    count = 0;
    for (auto& task: tasks){
        if (task.deadline < deadline){
            ++count;
        }
    }

    acount = algos.count_tasks_with_deadlines_before(tasks.begin(), tasks.end(), deadline);

    ASSERT_EQ(count, acount) << "The returned count should be " << count;
}

TEST(algorithms, add_assignee_to_task) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    // non existing task:
    ASSERT_FALSE(algos.add_assignee_to_task(tasks.begin(), tasks.end(), 999, "alice")) << "This task doesn't exist!";

    auto id = tasks[11].id;
    auto existing = *tasks[11].assignees.begin();

    ASSERT_FALSE(algos.add_assignee_to_task(tasks.begin(), tasks.end(), id, existing))
                                << "This task already had assignee: " << existing;

    id = tasks.back().id;

    ASSERT_TRUE(algos.add_assignee_to_task(tasks.begin(), tasks.end(), id, "zack"))
                                << "This task didn't have assignee: " << "zack";

    ASSERT_TRUE(std::find(tasks.back().assignees.begin(), tasks.back().assignees.end(), "zack") !=
                tasks.back().assignees.end()) << "It looks like zack wasn't added";
}

TEST(algorithms, get_tasks_with_priority) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto empty = algos.get_tasks_with_priority(tasks.begin(), tasks.end(), 11);

    ASSERT_TRUE(empty.empty()) << "No tasks has a priority 11";

    auto prio = tasks[5].priority;

    std::set<int> tasks_with_prio;
    for (auto& task : tasks) {
        if (task.priority == prio) {
            tasks_with_prio.emplace(task.id);
        }
    }

    auto ptasks = algos.get_tasks_with_priority(tasks.begin(), tasks.end(), prio);

    for (auto& task : ptasks) {
        ASSERT_EQ(task.priority, prio) << "Only tasks with priority" << prio << " should be returned";
        ASSERT_TRUE(std::find(tasks_with_prio.begin(), tasks_with_prio.end(), task.id) != tasks_with_prio.end())
                                    << "Wrong task returned";
        tasks_with_prio.erase(task.id);
    }

    ASSERT_TRUE(tasks_with_prio.empty()) << "Some tasks weren't returned";
}

TEST(algorithms, extract_tasks_with_deadline_before) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    saxion::task::time_type deadline = test_helper::now() + test_helper::random(0, 2880) * test_helper::minute();

    std::vector<int> before, after;
    for (auto& task : tasks) {
        if (task.deadline < deadline) {
            before.emplace_back(task.id);
        } else {
            after.emplace_back(task.id);
        }
    }

    std::vector<saxion::task> extracted;

    auto p = algos.extract_tasks_with_deadline_before(tasks.begin(), tasks.end(), std::back_inserter(extracted), deadline);

    tasks.erase(p, tasks.end());

    ASSERT_EQ(before.size(), extracted.size());
    ASSERT_EQ(after.size(), tasks.size());
}

TEST(algorithms, list_sorted_by_prio) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    std::set<std::tuple<int, int>> id_prios;
    for (auto& task : tasks) {
        id_prios.emplace(task.id, task.priority);
    }

    auto sorted = algos.list_sorted_by_prio(tasks.begin(), tasks.end());

    auto prio = -1;
    auto id = -1;
    for (auto idp : sorted) {
        ASSERT_TRUE(std::get<1>(idp) > prio || (std::get<1>(idp) == prio && std::get<0>(idp) > id))
                                    << "Elements not ordered";
        prio = std::get<1>(idp);
        id = std::get<0>(idp);
        ASSERT_TRUE(id_prios.erase(idp) == 1) << "Wrong element in the returned collection";
    }


    ASSERT_TRUE(id_prios.empty()) << "Some tasks weren't returned";
}

TEST(algorithms, remove_all_finished) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();
    saxion::task::time_type deadline = test_helper::now();

    std::set<int> to_do;
    for (auto& task : tasks) {
        if (task.deadline > deadline) {
            to_do.emplace(task.id);
        }
    }

    algos.remove_all_finished(tasks);

    ASSERT_EQ(tasks.size(), to_do.size());
    for (auto& task : tasks) {
        ASSERT_TRUE(to_do.erase(task.id) == 1) << "A task with id " << task.id << " should have been removed";
    }

    ASSERT_TRUE(to_do.empty()) << "Some unfisnihed tasks were removed";

}

TEST(algorithms, get_nth_to_complete) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    for (auto i = 0; i < 10; ++i) {
        int n = static_cast<int>(test_helper::random(0, tasks.size() - 1));

        auto ntask = algos.get_nth_to_complete(tasks.begin(), tasks.end(), n);
        auto before = 0, after = 0;
        for (auto& task: tasks) {
            if (task.deadline < ntask.deadline) {
                ++before;
            } else if (task.deadline > ntask.deadline) {
                ++after;
            } else {
                if (task.priority < ntask.priority) {
                    ++before;
                } else if (task.priority > ntask.priority) {
                    ++after;
                }
            }
        }

        ASSERT_EQ(n, before) << "Wrong task returned";
    }

}

TEST(algorithms, get_first_n_to_complete) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto n = static_cast<int>(test_helper::random(2, 10));

    auto first = algos.get_first_n_to_complete(tasks.begin(), tasks.end(), n);

    ASSERT_EQ(first.size(), n) << "The count of tasks returned should be " << n;

    int last_ind = 0; // last task to complete of the first n
    for (auto i = 1; i < static_cast<int>(first.size()); ++i) {
        if ((first[i].deadline > first[last_ind].deadline) ||
            ((first[i].deadline == first[last_ind].deadline) &&
             (first[i].priority > first[last_ind].priority)
            )
                ) {
            last_ind = i;
        }
    }

    // last of the n first tasks
    auto& last = first[last_ind];

    std::set<int> first_id, all_id;
    for (auto& task : tasks) {
        all_id.emplace(task.id);
    }

    for (auto& task : first) {
        first_id.emplace(task.id);
    }

    std::set<int> last_id;
    std::set_difference(all_id.begin(), all_id.end(), first_id.begin(), first_id.end(), std::inserter(last_id, last_id.end()));

    for (auto& task : tasks) {
        if (last_id.erase(task.id) == 1) {
            ASSERT_TRUE((task.deadline > last.deadline) ||
                        ((task.deadline == last.deadline) && (task.priority > last.priority)))
                                        << "A task with the following id should be returned " << task.id;
        }
    }


}

TEST(algorithms, cost_burndown) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    std::vector<double> costs;

    algos.cost_burndown(tasks.begin(), tasks.end(), std::back_inserter(costs));

    // since we happen to know the correct answer...
    std::vector<double> proper{10, 80, 110, 145, 170, 190, 220, 255, 335, 420, 425, 515, 565, 725, 770, 865, 935};

    ASSERT_EQ(costs.size(), proper.size()) << "The count of returned accumulated costs is incorrect";
    for (std::size_t i = 0; i < costs.size(); ++i){
        ASSERT_DOUBLE_EQ(costs[i], proper[i]) << "Some returned values are not ok";
    }
}

TEST(algorithms, cheapest_and_most_expensive) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    int id_min = -1, id_max = -1;
    double min = std::numeric_limits<double>::max(), max = std::numeric_limits<double>::min();

    for (auto& task: tasks) {
        if (task.cost > max) {
            max = task.cost;
            id_max = task.id;
        } else if (task.cost < min) {
            min = task.cost;
            id_min = task.id;
        }
    }

    auto[cheap, expensive] = algos.cheapest_and_most_expensive(tasks.begin(), tasks.end());

    ASSERT_EQ(min, cheap.cost) << "The cheapest task should cost " << min;
    ASSERT_EQ(id_min, cheap.id) << "The cheapest task id should be " << id_min;

    ASSERT_EQ(max, expensive.cost) << "The most expensive task should cost " << max;
    ASSERT_EQ(id_max, expensive.id) << "The most expensive task id should be " << id_max;
}

TEST(algorithms, total_cost) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto sum = 0.;
    for (auto& task: tasks) {
        sum += task.cost;
    }

    auto asum = algos.total_cost(tasks.begin(), tasks.end());

    ASSERT_DOUBLE_EQ(sum, asum) << "The total cost should be " << sum;

    auto amount = test_helper::random(100, 1000) / 10.0;
    sum -= amount;

    amount /= tasks.size();
    for (auto& task: tasks) {
        task.cost -= amount;
    }

    asum = algos.total_cost(tasks.begin(), tasks.end());

    ASSERT_DOUBLE_EQ(sum, asum) << "The total cost should be " << sum;

}

TEST(algorithms, total_cost_of) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto name = *(tasks[5].assignees.begin());

    auto sum = 0.;
    for (auto& task: tasks) {
        if (std::find(task.assignees.begin(), task.assignees.end(), name) != task.assignees.end()) {
            sum += task.cost;
        }
    }

    auto asum = algos.total_cost_of(tasks.begin(), tasks.end(), name);

    ASSERT_DOUBLE_EQ(sum, asum) << "The total cost should be " << sum;
}

TEST(algorithms, separate_by_deadline) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto deadline = test_helper::now();

    auto p = algos.separate_by_deadline(tasks.begin(), tasks.end(), deadline);

    ASSERT_TRUE(p->deadline < deadline) << "A partition task with id: " << p->id
                                        << " should have a deadline < than the parameter";

    for (auto task = tasks.begin(); task != p; ++task) {
        ASSERT_TRUE(task->deadline < deadline)
                                    << "A task with id: " << task->id << " shouldn't be in the first partition";
    }

    for (auto task = p + 1; task < tasks.end(); ++task) {
        ASSERT_TRUE(task->deadline >= deadline)
                                    << "A task with id: " << task->id << " shouldn't be in the second partition";
    }

    deadline += test_helper::day() * 2;

    p = algos.separate_by_deadline(tasks.begin(), tasks.end(), deadline);

    ASSERT_TRUE(p->deadline < deadline) << "A partition task with id: " << p->id
                                        << " should have a deadline < than the parameter";

    for (auto task = tasks.begin(); task != p; ++task) {
        ASSERT_TRUE(task->deadline < deadline)
                                    << "A task with id: " << task->id << " shouldn't be in the first partition";
    }

    for (auto task = p + 1; task < tasks.end(); ++task) {
        ASSERT_TRUE(task->deadline >= deadline)
                                    << "A task with id: " << task->id << " shouldn't be in the second partition";
    }

}

TEST(algorithms, estimate_workload) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    auto names = test_helper::assignees();
    test_helper::random_shuffle(names);

    auto name = names.front();

    auto count = std::count_if(tasks.begin(), tasks.end(), [name](auto& task) {
        return std::find(task.assignees.begin(), task.assignees.end(), name) != task.assignees.end();
    });
    //min & max possible estimated counts:

    auto sz = static_cast<int>(tasks.size() / 2);
    auto max = std::min(static_cast<int>(count), sz);
    auto min = std::max(0, static_cast<int>(count) - static_cast<int>(tasks.size() - sz));

    auto workload = algos.estimate_workload(tasks.begin(), tasks.end(), name);

    // std::cout << " \n\nwl: " << workload << " | " << name << ": " << count << " | " << min << ", " << max << "\n\n";

    // no real way to auto test this one, so just bound the number:

    ASSERT_TRUE(workload >= (1.0 * min) / sz)
                                << "The estimated workload (" << workload <<") should be greater than: " << (1.0 * min) / sz;
    ASSERT_TRUE(workload <= (1.0 * max) / sz)
                                << "The estimated workload (" << workload << ") should be smaller than: " << (1.0 * max) / sz;


}

TEST(algorithms, average_cost_of_prio) {
    auto tasks = test_helper::tasks();
    auto algos = saxion::algos();

    for (auto i = 0; i < 5; ++i) {
        auto sum = 0.;
        auto count = 0;
        int prio = static_cast<int>(test_helper::random(1, 10));

        for (auto& task: tasks) {
            if (task.priority == prio) {
                sum += task.cost;
                ++count;
            }
        }

        auto cost = algos.average_cost_of_prio(tasks.begin(), tasks.end(), prio);

        ASSERT_DOUBLE_EQ(sum / count, cost) << "Average cost for wrong for priority: " << prio;
    }
}

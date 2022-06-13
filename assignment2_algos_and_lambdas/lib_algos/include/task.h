//
// Created by Saxion on 2018-12-11.
//

#ifndef ALGOS_TASK_H
#define ALGOS_TASK_H

#include <set>
#include <string>
#include <chrono>
#include <iostream>

namespace saxion {

    namespace detail{
        struct name_compare;
        struct id_compare;
        struct priority_compare;
        struct deadline_compare;
    }

    struct task {
        using clock_type = std::chrono::system_clock;
        using time_type = std::chrono::time_point<std::chrono::system_clock>;
        using time_difference_type = std::chrono::system_clock::duration;

        int id;
        std::string name;
        int priority;
        double cost;
        time_type deadline;
        std::set<std::string> assignees;

        void swap(task& other){
            std::swap(id, other.id);
            std::swap(name, other.name);
            std::swap(priority, other.priority);
            std::swap(cost, other.cost);
            std::swap(deadline, other.deadline);
            std::swap(assignees, other.assignees);
        }

        using name_comparator = detail::name_compare;
        using id_comparator  =  detail::id_compare;
        using priority_comparator = detail::priority_compare;
        using deadline_comparator = detail::deadline_compare;


        friend std::ostream& operator<<(std::ostream&, const task&);

    };

    namespace detail {
        struct name_compare {
            bool operator()(const task& lhs, const task& rhs) const {
                return lhs.name < rhs.name;
            }
        };

        struct id_compare {
            constexpr bool operator()(const task& lhs, const task& rhs) const {
                return lhs.id < rhs.id;
            }
        };

        struct priority_compare {
            constexpr bool operator()(const task& lhs, const task& rhs) const {
                return lhs.priority < rhs.priority;
            }
        };

        struct deadline_compare {
            constexpr bool operator()(const task& lhs, const task& rhs) const {
                return lhs.deadline < rhs.deadline;
            }
        };
    }


    std::ostream& operator<<(std::ostream& out, const task& t){
        return out << t.id << ": " << t.name << " | " << t.priority << " | " << t.cost;
    }
}

namespace std{
    void swap(saxion::task& lhs, saxion::task& rhs) noexcept {
        lhs.swap(rhs);
    }
}


#endif //ALGOS_TASK_H

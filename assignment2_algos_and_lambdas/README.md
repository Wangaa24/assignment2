# Assignment 2 (20 points)

The second assignment revolves around the use of `<algorithm>` and `<numeric>` headers. You will have to implement simple functions that process or manipulate objects of type `tasks` that are stored in a container.

The grading of this assignment is partially automatic. Run the `tests_algos` target to see which functions pass the tests and how many points (approximately) you scored.

You can score a maximum of 20 points. If the program says you scored more, you still keep only 20. This also means you don't need to implement everything to get the maximum.

## running the project

1. Open the project.

2. Wait for the *CMake* project to load. It will take a while because it fetches the Google Test framework.

3. Select the target `tests_algos` and run it. All the tests should fail.

4. Continue reading to figure out what you have to do.


## what do I have to do?

You have to implement functions whose stubs are located in [lib_algos/include/algos.h](lib_algos/include/algos.h).
Most of those functions take iterators (`begin` and `end`) to a collection of `task` objects.
You need to use those iterators, and any additional parameters passed to a function, to perform a simple operation.
For instance, you might be asked to count the number of `task`s that meet some criteria or to modify `task`s in some way.

There are three conditions:

1. You use iterators and iterators only.

2. You are not allowed to use manual loops (`for`, `while`, `do-while`).

3. In the whole *algos.h* file you may use either [`std::for_each`](https://devdocs.io/cpp/algorithm/for_each) or [`std::for_each_n`](https://devdocs.io/cpp/algorithm/for_each_n) exactly once.
   (This means that only one of all the functions you implement may use either of those standard functions).

4. You cannot add any extra files to the project that will be referred/ included by *algos.h*.
   (You can add more files or even targets for your own experimenting - you just cannot use them for implementing functions in *algos.h*.)


Let's see how this could work.

## `task`

`task` is an _abstract data type_ that represents some task to be carried out. Each `task` has:

* a unique `id` associated with it,
* a `deadline` defined with resolution of at least one second,
* a `priority` - a numeric value from 1 to 10; __a lower number represents a tasks with higher priority__,
* a `cost` that represents the resources needed to complete the task,
* a set named `assignees`, which contains the names of employees assigned to the task,
* a `name` - a non-unique name associated with the task.

The `task` definition can be found in [lib_algos/include/task.h](lib_algos/include/task.h); its synopsis is shown below:

```cpp
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
};
```

## functions

All the functions that you have to implement can be found in [lib_algos/include/algos.h](lib_algos/include/algos.h). 
Each function declaration is accompanied by a short description of what it should do. 
For instance, let's say that the following function declaration is given:

```cpp
// 1 point
template <typename _Iter>
auto count_tasks_with_prio(_Iter begin, _Iter end, int prio){
    // returns the count of tasks with priority <prio>
    return -1;
}
```

This function takes iterators to the beginning and the end of some container and returns the count of tasks in this container that have priority `prio`. 
You will notice that almost all the functions that you have to implement take the `begin` and `end` iterators as parameters.

A naive implementation of the function iterates over the container using the provided iterators and _manually_ counts the tasks:

```cpp
// 1 point
template <typename _Iter>
auto count_tasks_with_prio(_Iter begin, _Iter end, int prio){
    auto count = 0;
    for (auto t = begin, t != end; ++t){
        if (t->priority == prio){
            ++count;
        }
    }
    return count;
}
```

But such an implementation won't score you any points. 
The goal of this assignment is to use functions from the [`<algorithm>`](https://devdocs.io/cpp/header/algorithm) and [`<numeric>`](https://devdocs.io/cpp/header/numeric) headers. 
The standard function that matches the task at hand is `std::count_if`. Let's use it:

```cpp
// 1 point
template <typename _Iter>
auto count_tasks_with_prio(_Iter begin, _Iter end, int prio){
    return std::count_if(begin, end, [prio](const task& t){
        return t.priority == prio;
    });
}
```

Notice that one of the arguments passed to the `std::count_if` function is the lambda expression:

```cpp
[prio] (const task& t) { return t.priority == prio; }
```

It captures the `prio` argument to be able to use it. The only parameter to the lambda expression is of type `task`, because that's what we iterate over. 
The result of the lambda expression is just a `bool` value indicating whether the current task's priority is equal to `prio`.

## tips

1. All the other functions can be implemented in similar way. It's a good idea to start with a sketch of an algorithm using the traditional `for` loops and later transform it into an appropriate function call that uses a lambda expression to express the algorithm's logic.

2. Run the `tests_algos` target to see which functions pass the tests and how many points (approximately) you scored. (If all the functions are implemented using `<algorithm>` & `<numeric>` headers, using lambda expression, the score is exact.)

3. You are allowed to re-order the elements in a container to which the iterators point. You shouldn't, however, modify them, unless explicitly requested.

4. `std::accumulate` is actually a [fold](https://en.wikipedia.org/wiki/Fold_(higher-order_function)). It can be used very creatively. See, for instance, [this StackOverflow question](https://stackoverflow.com/questions/12633950/understanding-stdaccumulate?answertab=votes#tab-top)

4. Functions awarded with one point can be implemented with a single `std::` function call, sometimes you'll need to write one more line to somehow transform the result, but that's all.

5. Functions awarded with two points usually require calls to two `std::` functions. Sometimes one of those calls is nested in a lambda expression. Often, one of those two functions is `std::find`

6. Tasks awarded with three points do not necessarily require three different `std::` function calls. They are just more difficult to implement.

That's all!

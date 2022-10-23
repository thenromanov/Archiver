#include <catch.hpp>
#include <queue>

#include "priority_queue.hpp"

TEST_CASE("Basics") {
    {
        PriorityQueue<int> q;
        REQUIRE(q.Size() == 0);
        REQUIRE(q.Empty() == true);
        q.Push(1);
        q.Push(3);
        REQUIRE(q.Top() == 1);
        q.Pop();
        REQUIRE(q.Top() == 3);
        q.Pop();
    }
    {
        PriorityQueue<int> q({1, 3, 2});
        REQUIRE(q.Size() == 3);
        REQUIRE(q.Top() == 1);
        q.Pop();
        REQUIRE(q.Top() == 2);
        q.Pop();
        REQUIRE(q.Top() == 3);
        q.Pop();
    }
    {
        std::vector<int> value{3, 1, 3};
        PriorityQueue<int> q(value);
        REQUIRE(q.Size() == 3);
        REQUIRE(q.Extract() == 1);
        REQUIRE(q.Extract() == 3);
        REQUIRE(q.Extract() == 3);
        REQUIRE(q.Size() == 0);
    }
}

TEST_CASE("Custom comparator") {
    {
        PriorityQueue<char, std::vector<char>, std::greater<char>> q({'a', 'c', 'b', 'd'});
        REQUIRE(q.Size() == 4);
        REQUIRE(q.Top() == 'd');
        q.Pop();
        REQUIRE(q.Extract() == 'c');
        REQUIRE(q.Extract() == 'b');
        REQUIRE(q.Extract() == 'a');
        REQUIRE(q.Size() == 0);
    }
}

TEST_CASE("Undefined behaviour") {
    {
        PriorityQueue<int> q;
        q.Push(1);
        q.Push(2);
        q.Pop();
        q.Pop();
        try {
            q.Pop();
        } catch (const std::exception& e) {
            return;
        }
        FAIL("IllegalOperation");
    }
}
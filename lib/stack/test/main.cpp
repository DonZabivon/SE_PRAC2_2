#include <iostream>
#include <string>
#include <cassert>
using namespace std;

#include "stack.h"

void test_def_constr()
{
    Stack<int> s;
    assert(s.empty());
    assert(s.size() == 0);
    assert(to_string(s) == "0:");
}

void test_copy_constr()
{
    Stack<int> s1;
    for (int i = 0; i < 5; ++i)
    {
        s1.push(i);
    }

    Stack<int> s2 = s1;

    assert(to_string(s1) == "5: 4 3 2 1 0");
    assert(to_string(s2) == "5: 4 3 2 1 0");

    s1.clear();

    assert(to_string(s1) == "0:");
    assert(to_string(s2) == "5: 4 3 2 1 0");
}

void test_move_constr()
{
    Stack<int> s1;
    for (int i = 0; i < 5; ++i)
    {
        s1.push(i);
    }

    Stack<int> s2 = std::move(s1);

    assert(to_string(s1) == "0:");
    assert(to_string(s2) == "5: 4 3 2 1 0");
}

void test_copy_assign()
{
    Stack<int> s1, s2;
    for (int i = 0; i < 5; ++i)
    {
        s1.push(i);
    }

    s2 = s1;

    assert(to_string(s1) == "5: 4 3 2 1 0");
    assert(to_string(s2) == "5: 4 3 2 1 0");

    s1.clear();

    assert(to_string(s1) == "0:");
    assert(to_string(s2) == "5: 4 3 2 1 0");
}

void test_move_asign()
{
    Stack<int> s1, s2;
    for (int i = 0; i < 5; ++i)
    {
        s1.push(i);
    }

    s2 = std::move(s1);

    assert(to_string(s1) == "0:");
    assert(to_string(s2) == "5: 4 3 2 1 0");
}

void test_empty()
{
    Stack<int> s;
    assert(s.empty());
    assert(s.size() == 0);

    s.push(1);
    assert(!s.empty());
}

void test_size()
{
    Stack<int> s;
    assert(s.size() == 0);

    for (int i = 0; i < 5; ++i)
    {
        assert((int)s.size() == i);
        s.push(i);
        assert((int)s.size() == i + 1);
    }

    for (int i = 5; i > 0; --i)
    {
        assert((int)s.size() == i);
        s.pop();
        assert((int)s.size() == i - 1);
    }
}

void test_clear()
{
    Stack<int> s;

    s.clear();
    assert(s.size() == 0);
    assert(s.empty());

    for (int i = 0; i < 5; ++i)
    {
        s.push(i);
    }

    s.clear();
    assert(s.size() == 0);
    assert(s.empty());
}

void test_top()
{
    Stack<int> s;

    try
    {
        s.top();
        assert(false);
    }
    catch (const underflow_error &)
    {
        // OK
    }
    catch (const exception &)
    {
        assert(false);
    }

    for (int i = 0; i < 5; ++i)
    {
        s.push(i);
        assert(s.top() == i);
    }

    for (int i = 5; i > 0; --i)
    {
        assert(s.top() == i - 1);
        s.pop();
    }

    try
    {
        s.top();
        assert(false);
    }
    catch (const underflow_error &)
    {
        // OK
    }
    catch (const exception &)
    {
        assert(false);
    }
}

void test_top_const()
{
    Stack<int> s1;
    for (int i = 0; i < 5; ++i)
    {
        s1.push(i);
    }

    const Stack<int> s2 = s1;
    assert(s2.top() == 4);
}

void test_push()
{
    Stack<int> s;

    for (int i = 0; i < 5; ++i)
    {
        s.push(i);
    }

    assert(s.size() == 5);
    assert(to_string(s) == "5: 4 3 2 1 0");
}

void test_move_push()
{
    Stack<string> s;

    for (int i = 0; i < 5; ++i)
    {
        string v = to_string(i);
        s.push(std::move(v));
        assert(v.empty());
    }

    assert(s.size() == 5);
    assert(to_string(s) == "5: 4 3 2 1 0");
}

void test_simple_pop()
{
    Stack<string> s;

    try
    {
        s.pop();
        assert(false);
    }
    catch (const underflow_error &)
    {
        // OK
    }
    catch (const exception &)
    {
        assert(false);
    }

    for (int i = 0; i < 5; ++i)
    {
        string x = to_string(i);
        s.push(x);
    }

    for (int i = 5; i > 0; --i)
    {
        s.pop();
    }

    try
    {
        s.pop();
        assert(false);
    }
    catch (const underflow_error &)
    {
        // OK
    }
    catch (const exception &)
    {
        assert(false);
    }
}

void test_pop()
{
    Stack<string> s;
    string x = "x";

    try
    {
        s.pop(x);
        assert(false);
    }
    catch (const underflow_error &)
    {
        // OK
        assert(x == "x");
    }
    catch (const exception &)
    {
        assert(false);
    }

    for (int i = 0; i < 5; ++i)
    {
        x = to_string(i);
        s.push(x);
    }

    for (int i = 5; i > 0; --i)
    {
        s.pop(x);
        assert(x == to_string(i - 1));
    }

    try
    {
        s.pop(x);
        assert(false);
    }
    catch (const underflow_error &)
    {
        // OK
    }
    catch (const exception &)
    {
        assert(false);
    }
}

void test_move_pop()
{
    Stack<string> s;

    string x = "x";

    try
    {
        s.pop(std::move(x));
        assert(false);
    }
    catch (const underflow_error &)
    {
        // OK
        assert(x == "x");
    }
    catch (const exception &)
    {
        assert(false);
    }

    for (int i = 0; i < 5; ++i)
    {
        x = to_string(i);
        s.push(std::move(x));
    }

    for (int i = 5; i > 0; --i)
    {
        s.pop(std::move(x));
        assert(x == to_string(i - 1));
    }
}

void test_equality()
{
    Stack<int> s1, s2;

    assert(s1 == s2);
    assert(!(s1 != s2));

    for (int i = 0; i < 5; ++i)
    {
        s1.push(i);
    }

    assert(s1 != s2);
    assert(!(s1 == s2));

    for (int i = 0; i < 5; ++i)
    {
        s2.push(i);
    }

    assert(s1 == s2);
    assert(!(s1 != s2));

    for (int i = 0; i < 5; ++i)
    {
        s1.push(i);
        s2.push(i + 1);
    }

    assert(s1 != s2);
    assert(!(s1 == s2));

    while (s1.size() > 5)
    {
        s1.pop();
    }

    while (s2.size() > 5)
    {
        s2.pop();
    }

    assert(s1 == s2);
    assert(!(s1 != s2));

    while (!s1.empty())
    {
        s1.pop();
    }

    while (!s2.empty())
    {
        s2.pop();
    }

    assert(s1 == s2);
    assert(!(s1 != s2));
}

void test_to_string()
{
    Stack<int> s;

    for (int i = 0; i < 10; ++i)
    {
        s.push(i);
    }

    assert(to_string(s) == "10: 9 8 7 6 5 4 3 2 1 0");

    for (int i = 0; i < 5; ++i)
    {
        s.pop();
    }

    assert(to_string(s) == "5: 4 3 2 1 0");

    for (int i = 0; i < 5; ++i)
    {
        s.pop();
    }

    assert(to_string(s) == "0:");
}

int main()
{
    test_def_constr();
    test_copy_constr();
    test_move_constr();
    test_copy_assign();
    test_move_asign();
    test_empty();
    test_size();
    test_clear();
    test_top();
    test_top_const();
    test_push();
    test_move_push();
    test_pop();
    test_move_pop();
    test_equality();
    test_to_string();

    cout << "All tests passed!" << endl;
    return 0;
}
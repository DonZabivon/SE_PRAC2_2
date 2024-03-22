#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>

template <typename T>
class Stack
{
    struct Node
    {
        T m_val;
        Node *m_next;
        Node(const T &val, Node *next = nullptr) : m_val(val), m_next(next) {}
        Node(T &&val, Node *next = nullptr) : m_val(std::move(val)), m_next(next) {}
    };

    size_t m_size = 0;
    Node *m_top = nullptr;

public:
    Stack();

    Stack(const Stack &x);

    Stack(Stack &&x);

    ~Stack();

    Stack &operator=(const Stack &x);

    Stack &operator=(Stack &&x);

    bool empty() const;

    size_t size() const;

    void clear();

    T &top();

    const T &top() const;

    void push(const T &val);

    void push(T &&val);

    void pop(T &val);

    void pop(T &&val = T());

    void swap(Stack &x);

    template <typename U>
    friend bool operator==(const Stack<U> &x, const Stack<U> &y);

    template <typename U>
    friend bool operator!=(const Stack<U> &x, const Stack<U> &y);

    template <typename U>
    friend std::string to_string(const Stack<U> &x);

    template <typename U>
    friend std::ostream &operator<<(std::ostream &out, const Stack<U> &x);
};

template <typename T>
Stack<T>::Stack()
{
}

template <typename T>
Stack<T>::Stack(const Stack<T> &x)
{
    if (!x.empty())
    {
        m_top = new Node(x.m_top->m_val);
        m_size++;
        Node *prev = m_top;
        for (Node *p = x.m_top->m_next; p; p = p->m_next)
        {
            Node *next = new Node(p->m_val);
            prev->m_next = next;
            prev = next;
            m_size++;
        }
    }
}

template <typename T>
Stack<T>::Stack(Stack<T> &&x)
{
    swap(x);
}

template <typename T>
Stack<T>::~Stack()
{
    clear();
}

template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &x)
{
    if (&x != this)
    {
        Stack y = x;
        swap(y);
    }
    return *this;
}

template <typename T>
Stack<T> &Stack<T>::operator=(Stack<T> &&x)
{
    if (&x != this)
    {
        swap(x);
    }
    return *this;
}

template <typename T>
bool Stack<T>::empty() const
{
    return m_top == nullptr;
}

template <typename T>
size_t Stack<T>::size() const
{
    return m_size;
}

template <typename T>
void Stack<T>::clear()
{
    while (!empty())
    {
        pop();
    }
}

template <typename T>
T &Stack<T>::top()
{
    return const_cast<T &>(static_cast<const Stack<T> *>(this)->top());
}

template <typename T>
const T &Stack<T>::top() const
{
    if (empty())
    {
        throw std::underflow_error("Stack is empty.");
    }
    return m_top->m_val;
}

template <typename T>
void Stack<T>::push(const T &val)
{
    T t = val;
    push(std::move(t));
}

template <typename T>
void Stack<T>::push(T &&val)
{
    m_top = new Node(std::move(val), m_top);
    m_size++;
}

template <typename T>
void Stack<T>::pop(T &val)
{
    pop(std::move(val));
}

template <typename T>
void Stack<T>::pop(T &&val)
{
    if (empty())
    {
        throw std::underflow_error("Stack is empty.");
    }
    Node *del = m_top;
    m_top = m_top->m_next;
    std::swap(val, del->m_val);
    delete del;
    m_size--;
}

template <typename T>
void Stack<T>::swap(Stack<T> &x)
{
    std::swap(this->m_top, x.m_top);
    std::swap(this->m_size, x.m_size);
}

template <typename T>
bool operator==(const Stack<T> &x, const Stack<T> &y)
{
    if (x.size() == y.size())
    {
        typename Stack<T>::Node *px = x.m_top;
        typename Stack<T>::Node *py = y.m_top;
        while (px)
        {
            if (px->m_val != py->m_val)
            {
                return false;
            }
            px = px->m_next;
            py = py->m_next;
        }
        return true;
    }
    return false;
}

template <typename T>
bool operator!=(const Stack<T> &x, const Stack<T> &y)
{
    return !(x == y);
}

template <typename T>
std::string to_string(const Stack<T> &x)
{
    ostringstream s;
    s << x;
    return s.str();
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Stack<T> &x)
{
    out << std::to_string(x.size()) << ":";
    typename Stack<T>::Node *px = x.m_top;
    while (px)
    {
        out << " " << px->m_val;
        px = px->m_next;
    }
    return out;
}

#endif

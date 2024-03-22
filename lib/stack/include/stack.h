#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <sstream>

template <typename T>
class Stack
{
    size_t m_size = 0;
    size_t m_cap = 1;
    T *m_data = nullptr;

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
    friend ostream &operator<<(ostream &out, const Stack<U> &x);
};

template <typename T>
Stack<T>::Stack() : m_size(0), m_cap(1)
{
    m_data = new T[m_cap];
}

template <typename T>
Stack<T>::Stack(const Stack<T> &x) : m_size(x.m_size), m_cap(x.m_cap)
{
    m_data = new T[m_cap];
    std::copy(x.m_data, x.m_data + x.m_size, m_data);
}

template <typename T>
Stack<T>::Stack(Stack<T> &&x) : Stack()
{
    swap(x);
}

template <typename T>
Stack<T>::~Stack()
{
    delete[] m_data;
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
    return m_size == 0;
}

template <typename T>
size_t Stack<T>::size() const
{
    return m_size;
}

template <typename T>
void Stack<T>::clear()
{
    m_cap = 1;
    m_size = 0;
    delete m_data;
    m_data =  new T[m_cap];    
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
    return m_data[m_size - 1];
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
    if (m_size == m_cap)
    {
        m_cap *= 2;
        T *new_data = new T[m_cap];
        std::copy(m_data, m_data + m_size, new_data);
        delete[] m_data;
        m_data = new_data;
    }
    m_data[m_size++] = std::move(val);    
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
    val = std::move(m_data[--m_size]);

    if (m_cap >= 2 && m_size <= m_cap / 4)
    {        
        m_cap /= 2;
        T *new_data = new T[m_cap];
        std::copy(m_data, m_data + m_size, new_data);
        delete[] m_data;
        m_data = new_data;
        
    }    
}

template <typename T>
void Stack<T>::swap(Stack<T> &x)
{
    std::swap(this->m_size, x.m_size);
    std::swap(this->m_cap, x.m_cap);
    std::swap(this->m_data, x.m_data);
}

template <typename T>
bool operator==(const Stack<T> &x, const Stack<T> &y)
{
    if (x.size() == y.size())
    {
        for (size_t i = 0; i < x.size(); ++i)
        {
            if (x.m_data[i] != y.m_data[i])
            {
                return false;
            }
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
    if (!x.empty())
    {
        for (size_t i = x.size(); i > 0; --i)
        {
            out << " " << x.m_data[i - 1];
        }
    }
    return out;
}

#endif

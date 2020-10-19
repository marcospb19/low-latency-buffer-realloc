#include <iostream>
using namespace std;

void panic(char const* const panic_message)
{
    cerr << panic_message << ".\n";
    exit(1);
}

const int DEFAULT_CAPACITY = 20;
const int GROWTH_RATE = 3;

template <typename T>
struct Stack {
    T* m_buf;
    int m_len;
    int m_capacity;

    Stack() : m_len{0}, m_capacity{DEFAULT_CAPACITY}
    {
        m_buf = new T[m_capacity];
    }

    void ensure_capacity()
    {
        if (m_len == m_capacity) {
            m_capacity *= GROWTH_RATE;
            m_buf = (T*)realloc(m_buf, sizeof(T[m_capacity]));

            if (m_buf == NULL) {
                panic("Malloc error in Stack::ensure_capacity");
            }
        }
    }

    void push(T value)
    {
        this->ensure_capacity();
        m_buf[m_len] = value;
        m_len += 1;
    }

    void pop()
    {
        if (m_len == 0) {
            panic("Nothing to pop in Stack::pop");
        }
        m_len -= 1;
    }

    T& get(int i)
    {
        if (i < 0 or i >= m_len) {
            panic("Out of bounds access in Stack::get");
        }
        return m_buf[i];
    }

    T& top()
    {
        if (m_len == 0) {
            panic("Nothing on top in Stack::top");
        }
        return m_buf[m_len - 1];
    }

    int len() { return m_len; }

    void show()
    {
        for (int i = 0; i < m_len; ++i) {
            cout << this->get(i) << ' ';
        }
        cout << '\n';
    }
};

int main()
{
    auto stack = Stack<int>();

    for (int i = 1; i <= 52; ++i) {
        stack.push(i);
    }

    stack.pop();
    stack.pop();

    cout << "len = " << stack.len() << endl;

    stack.show();
}

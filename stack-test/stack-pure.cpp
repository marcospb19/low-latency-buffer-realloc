#include <cassert>
#include <iostream>
using namespace std;

void panic(char const* const panic_message)
{
    cerr << panic_message << ".\n";
    exit(1);
}

const int DEFAULT_CAPACITY = 10;
const int GROWTH_RATE = 2;

template <typename T>
struct Stack {
    T* m_buf;
    int m_len;
    int m_capacity;

    // Old buf stuff
    T* m_old_buf;
    // Those two serve as iterators
    T* m_old_buf_it;
    T* m_old_buf_it_end;

    Stack()
        : m_len{0},
          m_capacity{DEFAULT_CAPACITY},
          m_old_buf{nullptr},        // Can this be removed? is it implicit?
          m_old_buf_it{nullptr},     // Can this be removed? is it implicit?
          m_old_buf_it_end{nullptr}  // Can this be removed? is it implicit?
    {
        m_buf = new T[m_capacity];
    }

    void ensure_capacity()
    {
        if (m_len < m_capacity) {
            return;  // Nothing to do
        }

        // If need allocate more, it means that old_buf is already gone
        assert(m_old_buf == nullptr);

        m_capacity *= GROWTH_RATE;
        auto new_buf = new T[m_capacity];

        // If the new allocated segment starts at the end of the previous one,
        // it means that we don't have to move anything.
        //
        // AAAAAAA________
        // AAAAAAABBBBBBBB
        // AAAAAAAAAAAAAAA // Consider this and continue
        if (new_buf == m_buf + m_len) {
            return;  // Nothing to do

            // Actually we are leaking memory here looooool
            // This optimization is loco Leo
        }

        // We'll set our old_buf variables
        m_old_buf = m_buf;
        m_old_buf_it = m_buf;              // Iterator starts at the [0] element
        m_old_buf_it_end = m_buf + m_len;  // Past the last element

        m_buf = new_buf;
    }

    void advance_lazy_buf_copy()
    {
        if (m_old_buf == nullptr) {
            return;  // Nothing to do
        }
        int index = size_t(m_old_buf_it - m_old_buf);

        m_buf[index] = *m_old_buf_it;

        // ou
        m_buf[index] = m_old_buf[index];

        // Nesse ponto estou questionando a escolha de usar buf_it como
        // ponteiros ao invés de um inteiro básico para dizer a posição

        m_old_buf_it += 1;

        // If we reached the end of our task of copying from old_, get rid of
        // this
        if (m_old_buf_it == m_old_buf_it_end) {
            free(m_old_buf);
            m_old_buf = nullptr;
            m_old_buf_it = nullptr;
            m_old_buf_it_end = nullptr;
        }
    }

    void push(T value)
    {
        this->ensure_capacity();
        this->advance_lazy_buf_copy();
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

        // Se não tem old_buf, tá no buffer normal
        if (m_old_buf == nullptr) {
            return m_buf[i];
        } else {
            // Se tem old_buf, ver se está na partição antiga ou na nova
            if (i < m_capacity / 2) {
                return m_old_buf[i];
            } else {
                return m_buf[i];
            }
        }
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

    for (int i = 1; i <= 80; ++i) {
        stack.push(i);
        stack.show();
    }

    stack.pop();
    stack.pop();

    cout << "len = " << stack.len() << endl;

    stack.show();
}

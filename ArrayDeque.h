#ifndef ArrayDeque_h
#define ArrayDeque_h

#include <algorithm>
#include <stdexcept>

template < class T >
class ArrayDeque
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using rvalue_reference = T&&;
    using pointer = T*;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    pointer buffer;
    size_type capacite;
    size_type debut;
    size_type taille;

public:
    ArrayDeque(size_type cap = 0) : debut(0), taille(0), capacite(cap) {
        buffer = capacite != 0 ?
                 (pointer) ::operator new(capacite * sizeof(value_type))
                               : nullptr;
    }


    size_type size() const {
        return taille;
    }

    size_type capacity() const {}

    bool empty() const {}

    value_type at() const {}

    reference at() {}

    value_type back() const {}

    reference back() {}

    value_type front() const {}

    reference front() {}

    void push_back(const_reference value) {}

    void push_front(const_reference value) {}

    void pop_back() {}

    void pop_front() {}

    // Completer cette classe générique pour qu'elle passe le codecheck


};

#endif /* ArrayDeque_hpp */
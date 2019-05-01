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

    ~ArrayDeque() {

        // destroy object
        ::operator delete(this->buffer);
    }

    size_type size() const {
        return taille;
    }

    size_type capacity() const {
        return capacite;
    }

    bool empty() const {
        return !taille;
    }

    const_reference at(size_type logical_i) const {
        return *(buffer + physical_i(logical_i));
    }

    reference at(size_type logical_i) {
        return *(buffer + physical_i(logical_i));
    }

    value_type back() const {}

    reference back() {}

    value_type front() const {}

    reference front() {}

    void push_back(const_reference value) {
        if(taille >= this->capacity()){
            increaseCapacity();
        }

        this->at(physical_i(taille)) = value;


        ++taille;
    }

    void push_front(const_reference value) {
    }

    void pop_back() {}

    void pop_front() {}

    // Completer cette classe générique pour qu'elle passe le codecheck


private:

    /**
     * @brief convert a logical index into a physical one
     *
     * @param logical_i to convert
     * @return the converted index
     */
    size_type physical_i(size_type logical_i) const {
        size_type physicalIndex = (debut + logical_i) % capacity();

        return (physicalIndex >= 0) ? physicalIndex : physicalIndex + capacity();
    }

    void increaseCapacity(){
        ArrayDeque tmpArrayDeque(capacite + 1);

        if(!empty()) {

            for (size_type i = 0; i < this->size(); ++i) {
                tmpArrayDeque.push_back(*(buffer + physical_i(i)));
            }
        }

        std::swap(this->buffer, tmpArrayDeque.buffer);

        ++capacite;
    }



};

#endif /* ArrayDeque_hpp */
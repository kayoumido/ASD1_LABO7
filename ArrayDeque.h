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

    ArrayDeque(size_type cap = 0) : debut(0), taille(0), capacite(cap){
        buffer = capacite != 0 ?
                 (pointer) ::operator new(capacite * sizeof(value_type))
                               : nullptr;
    }

    ArrayDeque(const ArrayDeque &ad): ArrayDeque(ad.capacite) {

        debut = ad.debut;

        for(size_type i = 0; i < ad.size(); ++i){
            push_back(ad.at(i));
        }

    }

    ArrayDeque& operator=(const ArrayDeque& rhs){
        // Prevent for case a1 = a1 => check memory address if same just return the current this object
        if(&rhs != this) {
            ArrayDeque tmpAD = rhs;
            std::swap(buffer, tmpAD.buffer);
        }

        return *this;
    }

    ~ArrayDeque() {

        while(!empty()){
            pop_front();
        }

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

    const_reference back() const {
        return *(buffer + physical_i(size() - 1));
    }

    reference back() {
        return *(buffer + physical_i(size() - 1));
    }

    const_reference front() const {
        return at(0);
    }

    reference front() {
        return at(0);
    }

    void push_back(const_reference value) {
        if(taille >= capacity()){
            increaseCapacity();
        }

        new (buffer + physical_i(taille)) value_type(value);

        ++taille;
    }

    void push_back(rvalue_reference rvalue) {
        if(taille >= capacity()){
            increaseCapacity();
        }

        new (buffer + physical_i(taille)) value_type(std::move(rvalue));

        ++taille;
    }

    void push_front(const_reference value) {
        if(taille >= capacity()){
            increaseCapacity();
        }

        new (buffer + physical_i(capacity() - 1)) value_type(value);
        debut = physical_i(capacity() - 1);

        ++taille;
    }

    void push_front(rvalue_reference rvalue) {
        if(taille >= capacity()){
            increaseCapacity();
        }

        new (buffer + physical_i(capacity() - 1)) value_type(std::move(rvalue));
        debut = physical_i(capacity() - 1);

        ++taille;
    }



    void pop_back() {
        if(!empty()){
            (buffer + physical_i(size() - 1))->~value_type();
            --taille;
        }
    }

    void pop_front() {
        if(!empty()) {
            (buffer + physical_i(0))->~value_type();
            debut = physical_i(1);
            --taille;
        }
    }

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
        size_type newCapacity = (capacity() == 0) ? 1 : 2 * capacity();
        ArrayDeque tmpArrayDeque(newCapacity);
        tmpArrayDeque.debut = debut;

        if(!empty()) {
            for (size_type i = 0; i < this->size(); ++i) {
                tmpArrayDeque.push_back((rvalue_reference)at(i));
            }
        }

        std::swap(this->buffer, tmpArrayDeque.buffer);
        std::swap(this->capacite, tmpArrayDeque.capacite);
    }


};

#endif /* ArrayDeque_hpp */
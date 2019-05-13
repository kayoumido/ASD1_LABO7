/**
-----------------------------------------------------------------------------------
Laboratoire : 07
\file       ArrayDeque.h
\author     Loïc Dessaules, Doran Kayoumi, Gabrielle Thurnherr
\date       13/05/2019
\brief      Create a whole ArrayDeque class with dynamic allocation
Compilateur MinGW-gcc 6.3.0
**/

#ifndef ArrayDeque_h
#define ArrayDeque_h

#include <algorithm>
#include <stdexcept>


template<class T>
class ArrayDeque {
public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using rvalue_reference = T &&;
    using pointer = T *;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    pointer buffer;
    size_type capacite;
    size_type debut;
    size_type taille;

public:

    /**
     * @brief Constructs an Array Deque using with a given capacity
     *
     * @param cap wanted size for the array deque
     */
    ArrayDeque(size_type cap = 0) : debut(0), taille(0), capacite(cap) {
        buffer = capacite != 0 ?
                 (pointer) ::operator new(capacite * sizeof(value_type))
                               : nullptr;
    }

    /**
     * @brief Constructs an Array Deque using an other one
     * @details So we can have a strong guarantee, we use the default constructor
     *          (see ArrayDeque(size_type cap = 0)) to init an empty Array Deque. With this
     *          method, if there are any issues during the copy of the content, the newly created
     *          object will be discarded.
     *
     * @param ad Array Deque used for construction
     */
    ArrayDeque(const ArrayDeque &ad) : ArrayDeque(ad.capacite) {

        debut = ad.debut;

        for (size_type i = 0; i < ad.size(); ++i) {
            push_back(ad.at(i));
        }

    }

    /**
     * @brief Assigns new contents to the array deque, replacing it's current contents
     *
     * @param rhs Array Deque of the same type
     * @return *this
     */
    ArrayDeque &operator=(const ArrayDeque &rhs) {
        // Prevent for case a1 = a1 => check memory address if same just return the current this object
        if (&rhs != this) {
            ArrayDeque tmpAD = rhs;
            std::swap(buffer, tmpAD.buffer);
        }

        return *this;
    }

    /**
     * @brief Destroys the array deque and frees memory
     */
    ~ArrayDeque() {

        while (!empty()) {
            pop_front();
        }

        ::operator delete(this->buffer);
    }

    /**
     * @brief get the size of the array deque
     * @return the size of the current array deque
     */
    size_type size() const {
        return taille;
    }

    /**
     * @brief get the capacity of the array deque
     * @return the capacity of the current array deque
     */
    size_type capacity() const {
        return capacite;
    }

    /**
     * @brief check if the array deque is empty
     * @return true if it's empty otherwise false
     */
    bool empty() const {
        return !taille;
    }

    /**
     * @brief get an element from the array deque at a given position
     * @param logical_i position of the element to get
     *
     * @return const reference (i.e. read only) of the element at the given position
     */
    const_reference at(size_type logical_i) const {
        return *(buffer + physical_i(logical_i));
    }

    /**
     * @brief get an element from the array deque at a given position
     * @param logical_i position of the element to get
     *
     * @return reference (i.e. read & write) of the element at the given position
     */
    reference at(size_type logical_i) {
        return *(buffer + physical_i(logical_i));
    }

    /**
     * @brief get the last element of the array deque
     *
     * @return const reference (read only) of the last element of the array deque
     */
    const_reference back() const {
        return *(buffer + physical_i(size() - 1));
    }

    /**
     * @brief get the last element of the array deque
     *
     * @return const reference (read & write) of the last element of the array deque
     */
    reference back() {
        return *(buffer + physical_i(size() - 1));
    }

    /**
     * @brief Return the first item of the array deque
     * @note    this version of the method was created
     *          for const ArrayDeque and doesn't allow to do :
     *          ArrayDeque.front() = <something>
     * @return The first item of the arrayDeque
     */
    const_reference front() const {
        return at(0);
    }

    /**
     * @brief Return the first item of the array deque
     * @note    this version of the method was created
     *          for non const ArrayDeque and allows to do :
     *          ArrayDeque.front() = <something>
     * @return The first item of the arrayDeque
     */
    reference front() {
        return at(0);
    }

    /**
     * @brief add a new value at the end of the ArrayDeque
     * @param value The value to add
     */
    void push_back(const_reference value) {
        if (taille >= capacity()) {
            increaseCapacity();
        }

        new(buffer + physical_i(taille)) value_type(value);

        ++taille;
    }

    /**
     * @brief add a new value at the end of the ArrayDeque
     * @param rvalue The value to add as an rvalue_reference to be able to move the item in place of a copy
     */
    void push_back(rvalue_reference rvalue) {
        if (taille >= capacity()) {
            increaseCapacity();
        }

        // Construct our new value
        new (buffer + physical_i(taille)) value_type(std::move(rvalue));

        ++taille;
    }

    /**
     * @brief add a new value at the beginning of the ArrayDeque
     *
     * @param value The value to add
     */
    void push_front(const_reference value) {
        if (taille >= capacity()) {
            increaseCapacity();
        }

        // Construct our new value
        new (buffer + physical_i(capacity() - 1)) value_type(value);
        debut = physical_i(capacity() - 1);

        ++taille;
    }

    /**
     * @brief add a new value at the beginning of the ArrayDeque
     *
     * @param rvalue The value to add as an rvalue_reference to be able to move the item in place of a copy
     */
    void push_front(rvalue_reference rvalue) {
        if (taille >= capacity()) {
            increaseCapacity();
        }

        // Construct our new value
        new (buffer + physical_i(capacity() - 1)) value_type(std::move(rvalue));
        debut = physical_i(capacity() - 1);

        ++taille;
    }


    /**
     * Remove the last item in the ArrayDeque
     */
    void pop_back() {
        if(!empty()){
            // Destroy the current value (call his destructor)
            (buffer + physical_i(size() - 1))->~value_type();
            --taille;
        }
    }

    /**
     * Remove the first item in the ArrayDeque
     */
    void pop_front() {
        if(!empty()) {
            // Destroy the current value (call his destructor)
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


    /**
     * @brief   Increase the capacity of the current buffer.
     *          The capacity is increased by doubling the capacity (1 if no capacity) of the current buffer.
     * @details The data stored in the current buffer is copied into a temp ArrayDeque and swap tmp (buffer and capacity)
     *          with this (buffer and capacity). We use rvalue_Reference at to do that, to be able to move the value and not copy
     *          This way => strong warranty respected
     */
    void increaseCapacity(){
        size_type newCapacity = (capacity() == 0) ? 1 : 2 * capacity();
        ArrayDeque tmpArrayDeque(newCapacity);
        tmpArrayDeque.debut = debut;

        if (!empty()) {
            for (size_type i = 0; i < this->size(); ++i) {
                tmpArrayDeque.push_back((rvalue_reference) at(i));
            }
        }

        std::swap(this->buffer, tmpArrayDeque.buffer);
        std::swap(this->capacite, tmpArrayDeque.capacite);
    }


};

#endif /* ArrayDeque_hpp */
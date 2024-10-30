#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

#include <iostream>
#include <new>       // Needed for bad_alloc exception
#include <cstdlib>   // Needed for the exit function
#include <exception>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <algorithm>
#include "TypeTraits.h"
#include <mutex>

//#define DEBUG

template <typename T>
class SimpleVector {
private:
    T* array; // Pointer to the array
    unsigned int count; // Number of elements in the array
    unsigned int capacity; // Capacity of the array

    void resize(); // Resize the array to 2 * capacity
    void resize(unsigned int newCapacity); // Resize the array to the specified capacity
    void ensureCapacity(); // Ensure that the array has enough capacity to add a new element
    int calculateNewCapacity(); // Calculate the new capacity of the array
    mutable std::mutex mtx; // Mutex for synchronization

public:
    class SimpleVectorIterator; // Forward declaration of the SimpleVectorIterator class

    SimpleVector(); // Default constructor
    
    SimpleVector(unsigned int initialCapacity); // Constructor with initial capacity
    
    SimpleVector(const SimpleVector& other); // Copy constructor
    SimpleVector(std::initializer_list<T> initList); // Constructor with initializer list

    ~SimpleVector(); // Destructor
    void releaseMemory(); // Release the memory used by the array
    bool shrinkToFit(); // Shrink the capacity of the array to match the count of elements
    void clear(); // Clear the array by setting all elements to their default value and resetting the count
    void push_back(const T& item); // Add an element to the array
    void put(const T& item); // Add an element to the array
    template<typename... Args>
    void bulk_add(Args&&... args); // Add multiple elements to the array
    void emplace_back(); // Add an element to the array using emplace
    void emplace_back(const T& value); // Add an element to the array using emplace
    T& back(); // Get the last element in the array
    T& front(); // Get the first element in the array
    void remove(const T& item); // Remove an element from the array
    T& operator[](unsigned int index); // Overload the subscript operator for non-const objects
    const T& operator[](unsigned int index) const; // Overload the subscript operator for const objects
    unsigned int size() const; // Get the number of elements in the array
    unsigned int elements() const; // Get the number of elements in the array
    T& get(unsigned int index); // Get the element at the specified index
    bool isEmpty() const; // Check if the array is empty
    int indexOf(const T& element); // Get the index of the specified element
    bool contains(const T& element); // Check if the array contains the specified element
    bool contains(const T& element) const; // Check if the array contains the specified element

    SimpleVectorIterator begin(); // Get an iterator pointing to the first element in the array
    SimpleVectorIterator end(); // Get an iterator pointing to one past the last element in the array

    const SimpleVectorIterator cbegin(); // Get a const iterator pointing to the first element in the array.
    const SimpleVectorIterator cend(); // Get a const iterator pointing to one past the last element in the array.
    
    typedef SimpleVectorIterator iterator; // Define the iterator type
};

//==============================================================================|

#ifndef SIMPLEVECTOREXCEPTION
#define SIMPLEVECTOREXCEPTION
class SimpleVectorException : public std::exception {
public:
    explicit SimpleVectorException(const std::string& message) : msg_(message) {}
    virtual const char* what() const noexcept {
        return msg_.c_str();
    }
private:
    std::string msg_;
};
#endif

#ifndef INDEXOUTOFBOUNDSEXCEPTION
#define INDEXOUTOFBOUNDSEXCEPTION
class IndexOutOfBoundsException : public std::exception {
public:
    explicit IndexOutOfBoundsException(const std::string& message) : msg_(message) {}
    virtual const char* what() const noexcept {
        return msg_.c_str();
    }
private:
    std::string msg_;
};
#endif

//==============================================================================|

/**
 * @brief Constructs a new SimpleVector object.
 * 
 * @details This constructor creates a new SimpleVector object with an initial capacity of 4.
 * 
 * @tparam T The type of the elements in the SimpleVector.
 * 
 */
template <typename T>
SimpleVector<T>::SimpleVector() : array(new T[4]), count(0), capacity(4) {
    #ifdef DEBUG
    std::cout << "Created array with size: 4" << std::endl;
    #endif
}

/**
 * @brief Constructs a new SimpleVector object using an initializer list. 
 * 
 * @details This constructor creates a new SimpleVector object and initializes it with the values from the initializer list.
 * The size of the SimpleVector object will be equal to the size of the initializer list, and the capacity will be set to the size of the initializer list.
 * 
 * @tparam T The type of the elements in the SimpleVector.
 * @param initList The initializer list used to initialize the SimpleVector object.
 * 
 * @throw SimpleVectorException if the initializer list is empty.
 */
template <typename T>
SimpleVector<T>::SimpleVector(std::initializer_list<T> initList) {
    if(initList.size() == 0) {
        throw SimpleVectorException("Initializer list must not be empty.");
    }
    if(initList.size() > std::numeric_limits<unsigned int>::max()) {
        throw SimpleVectorException("Initializer list is too large for this SimpleVector implementation.");
    }
    array = new T[initList.size()];
    #ifdef DEBUG
    std::cout << "Created array with size: " << initList.size() << std::endl;
    #endif
    capacity = static_cast<unsigned int>(initList.size());
    count = 0;
    for (const T& value : initList) {
        if(count == capacity) {
            resize(2 * capacity);
        }
        array[count++] = value;
        #ifdef DEBUG
        std::cout << "Added element: " << value << std::endl;
        #endif
    }
}

/**
 * @brief Constructs a new SimpleVector object with the specified initial capacity.
 * 
 * @details This constructor creates a new SimpleVector object with the specified initial capacity.
 * 
 * @tparam T The type of the elements in the SimpleVector.
 * @param initialCapacity The initial capacity of the SimpleVector object.
 * 
 * @throw SimpleVectorException if the initial capacity is 0.
 */
template <typename T>
SimpleVector<T>::SimpleVector(unsigned int initialCapacity) : array(new T[initialCapacity]), count(0), capacity(initialCapacity) {    
    if (initialCapacity == 0) {
        throw SimpleVectorException("Initial capacity must be greater than 0.");
    }

    array = new T[initialCapacity];
    #ifdef DEBUG
    std::cout << "Created array with size: " << initialCapacity << std::endl;
    #endif

    count = 0;
    capacity = initialCapacity;

}

/**
 * @brief Constructs a new SimpleVector object by copying another SimpleVector object.
 * 
 * @details This constructor creates a new SimpleVector object by copying the elements from another SimpleVector object.
 * 
 * @tparam T The type of the elements in the SimpleVector.
 * @param other The SimpleVector object to copy.
 * 
 * @throw SimpleVectorException if the other SimpleVector object is empty.
 */
template <typename T>
SimpleVector<T>::SimpleVector(const SimpleVector& other) : capacity(other.capacity), count(other.count) {
    if(other.count == 0) {
        throw SimpleVectorException("Cannot copy from an empty vector.");
    }
    
    array = new T[capacity];
    
    for (unsigned int i = 0; i < count; i++) {
        array[i] = other.array[i];
    }
    
    #ifdef DEBUG
    std::cout << "Copied array with size: " << capacity << std::endl;
    #endif
}

/**
 * @brief Destroys the SimpleVector object.
 * 
 * @details This destructor releases the memory used by the SimpleVector object.
 * 
 * @tparam T The type of the elements in the SimpleVector.
 */
template <typename T>
SimpleVector<T>::~SimpleVector() {
    delete[] array;
    #ifdef DEBUG
    std::cout << "Deleted array" << std::endl;
    #endif
}

/**
 * @brief Ensure that the array has enough capacity to add a new element by resizing it if necessary.
 * 
 * @details This method checks if the array has enough capacity to add a new element.
 * If the array is full, it resizes the array to twice its current capacity.
 * 
 */
template <typename T>
void SimpleVector<T>::ensureCapacity() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety
    if (count == capacity) {
        resize(2 * capacity);
    }
    #ifdef DEBUG
    std::cout << "Ensured capacity" << std::endl;
    #endif
}

/**
 * @brief Calculate the new capacity of the array.
 * 
 * @details This method calculates the new capacity of the array when resizing.
 * 
 * @return The new capacity of the array.
 */
template <typename T>
int SimpleVector<T>::calculateNewCapacity() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    return 2 * capacity;
}

/**
 * @brief Resize the array to twice its current capacity.
 * 
 * @details This method resizes the array to twice its current capacity.
 * 
 */
template <typename T>
void SimpleVector<T>::resize() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Resizing array" << std::endl;
    #endif

    int newCapacity = calculateNewCapacity();
    if(newCapacity <= capacity) {
        throw SimpleVectorException("New capacity must be greater than current capacity.");
    }

    T* newArray = new T[newCapacity];
    if(!newArray) {
        throw SimpleVectorException("Memory allocation failed. Cannot resize array.");
    }

    for(unsigned int i = 0; i < count; i++) {
        newArray[i] = array[i];
    }
    #ifdef DEBUG
    std::cout << "Copied elements to new array" << std::endl;
    #endif
    delete[] array;
    #ifdef DEBUG
    std::cout << "Deleted old array" << std::endl;
    #endif

    array = newArray;
    #ifdef DEBUG
    std::cout << "Set array to new array" << std::endl;
    #endif
    capacity = newCapacity;
    #ifdef DEBUG
    std::cout << "Set capacity to new capacity" << std::endl;
    #endif
    
}


/**
 * @brief Resize the array to the specified capacity.
 * 
 * @details This method resizes the array to the specified capacity.
 * 
 * @param newCapacity The new capacity of the array.
 * 
 * @throw SimpleVectorException if the new capacity is the same as the current capacity.
 * @throw SimpleVectorException if the new capacity is less than the current capacity.
 * @throw SimpleVectorException if memory allocation fails.
 */
template <typename T>  
void SimpleVector<T>:: resize(unsigned int newCapacity) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Resizing array to new capacity: " << newCapacity << std::endl;
    #endif
    if(newCapacity == capacity) {
        throw SimpleVectorException("New capacity must be different from current capacity.");
    }

    if(newCapacity > std::numeric_limits<unsigned int>::max()) {
        throw SimpleVectorException("New capacity is too large for this SimpleVector implementation.");
    }

    T* newArray = new T[newCapacity];
    if (!newArray) {
        throw SimpleVectorException("Memory allocation failed.");
    }
    #ifdef DEBUG
    std::cout << "Created new array with size: " << newCapacity << std::endl;
    #endif
    int elementsToCopy = (newCapacity < count) ? newCapacity : count;
    for (int i = 0; i < elementsToCopy; ++i) {
        newArray[i] = array[i];
    }
    #ifdef DEBUG
    std::cout << "Copied elements to new array" << std::endl;
    #endif
    delete[] array;
    #ifdef DEBUG
    std::cout << "Deleted old array" << std::endl;
    #endif

    array = newArray;
    #ifdef DEBUG
    std::cout << "Set array to new array" << std::endl;
    #endif
    capacity = newCapacity;
    #ifdef DEBUG
    std::cout << "Set capacity to new capacity" << std::endl;
    #endif
    count = elementsToCopy;
    #ifdef DEBUG
    std::cout << "Set count to elements copied" << std::endl;
    #endif
}

/**
 * @brief Release the memory used by the array.
 * 
 * @details This method releases the memory used by the array.
 * 
 */
template <typename T>
void SimpleVector<T>::releaseMemory() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Releasing memory" << std::endl;
    #endif
    delete[] array;
    #ifdef DEBUG
    std::cout << "Deleted array" << std::endl;
    #endif
    array = nullptr;
    #ifdef DEBUG
    std::cout << "Set array to nullptr" << std::endl;
    #endif
    capacity = 0;
    count = 0;.
    #ifdef DEBUG
    std::cout << "Set capacity and count to 0" << std::endl;
    #endif
}

/**
 * @brief Shrink the capacity of the array to match the count of elements.
 * 
 * @details This method shrinks the capacity of the array to match the count of elements.
 * 
 * @return True if the memory was successfully reserved, false otherwise.
 */
template <typename T>
bool SimpleVector<T>::shrinkToFit() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Shrinking to fit" << std::endl;
    #endif
    if (count < capacity) {
        resize(count);
        return true;
    }
    return false;
}

/**
 * @brief Clear the array by setting all elements to their default value and resetting the count.
 * 
 * @details This method clears the array by setting all elements to their default value and resetting the count.
 */
template <typename T>
void SimpleVector<T>::clear() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety
    #ifdef DEBUG
    std::cout << "Clearing array" << std::endl;
    #endif
    resize(1);
    count = 0;
}

/**
 * @brief Add an element to the array.
 * 
 * @details This method adds an element to the array.
 * 
 * @param item The item to be added to the array.
 */
template <typename T>
void SimpleVector<T>::push_back(const T& item) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Pushing back element: " << item << std::endl;
    #endif
    if(count == capacity) {
        resize(2 * capacity);
    }
    array[count++] = item;
}

/**
 * @brief Add an element to the array.
 * 
 * @details This method adds an element to the array.
 * 
 * @param item The item to be added to the array.
 */
template <typename T>
void SimpleVector<T>::put(const T& item) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Adding element: " << item << std::endl;
    #endif
    if(count == capacity) {
        resize(2 * capacity);
    }
    array[count++] = item;
}

/**
 * @brief Add multiple elements to the array.
 * 
 * @details This method adds multiple elements to the array.
 * 
 * @tparam Args The types of the elements to be added to the array.
 * @tparam T The type of the elements in the array.
 * 
 * @param args The elements to be added to the array.
 */
template <typename T>
template<typename... Args>
void SimpleVector<T>::bulk_add(Args&&... args) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

#ifdef DEBUG
    std::cout << "Adding multiple elements" << std::endl;
#endif

    constexpr size_t argsCount = sizeof...(args);
    if (argsCount == 0) {
        throw SimpleVectorException("No elements to add.");
    }

    if (argsCount > std::numeric_limits<unsigned int>::max()) {
        throw SimpleVectorException("Too many elements to add.");
    }

    unsigned int newCount = count + static_cast<unsigned int>(argsCount);
    if (newCount < count) {  // Check for overflow
        throw SimpleVectorException("Adding these elements would cause an overflow.");
    }

    if (newCount > capacity) {
        size_t newCapacity = std::max(static_cast<size_t>(capacity) * 2, static_cast<size_t>(newCount));
        if (newCapacity > std::numeric_limits<unsigned int>::max()) {
            newCapacity = std::numeric_limits<unsigned int>::max();
        }
        resize(static_cast<unsigned int>(newCapacity));
    }

    int dummy[] = { (put(std::forward<Args>(args)), 0)... };
    (void)dummy; // To avoid unused variable warning
}

/**
 * @brief Add an element to the array using emplace.
 * 
 * @details This method adds an element to the array using emplace.
 */
template <typename T>
void SimpleVector<T>::emplace_back() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Emplacing element" << std::endl;
    #endif
    ensureCapacity();
    try{
        new (array + count) T();
        count++;
    } catch (std::bad_alloc& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
    
}

/**
 * @brief Add an element to the array using emplace.
 * 
 * @details This method adds an element to the array using emplace.
 * 
 * @note Definition: Emplace is a member function in C++ STL. It is used to insert a new element into the container.
 * @param value The value of the element to be added to the array.
 */
template <typename T>
void SimpleVector<T>::emplace_back(const T& value) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Emplacing element: " << value << std::endl;
    #endif
    ensureCapacity();
    try{
        new (array + count) T(value);
        count++;
    } catch (std::bad_alloc& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

/**
 * @brief Get the last element in the array.
 * 
 * @details This method gets the last element in the array.
 * 
 * @return The last element in the array.
 */
template <typename T>
T& SimpleVector<T>::back() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Getting last element" << std::endl;
    #endif
    if(count == 0) {
        throw IndexOutOfBoundsException("Array is empty.");
    }
    return array[count - 1];
}

/**
 * @brief Get the first element in the array.
 * 
 * @details This method gets the first element in the array.
 * 
 * @return The first element in the array.
 */
template <typename T>
T& SimpleVector<T>::front() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    #ifdef DEBUG
    std::cout << "Getting first element" << std::endl;
    #endif
    if(count == 0) {
        throw IndexOutOfBoundsException("Array is empty.");
    }
    return array[0];
}

/**
 * @brief Remove an element from the array.
 * 
 * @details This method removes an element from the array.
 * 
 * @param item The item to be removed from the array.
 */
template <typename T>
void SimpleVector<T>::remove(const T& item) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    unsigned int index = 0;
    for (unsigned int i = 0; i < count; i++) {
        if (array[i] == item) {
            // Element found, skip it
            continue;
        }
        array[index++] = array[i];
    }
    count = index;
}

/**
 * @brief Overload the subscript operator for non-const objects.
 * 
 * @details This method overloads the subscript operator for non-const objects.
 * 
 * @param index The index of the element to be accessed.
 * @return The element at the specified index.
 */
template <typename T>
T& SimpleVector<T>::operator[](unsigned int index) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    static T dummy;
    if (index >= count || index < 0) {
        throw IndexOutOfBoundsException("Error: Index out of bounds.");
    }
    return array[index];
}

/**
 * @brief Overload the subscript operator for const objects.
 * 
 * @details This method overloads the subscript operator for const objects.
 * 
 * @param index The index of the element to be accessed.
 * @return The element at the specified index.
 */
template <typename T>
const T& SimpleVector<T>::operator[](unsigned int index) const {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    if (index >= count || index < 0) {
        throw IndexOutOfBoundsException("Error: Index out of bounds. "); // You can handle this error differently if needed
    }
    return array[index];
}

/**
 * @brief Get the capacity of the Array.
 * 
 * @details This method gets the capacity of the array.
 * 
 * @return The capacity of the array.
 */
template <typename T>
unsigned int SimpleVector<T>::size() const {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    if(this -> capacity == 0){
        return 0;
    }
    return this -> capacity;
}

/**
 * @brief Get the number of elements in the array.
 * 
 * @details This method gets the number of elements in the array.
 * 
 * @return The number of elements in the array.
 */
template <typename T>
unsigned int SimpleVector<T>::elements() const {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    if(this -> count == 0){
        return 0;
    }
    return this -> count;
}

/**
 * @brief Get the element at the specified index.
 * 
 * @details This method gets the element at the specified index.
 * 
 * @param index The index of the element to be accessed.
 * 
 * @throw IndexOutOfBoundsException if the index is out of bounds.
 * @return Reference to the element at the specified index.
 */
template <typename T>
T& SimpleVector<T>::get(unsigned int index) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    if (index >= count) {
        throw IndexOutOfBoundsException("Error: Index out of bounds: index cannot be greater than count.");
    }
    if (index < 0) {
        throw IndexOutOfBoundsException("Error: Index out of bounds: index cannot be negative.");
    }
    return array[index];
}

/**
 * @brief Check if the array is empty.
 * 
 * @details This method checks if the array is empty.
 * 
 * @return True if the array is empty, false otherwise.
 */
template <typename T>
bool SimpleVector<T>::isEmpty() const {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    return count == 0;
}

/**
 * @brief Get the index of the specified element.
 * 
 * @details This method gets the index of the specified element.
 * 
 * @param element The element to get the index of.
 * 
 * @return The index of the specified element.
 */
template <typename T>
int SimpleVector<T>::indexOf(const T& element) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    for (unsigned int i = 0; i < count; i++) {
        if (array[i] == element) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief The SimpleVectorIterator class.
 *
 * @details This class represents an iterator for the SimpleVector class.
 *
 * @tparam T The type of the elements in the SimpleVector.
 */
template <typename T>
class SimpleVector<T>::SimpleVectorIterator {
private:
    T* current;
    T* end;

public:
    SimpleVectorIterator(T* start, T* stop) : current(start), end(stop) {}

    /**
     * @brief Check if the iterator has reached the end of the vector
     * @return True if the iterator has reached the end of the vector, false otherwise
    */
    bool hasNext() {
        return current != end;
    }

    /**
     * @brief Get the next element in the vector
     * @return The next element in the vector
    */
    T& next() {
        return *current++;
    }

    // The iterator needs to be compatible with STL-style iteration
    /**
     * @brief Overload the prefix increment operator
    */
    SimpleVectorIterator& operator++() {
        ++current;
        return *this;
    }

    /**
     * @brief Overload the postfix increment operator
    */
    bool operator!=(const SimpleVectorIterator& other) const {
        return current != other.current;
    }
    /**
     * @brief Overload the dereference operator
    */
    T& operator*() {
        return *current;
    }

};




/**
 * @brief Get an iterator pointing to the first element in the array.
 * 
 * @details This method gets an iterator pointing to the first element in the array.
 * 
 * @return An iterator pointing to the first element in the array.
 */
template <typename T>
typename SimpleVector<T>::SimpleVectorIterator SimpleVector<T>::begin() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    return SimpleVectorIterator(array, array + count);
}

/**
 * @brief Get an iterator pointing to one past the last element in the array.
 * 
 * @details This method gets an iterator pointing to one past the last element in the array.
 * 
 * @return An iterator pointing to one past the last element in the array.
 */
template <typename T>
typename SimpleVector<T>::SimpleVectorIterator SimpleVector<T>::end() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    return SimpleVectorIterator(array + count, array + count);
}

/**
 * @brief Get a const iterator pointing to the first element in the array.
 * 
 * @details This method gets a const iterator pointing to the first element in the array.
 * 
 * @return A const iterator pointing to the first element in the array.
 */
template <typename T>
const typename SimpleVector<T>::SimpleVectorIterator SimpleVector<T>::cbegin() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    return SimpleVectorIterator(array, array + count);
}

/**
 * @brief Get a const iterator pointing to one past the last element in the array.
 * 
 * @details This method gets a const iterator pointing to one past the last element in the array.
 * 
 * @return A const iterator pointing to one past the last element in the array.
 */
template <typename T>
const typename SimpleVector<T>::SimpleVectorIterator SimpleVector<T>::cend() {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    return SimpleVectorIterator(array + count, array + count);
}


template <typename T>
bool SimpleVector<T>::contains(const T& element) {
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    for (unsigned int i = 0; i < count; i++) {
        if (array[i] == element) {
            return true;
        }
    }
    return false;
}

template <typename T>
bool SimpleVector<T>::contains(const T& element) const{
    std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safety

    for (unsigned int i = 0; i < count; i++) {
        if (array[i] == element) {
            return true;
        }
    }
    return false;
}







#endif // SIMPLEVECTOR_H
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <iostream>
#include <string>
#include <exception>
#include <vector>


template <typename T>
class ArrayList {
public:
    enum SizeType { FIXED, DYNAMIC }; // Size type
    enum SortAlgorithm { BUBBLE_SORT, QUICK_SORT, MERGE_SORT }; // Sorting algorithms
    //Function declarations
    ArrayList(SizeType type  = DYNAMIC, int initialSize = 8);

    ArrayList(const ArrayList<T>& list);

    ~ArrayList();
    void clear();
    void add(T item);
    bool addAll(const ArrayList<T>& other);
    bool addAll(const T* other, int length);
    bool insert(int index, T item);
    bool insertAll(int index, const ArrayList<T>& other);
    bool insertAll(int index, const T* other, int length); 
    bool remove(int index);
    bool removeIf(bool (*predicate)(T)); // You Can use the Predicates Class
    bool removeItem(T item); 
    void removeRange(int fromIndex, int toIndex);
    bool retainAll(const ArrayList<T>& other);
    T get(int index) const;
    std::string getAsString(int index) const;
    std::string tostring(const T& value) ;
    ArrayList& operator =(const ArrayList<T>& list);
    T& operator [] (int index);
    bool contains(T item) const;
    int indexOf(T item) const;
    int capacity() const;
    int size() const;
    bool isEmpty() const;
    bool set(int index, T item);
    void replaceAll(T (*operatorFunction)(T)); // You can use the Operator Class
    void forEach(void (*consumer)(T)) const;
    T* toArray(T* outputArray) const;
    ArrayList<T>* sublist(int fromIndex, int toIndex) const;
    ArrayList<T>* clone() const;
    void ensureCapacity(int minCapacity);
    void trimToSize();
    void mergeSort(ArrayList<T>& list, size_t l, size_t r);
    void quickSort(bool (*comparator)(T, T));
    void bubbleSort(bool (*comparator)(T, T));     
    void setSortAlgorithm(SortAlgorithm algorithm = MERGE_SORT);
    SortAlgorithm getSortAlgorithm();
    void sort(bool (*comparator)(T, T));
    void sort(bool (*comparator)(T, T), SortAlgorithm algorithm); 
    T* begin() const;
    T* end() const;
    void reverse();
    bool exists(T item);

private:
    //Variable declarations
    
    T* array;
    SizeType sizeType;
    int arrayCapacity;
    int count;
    SortAlgorithm sortAlgorithm;
    bool DEBUG = false;
    static constexpr double RESIZE_FACTOR = 1.5;

    //Function declarations
    void resize();
    size_t partition(bool (*comparator)(T, T), size_t low, size_t high);
    void quickSortHelper(bool (*comparator)(T, T), size_t low, size_t high);
    void merge(ArrayList<T>& list, size_t l, size_t m, size_t r);
    void swap(T* a, T* b);
};


class ArrayListException : public std::exception {
public:
    explicit ArrayListException(const std::string& message) : msg_(message) {}
    virtual const char* what() const noexcept {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


class IndexOutOfBoundsException : public std::exception {
public:
    explicit IndexOutOfBoundsException(const std::string& message) : msg_(message) {}
    virtual const char* what() const noexcept {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


/**
     * @brief Constructs a new ArrayList.
     *
     * This constructor creates a new ArrayList with the specified size type, initial size, and debug flag.
     * The size type determines whether the size of the ArrayList is static or dynamic. The initial size specifies
     * the initial capacity of the ArrayList. The debug flag determines whether debug messages should be prsize_ted.
     *
     * @param type The size type of the ArrayList. This should be either DYNAMIC or STATIC.
     * @param initialSize The initial capacity of the ArrayList.
     * @param debug A flag indicating whether debug messages should be prsize_ted.
    */
    template <typename T>
    ArrayList<T>::ArrayList(SizeType type, int initialSize)
        : sizeType(type), arrayCapacity(initialSize), count(0) {
        array = new T[arrayCapacity];
        if(DEBUG){
            std::cout << "ArrayList created with initial size: " << initialSize << std::endl;
        }
    }

    /**
    * @brief COPY Constructor
    */    
    template <typename T>
    ArrayList<T>::ArrayList(const ArrayList<T>& list) 
        : sizeType(list.sizeType), arrayCapacity(list.arrayCapacity), count(list.count) {
        array = new T[arrayCapacity];
        if(DEBUG){
            std::cout << "ArrayList created with initial size: " << list.arrayCapacity << std::endl;
        }
        std::copy(list.array, list.array + list.count, array);
        if(DEBUG){
            std::cout << "ArrayList TYPE: " << list.sizeType << std::endl;
            std::cout << "ArrayList INITIAL SIZE: " << list.arrayCapacity << std::endl;
            std::cout << "ArrayList CAPACITY: " << list.arrayCapacity << std::endl;
            std::cout << "ArrayList COUNT: " << list.count << std::endl;
        }
    }

    /**
     * @brief Destroys the ArrayList.
     *
     * This destructor deletes the ArrayList and frees the memory allocated for it.
     * If the debug flag is set, it prsize_ts a message indicating that the ArrayList is being deleted.
    */
    template <typename T>
    ArrayList<T>::~ArrayList() {
        delete[] array;
        if(DEBUG){
            std::cout << "ArrayList destroyed" << std::endl;
        }
    }
   
   /**
     * @brief Adds an item to the ArrayList.
     *
     * This function adds the specified item to the ArrayList. If the size type of the ArrayList is DYNAMIC and the ArrayList is full,
     * it resizes the ArrayList to accommodate the new item. If the ArrayList is not full, it adds the item to the ArrayList.
     * If the ArrayList is full and its size type is STATIC, it prsize_ts an error message and does not add the item.
     * (Only if the debug flag is set to true)
     * @param item The item to add to the ArrayList.
    */
    template <typename T>
    void ArrayList<T>::add(T item) {
        if(sizeType == FIXED && count >= arrayCapacity){
            throw ArrayListException("Error: Cannot add item to full ArrayList if Mode is FIXED");          
        }
        
        // Calculate the load factor
        float loadFactor = (float)count / arrayCapacity;
        if(DEBUG){
            std::cout << "Load Factor: " << loadFactor << std::endl;
        }
        // Resize the array if the load factor is greater than or equal to 0.8
        if (sizeType == DYNAMIC && loadFactor >= 0.8) {
            if(DEBUG){
                std::cout << "Resizing ArrayList" << std::endl;
            }
            resize();
        }
        // Add the item to the array
        if (count < arrayCapacity) {
            array[count++] = item;
            if(DEBUG){
                std::cout << "Item added to ArrayList at index" << count << std::endl;
            }
        }
    }

    /**
     * @brief Adds all items from another ArrayList to this ArrayList.
     *
     * This function adds all items from the specified ArrayList to this ArrayList. If the size type of this ArrayList is DYNAMIC and 
     * the total count of items in both ArrayLists exceeds the capacity of this ArrayList, it resizes this ArrayList to accommodate the new items.
     * If the total count of items in both ArrayLists does not exceed the capacity of this ArrayList, it adds the items from the other ArrayList to this ArrayList.
     * If the total count of items in both ArrayLists exceeds the capacity of this ArrayList and its size type is STATIC, it prsize_ts an error message and does not add the items.
     * (only if debug flag is set to true)
     * @param other The ArrayList whose items should be added to this ArrayList.
     * @return true if the items were added successfully, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::addAll(const ArrayList<T>& other) {
        if (sizeType == DYNAMIC && count + other.count > arrayCapacity) {
            if(DEBUG){
                std::cout << "Resizing ArrayList from " << arrayCapacity << " to " << (arrayCapacity * 2) << std::endl;
            }
            resize();
        }
        if (count + other.count <= arrayCapacity) {
            if(DEBUG){
                std::cout << "Adding " << other.count << " elements to ArrayList" << std::endl;
            }
            std::copy(other.array, other.array + other.count, array + count);
            count += other.count;
            if(DEBUG){
                std::cout << "New count: " << count << std::endl;
            }
            return true;
        }
        if(DEBUG){
            std::cout << "Failed to add elements: not enough capacity" << std::endl;
        }
        return false;
    }

    /**
     * @brief Adds all items from an array to this ArrayList.
     *
     * This function adds all items from the specified array to this ArrayList. If the size type of this ArrayList is DYNAMIC and 
     * the total count of items in the ArrayList and the array exceeds the capacity of this ArrayList, it resizes this ArrayList to accommodate the new items.
     * If the total count of items in the ArrayList and the array does not exceed the capacity of this ArrayList, it adds the items from the array to this ArrayList.
     * If the total count of items in the ArrayList and the array exceeds the capacity of this ArrayList and its size type is STATIC, it prsize_ts an error message and does not add the items.
     *
     * @param other The array/List whose items should be added to this ArrayList.
     * @param length The length of the array/List.
     * @return true if the items were added successfully, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::addAll(const T* other, int length) {
        if (sizeType == DYNAMIC && count + length > arrayCapacity) {
            if(DEBUG){
                std::cout << "Resizing ArrayList from " << arrayCapacity << " to " << (arrayCapacity * 2) << std::endl;
            }
            resize();
        }
        if (count + length <= arrayCapacity) {
            if(DEBUG){
                std::cout << "Adding " << length << " elements to ArrayList" << std::endl;
            }
            std::copy(other, other + length, array + count);
            count += length;
            if(DEBUG){
                std::cout << "New count: " << count << std::endl;
            }
            return true;
        }
        if(DEBUG){
            std::cout << "Failed to add elements: not enough capacity" << std::endl;
        }
        return false;
    }


    /**
     * @brief Inserts an item at a specific index in the ArrayList.
     *
     * This function inserts the specified item at the specified index in the ArrayList. If the index is greater than the count of items in the ArrayList,
     * it prsize_ts an error message and returns false. If the ArrayList is full, it resizes the ArrayList if its size type is DYNAMIC, or prsize_ts an error message and returns false if its size type is STATIC.
     * If the index is valid and the ArrayList is not full, or has been resized successfully, it shifts all items from the index to the end of the ArrayList one position to the right, inserts the item at the index, and increments the count of items.
     *
     * @param index The index at which to insert the item.
     * @param item The item to insert.
     * @return true if the item was inserted successfully, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::insert(int index, T item) {
        if (index > count) {
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }
        if (count == arrayCapacity) {
            if (sizeType == DYNAMIC) {
                if (DEBUG) {
                    std::cout << "Resizing ArrayList from " << arrayCapacity << " to " << (arrayCapacity * 2) << std::endl;
                }
                resize();
            } else {
                throw ArrayListException("Error: Cannot insert item size_to full ArrayList if Mode is FIXED");
            }
        }
        if (DEBUG) {
            std::cout << "Inserting item at index " << index << std::endl;
        }
        std::copy_backward(array + index, array + count, array + count + 1);
        array[index] = item;
        ++count;
        if (DEBUG) {
            std::cout << "Item inserted. New count: " << count << std::endl;
        }
        return true;
    }

    /**
     * @brief Overloads the assignment operator to copy the contents of another ArrayList to this ArrayList.
     */
    template <typename T>
    ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& list) {
        if (this == &list) {
            if (DEBUG) {
                std::cout << "Self-assignment detected" << std::endl;
            }
            return *this;
        }

        if (DEBUG) {
            std::cout << "Assigning ArrayList with capacity: " << list.arrayCapacity << std::endl;
        }

        delete[] array;

        arrayCapacity = list.arrayCapacity;
        array = new T[arrayCapacity];
        sizeType = list.sizeType;
        count = list.count;

        std::copy(list.array, list.array + list.count, array);

        if (DEBUG) {
            std::cout << "Assignment complete. New capacity: " << arrayCapacity << ", New count: " << count << std::endl;
        }

        return *this;
    }



    /**
     * @brief Overloads the [] operator to access items in the ArrayList.
     * 
     */
    template <typename T>
    T& ArrayList<T>::operator[](int index) {
        if (DEBUG) {
            std::cout << "Accessing element at index " << index << std::endl;
        }
        if (index >= count) {
            if (DEBUG) {
                std::cout << "Index " << index << " is out of bounds" << std::endl;
            }
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }
        if (DEBUG) {
            std::cout << "Element at index " << index << " accessed successfully" << std::endl;
        }
        return array[index];
    }


    /**
     * @brief Inserts all items from another ArrayList at a specific index in this ArrayList.
     *
     * This function inserts all items from the specified ArrayList at the specified index in this ArrayList. If the index is greater than the count of items in this ArrayList,
     * it prsize_ts an error message and returns false. If the ArrayList is full, it resizes the ArrayList if its size type is DYNAMIC, or prsize_ts an error message and returns false if its size type is STATIC.
     * If the index is valid and the ArrayList is not full, or has been resized successfully, it shifts all items from the index to the end of the ArrayList one position to the right, inserts the items from the other ArrayList at the index, and increments the count of items.
     *
     * @param index The index at which to insert the items.
     * @param other The ArrayList whose items should be inserted.
     * @return true if the items were inserted successfully, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::insertAll(int index, const ArrayList<T>& other) {
        if (DEBUG) {
            std::cout << "Inserting " << other.count << " elements at index " << index << std::endl;
        }

        if (index > count) {
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }

        if (count + other.count > arrayCapacity) {
            if (sizeType == DYNAMIC) {
                if (DEBUG) {
                    std::cout << "Resizing ArrayList from " << arrayCapacity << " to " << (arrayCapacity * 2) << std::endl;
                }
                resize();
            } else {
                throw ArrayListException("Error: Cannot insert elements size_to full ArrayList if Mode is FIXED");
            }
        }

        if (DEBUG) {
            std::cout << "Shifting elements to make space for new elements" << std::endl;
        }
        std::copy_backward(array + index, array + count, array + count + other.count);

        if (DEBUG) {
            std::cout << "Copying new elements size_to ArrayList" << std::endl;
        }
        std::copy(other.array, other.array + other.count, array + index);

        count += other.count;

        if(DEBUG) {
            std::cout << "Elements inserted successfully. New count: " << count << std::endl;
        }

        return true;
    }


    /**
     * @brief Inserts all items from an array/List at a specific index in this ArrayList.
     *
     * This function inserts all items from the specified array/list at the specified index in this ArrayList. If the index is greater than the count of items in this ArrayList,
     * it prsize_ts an error message and returns false. If the ArrayList is full, it resizes the ArrayList if its size type is DYNAMIC, or prsize_ts an error message and returns false if its size type is STATIC.
     * If the index is valid and the ArrayList is not full, or has been resized successfully, it shifts all items from the index to the end of the ArrayList one position to the right, inserts the items from the array at the index, and increments the count of items.
     *
     * @param index The index at which to insert the items.
     * @param other The array/List whose items should be inserted.
     * @param length The length of the array/List.
     * @return true if the items were inserted successfully, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::insertAll(int index, const T* other, int length) {
        if (index > count) {
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }

        if (count + length > arrayCapacity) {
            if (sizeType == DYNAMIC) {
                if (DEBUG) {
                    std::cout << "Resizing ArrayList from " << arrayCapacity << " to " << (arrayCapacity * 2) << std::endl;
                }
                resize();
            } else {
                throw ArrayListException("Error: Cannot insert elements size_to full ArrayList if Mode is FIXED");
            }
        }

        if (DEBUG) {
            std::cout << "Shifting elements to make space for new elements" << std::endl;
        }
        std::copy_backward(array + index, array + count, array + count + length);

        if (DEBUG) {
            std::cout << "Copying new elements size_to ArrayList" << std::endl;
        }
        std::copy(other, other + length, array + index);

        count += length;

        if(DEBUG) {
            std::cout << "Elements inserted successfully. New count: " << count << std::endl;
        }

        return true;
    }
    
    /**
     * @brief Removes an item from the ArrayList.
     *
     * This function removes the specified item from the ArrayList. If the item is not found in the ArrayList, it prsize_ts an error message and returns false.
     * If the item is found in the ArrayList, it removes the item from the ArrayList and returns true.
     *
     * @param item The item to remove from the ArrayList.
     * @return true if the item was removed successfully, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::removeItem(T item) {
        if (DEBUG) {
            std::cout << "Attempting to remove item: " << item << std::endl;
        }

        int index = indexOf(item);
        if (index >= 0) {
            if (DEBUG) {
                std::cout << "Item found at index: " << index << ". Removing item." << std::endl;
            }
            bool removed = remove(index);
            if (removed) {
                if(exists(item)){
                    throw ArrayListException("Error: Failed to remove item at index: " + std::to_string(index));
                }
                if (DEBUG) {
                    std::cout << "Item removed successfully." << std::endl;
                }
                return true;
            } else {
                throw ArrayListException("Error: Failed to remove item at index: " + std::string(index));
            }
        } else {
            throw ArrayListException("Error: Item not found in ArrayList");
        }
    }

    /**
     * @brief Checks if an item exists in the ArrayList.
     * 
     * This function checks if the specified item exists in the ArrayList. If the item is found in the ArrayList, it returns true.
     * If the item is not found in the ArrayList, it returns false.
     * 
     */
    template <typename T>
    bool ArrayList<T>::exists(T item) {
        for (int i = 0; i < count; ++i) {
            if (array[i] == item) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * @brief Removes an item at a specific index from the ArrayList.
     *
     * This function removes the item at the specified index from the ArrayList. If the index is less than the count of items in the ArrayList,
     * it shifts all items from the index + 1 to the end of the ArrayList one position to the left, and decrements the count of items.
     *
     * @param index The index of the item to remove.
    */
    template <typename T>
    bool ArrayList<T>::remove(int index) {
        if (index >= count) {
            throw IndexOutOfBoundsException("Error: Index out of bounds. Cannot remove item.");
        }

        if (DEBUG) {
            std::cout << "Removing item at index: " << index << std::endl;
        }

        std::copy(array + index + 1, array + count, array + index);
        --count;

        if (DEBUG) {
            std::cout << "Item removed. New count: " << count << std::endl;
        }

        return true;
    }

    /**
     * @brief Removes all items from the ArrayList that satisfy a predicate.
     *
     * This function removes all items from the ArrayList that satisfy the specified predicate. The predicate is a function that takes an item of type T and returns a boolean.
     * If the predicate returns true for an item, that item is removed from the ArrayList. The function shifts all items that do not satisfy the predicate to the left to fill the gaps left by the removed items.
     * The function can use the Predicates library, which can be found at: https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/lib
     *
     * @param predicate The predicate to test the items against.
     * @return true if any items were removed, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::removeIf(bool (*predicate)(T)) {
        if (DEBUG) {
            std::cout << "Attempting to remove items based on predicate" << std::endl;
        }
    
        T* newEnd = std::remove_if(array, array + count, predicate);
        ptrdiff_t removed = array + count - newEnd;
    
        if (removed > 0) {
            count -= static_cast<int>(removed);
            if (DEBUG) {
                std::cout << "Removed " << removed << " items. New count: " << count << std::endl;
            }
            return true;
        }
    
        if (DEBUG) {
            std::cout << "No items removed" << std::endl;
        }
        return false;
    }
    /**
     * @brief Removes a range of items from the ArrayList.
     *
     * This function removes a range of items from the ArrayList, from the specified fromIndex (inclusive) to the specified toIndex (exclusive).
     * If fromIndex is greater than toIndex, or if either index is out of bounds, it prsize_ts an error message and returns without removing any items.
     * If the indices are valid, it shifts all items from toIndex to the end of the ArrayList to the left to fill the gaps left by the removed items, and decrements the count of items by the number of removed items.
     *
     * @param fromIndex The start index of the range to remove (inclusive).
     * @param toIndex The end index of the range to remove (exclusive).
    */
   template <typename T>
    void ArrayList<T>::removeRange(int fromIndex, int toIndex) {
        if (DEBUG) {
            std::cout << "Attempting to remove range from index " << fromIndex << " to index " << toIndex << std::endl;
        }

        if (fromIndex > toIndex) {
            throw IndexOutOfBoundsException("Error: fromIndex is greater than toIndex");
        }
        if (fromIndex >= count || toIndex > count) {
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }

        int removed = toIndex - fromIndex;
        if (DEBUG) {
            std::cout << "Removing " << removed << " elements" << std::endl;
        }

        std::copy(array + toIndex, array + count, array + fromIndex);
        count -= removed;

        if (DEBUG) {
            std::cout << "Range removed successfully. New count: " << count << std::endl;
        }
    }

    /**
     * @brief Retains only the items in this ArrayList that are contained in the specified ArrayList.
     *
     * This function removes from this ArrayList all of its items that are not contained in the specified ArrayList.
     * After this call, this ArrayList will contain only the items that are also contained in the other ArrayList.
     *
     * @param other The ArrayList containing the items to be retained in this ArrayList.
     * @return true if this ArrayList changed as a result of the call, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::retainAll(const ArrayList<T>& other) {
        if (DEBUG) {
            std::cout << "Attempting to retain all elements that are also in the other ArrayList" << std::endl;
        }

        int removed = 0;
        for (int i = 0; i < count; ++i) {
            if (!other.contains(array[i])) {
                ++removed;
                if (DEBUG) {
                    std::cout << "Element " << array[i] << " not found in the other ArrayList. Marking for removal." << std::endl;
                }
            } else {
                array[i - removed] = array[i];
                if (DEBUG) {
                    std::cout << "Element " << array[i] << " retained." << std::endl;
                }
            }
        }

        if (removed > 0) {
            count -= removed;
            if (DEBUG) {
                std::cout << "Removed " << removed << " elements. New count: " << count << std::endl;
            }
            return true;
        }

        if (DEBUG) {
            std::cout << "No elements removed. All elements are retained." << std::endl;
        }
        return false;
    }

    /**
     * @brief Clears all items from the ArrayList.
     *
     * This function clears all items from the ArrayList and sets the count of items to 0.
    */
    template <typename T>
    void ArrayList<T>::clear() {
        delete[] array;
        if(DEBUG){
            std::cout << "ArrayList cleared" << std::endl;
        }
        array = new T[arrayCapacity];
        if(DEBUG){
            std::cout << "New ArrayList created with initial size: " << arrayCapacity << std::endl;
        }
        count = 0;
        if(DEBUG){
            std::cout << "ArrayList count set to 0" << std::endl;
        }
    }

    //Query and Access
    
    /**
     * @brief Retrieves the item at a specific index in the ArrayList.
     *
     * This function retrieves the item at the specified index in the ArrayList. If the index is less than the count of items in the ArrayList,
     * it returns the item at the index. If the index is out of bounds, it returns a default-constructed instance of type T.
     *
     * @param index The index of the item to retrieve.
     * @return The item at the specified index, or a default-constructed instance of type T if the index is out of bounds.
    */
    template <typename T>
    T ArrayList<T>::get(int index) const {
        if (DEBUG) {
            std::cout << "Attempting to get element at index " << index << std::endl;
        }

        if (index < count) {
            if (DEBUG) {
                std::cout << "Element at index " << index << " retrieved successfully" << std::endl;
            }
            return array[index];
        } else {
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }
    }

    /**
     * @brief Retrieves the item at a specific index in the ArrayList as a std::string.
     *
     * This function retrieves the item at the specified index in the ArrayList and converts it to a std::string. If the index is less than the count of items in the ArrayList,
     * it returns the item at the index as a std::string. If the index is out of bounds, it returns a default-constructed instance of type T as a std::string.
     *
     * @param index The index of the item to retrieve.
     * @return The item at the specified index as a std::string, or a default-constructed instance of type T as a std::string if the index is out of bounds.
    */
    template <typename T>
    std::string ArrayList<T>::getAsString(int index) const {
        if (DEBUG) {
            std::cout << "Attempting to get element as std::string at index " << index << std::endl;
        }

        if (index < count) {
            if (DEBUG) {
                std::cout << "Element at index " << index << " retrieved successfully" << std::endl;
            }
            return toString(array[index]);
        } else {
            
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }
    }

    /**
     * @brief Checks if the ArrayList contains an item.
     *
     * This function checks if the ArrayList contains the specified item. If the item is found in the ArrayList, it returns true.
     * If the item is not found in the ArrayList, it returns false.
     *
     * @param item The item to check for.
     * @return true if the ArrayList contains the item, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::contains(T item) const {
        for (int i = 0; i < count; ++i) {
            if (array[i] == item) {
                return true;
            }
        }
        return false;
    }
    
    /**
    * @brief Finds the index of the first occurrence of the specified item in the ArrayList.
    *
    * This function returns the index of the first occurrence of the specified item in the ArrayList, or -1 if the ArrayList does not contain the item.
    * It iterates over the ArrayList from the beginning to the end, comparing each item with the specified item using the == operator.
    * If it finds a match, it returns the index of the match. If it does not find a match, it prsize_ts an error message (if debug is true) and returns -1.
    *
    * @param item The item to find in the ArrayList.
    * @return The index of the first occurrence of the item in the ArrayList, or -1 if the ArrayList does not contain the item.
    */
    template <typename T>
    int ArrayList<T>::indexOf(T item) const {
        if (DEBUG) {
            std::cout << "Searching for item: " << item << std::endl;
        }

        for (int i = 0; i < count; ++i) {
            if (DEBUG) {
                std::cout << "Checking index " << i << ": " << array[i] << std::endl;
            }
            if (array[i] == item) {
                if (DEBUG) {
                    std::cout << "Item found at index " << i << std::endl;
                }
                return i;
            }
        }

        if (DEBUG) {
            std::cout << "Item not found in ArrayList" << std::endl;
        }
        return -1;  // Return -1 instead of throwing an exception
    }


    /**
     * @brief Retrieves the capacity of the ArrayList.
     *
     * This function returns the current capacity of the ArrayList, which is the maximum number of items it can hold without resizing.
     *
     * @return The capacity of the ArrayList.
    */
   template <typename T>
    int ArrayList<T>::capacity() const {
        if(DEBUG){
            std::cout << "ArrayList capacity: " << arrayCapacity << std::endl;
        }
        return arrayCapacity;
    }

    /**
     * @brief Retrieves the count of items in the ArrayList.
     *
     * This function returns the current count of items in the ArrayList.
     *
     * @return The count of items in the ArrayList.
    */
    template <typename T>
    int ArrayList<T>::size() const {
        if(DEBUG){
            std::cout << "ArrayList count: " << count << std::endl;
        }
        return count;
    }

    /**
     * @brief Checks if the ArrayList is empty.
     *
     * This function checks if the ArrayList is empty. If the ArrayList is empty, it returns true. If the ArrayList is not empty, it returns false.
     *
     * @return true if the ArrayList is empty, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::isEmpty() const {
        if(DEBUG){
            if(count == 0){
                std::cout << "ArrayList is empty" << std::endl;
            } else {
                std::cout << "ArrayList is not empty" << std::endl;
            }
        }
        return count == 0;
    }

    /**
     * @brief Sets the item at a specific index in the ArrayList.
     *
     * This function sets the item at the specified index in the ArrayList to the specified item.
     * If the index is less than the count of items in the ArrayList, it sets the item at the index and checks if the item was set correctly.
     * If the item was set correctly, it returns true. If the index is out of bounds, it prsize_ts an error message (if debug is true) and returns false.
     *
     * @param index The index at which to set the item.
     * @param item The item to set.
     * @return true if the item was set correctly, false otherwise.
    */
    template <typename T>
    bool ArrayList<T>::set(int index, T item) {
        if (DEBUG) {
            std::cout << "Attempting to set item at index " << index << std::endl;
        }

        if (index < count) {
            array[index] = item;
            if (DEBUG) {
                std::cout << "Item set at index " << index << ". Verifying..." << std::endl;
            }
            if (array[index] == item) {
                if (DEBUG) {
                    std::cout << "Item set successfully at index " << index << std::endl;
                }
                return true;
            } else {
                throw ArrayListException("Error: Failed to set item at index " + std::to_string(index));
            }
        } else {
            throw IndexOutOfBoundsException("Error: Index out of bounds");
        }
    }

    /**
     * @brief Replaces each item in the ArrayList with the result of applying an operator function to that item.
     *
     * This function applies the specified operator function to each item in the ArrayList, and replaces the item with the result of the function.
     * The operator function is a function that takes an item of type T and returns a new item of type T.
     * The function uses the Operators library, which can be found at: https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/lib
     *
     * @note You can utilize any Operator function from the Operators library, or create your own.
     * @param operatorFunction The operator function to apply to each item.
    */
    template <typename T>
    void ArrayList<T>::replaceAll(T (*operatorFunction)(T)) {
        if (DEBUG) {
            std::cout << "Attempting to replace all elements using the provided operator function" << std::endl;
        }

        if (operatorFunction == nullptr) {
            if (DEBUG) {
                std::cout << "Operator function is null" << std::endl;
            }
            throw std::invalid_argument("Error: Operator function cannot be null");
        }

        for (int i = 0; i < count; ++i) {
            if (DEBUG) {
                std::cout << "Replacing element at index " << i << ": " << array[i] << std::endl;
            }
            array[i] = operatorFunction(array[i]);
            if (DEBUG) {
                std::cout << "Element at index " << i << " replaced with: " << array[i] << std::endl;
            }
        }

        if (DEBUG) {
            std::cout << "All elements replaced successfully" << std::endl;
        }
    }


    //Utility Functions
    /**
     * @brief Performs an action on each item in the ArrayList.
     *
     * This function applies the specified consumer function to each item in the ArrayList.
     * The consumer function is a function that takes an item of type T and performs an action on it.
     * The function does not return a result.
     * The function can use the Operators library, if needed, which can be found at: https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/lib
     *
     * @param consumer The consumer function to apply to each item.
    */
    template <typename T>
    void ArrayList<T>::forEach(void (*consumer)(T)) const {
        if (DEBUG) {
            std::cout << "Attempting to apply consumer function to each element" << std::endl;
        }

        if (consumer == nullptr) {
            if (DEBUG) {
                std::cout << "Consumer function is null" << std::endl;
            }
            throw std::invalid_argument("Error: Consumer function cannot be null");
        }

        for (int i = 0; i < count; ++i) {
            if (DEBUG) {
                std::cout << "Applying consumer function to element at index " << i << ": " << array[i] << std::endl;
            }
            consumer(array[i]);
        }

        if (DEBUG) {
            std::cout << "Consumer function applied to all elements successfully" << std::endl;
        }
    }


    /**
     * @brief Converts the ArrayList to an array.
     *
     * This function copies the items from the ArrayList size_to the specified output array.
     * The output array must be large enough to hold all items in the ArrayList.
     * The function uses the memcpy function to copy the items, so the type T must be trivially copyable.
     *
     * @param outputArray The array size_to which to copy the items.
     * @return The output array.
    */
    template <typename T>
    T* ArrayList<T>::toArray(T* outputArray) const {
        if (DEBUG) {
            std::cout << "Attempting to copy elements to output array" << std::endl;
        }

        if (outputArray == nullptr) {
            if (DEBUG) {
                std::cout << "Output array is null" << std::endl;
            }
            throw std::invalid_argument("Error: Output array cannot be null");
        }

        int outputArraySize = sizeof(outputArray) / sizeof(outputArray[0]);
        if (outputArraySize < count) {
            if (DEBUG) {
                std::cout << "Output array size is smaller than the number of elements to copy" << std::endl;
            }
            throw std::invalid_argument("Error: Output array size is smaller than the number of elements to copy");
        }

        std::copy(array, array + count, outputArray);

        if (DEBUG) {
            std::cout << "Elements copied successfully to output array" << std::endl;
        }

        return outputArray;
    }

    /**
     * @brief Creates a sublist of the ArrayList.
     *
     * This function creates a new ArrayList that contains the items in the original ArrayList from the specified fromIndex, inclusive, to the specified toIndex, exclusive.
     * If fromIndex is greater than toIndex, or if either index is out of bounds, the function returns nullptr.
     *
     * @param fromIndex The start index of the sublist, inclusive.
     * @param toIndex The end index of the sublist, exclusive.
     * @return A new ArrayList that contains the items in the sublist, or nullptr if the indices are invalid.
    */
    template <typename T>
    ArrayList<T>* ArrayList<T>::sublist(int fromIndex, int toIndex) const {
        if (DEBUG) {
            std::cout << "Attempting to create sublist from index " << fromIndex << " to index " << toIndex << std::endl;
        }

        if (fromIndex > toIndex || fromIndex >= count || toIndex > count) {
            if (DEBUG) {
                std::cout << "Invalid indices for sublist" << std::endl;
            }
            return nullptr;
        }
        ArrayList* sublist = new ArrayList<T>();
        for (int i = fromIndex; i < toIndex; ++i) {
            sublist->add(array[i]);
        }

        if (DEBUG) {
            std::cout << "Sublist created successfully" << std::endl;
        }

        return sublist;
    }

    /**
     * @brief Creates a clone of the ArrayList.
     *
     * This function creates a new ArrayList that is a clone of the original ArrayList.
     * The new ArrayList has the same size type, capacity, and items as the original ArrayList.
     *
     * @return A new ArrayList that is a clone of the original ArrayList.
    */
    template <typename T>
    ArrayList<T>* ArrayList<T>::clone() const {
        if (DEBUG) {
            std::cout << "Attempting to clone the ArrayList" << std::endl;
        }

        try {
            ArrayList<T>* clone = new ArrayList<T>(sizeType, arrayCapacity);
            clone->addAll(*this);

            if (DEBUG) {
                std::cout << "ArrayList cloned successfully" << std::endl;
            }

            return clone;
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during cloning: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }

    /**
     * @brief Ensures that the ArrayList can hold at least the specified number of items without needing to resize.
     *
     * This function checks if the ArrayList's current capacity is less than the specified minimum capacity.
     * If it is, it resizes the ArrayList to the specified minimum capacity.
     * The function uses the memcpy function to copy the items to the new array, so the type T must be trivially copyable.
     *
     * @param minCapacity The minimum capacity that the ArrayList should be able to hold without resizing.
    */
    template <typename T>
    void ArrayList<T>::ensureCapacity(int minCapacity) {
        if (DEBUG) {
            std::cout << "Ensuring capacity. Minimum required: " << minCapacity << ", current capacity: " << arrayCapacity << std::endl;
        }

        if (minCapacity > arrayCapacity) {
            try {
                T* newArray = new T[minCapacity];
                if (array != nullptr) {
                    std::copy(array, array + count, newArray);
                }
                delete[] array;
                array = newArray;
                arrayCapacity = minCapacity;

                if (DEBUG) {
                    std::cout << "Capacity increased to " << arrayCapacity << std::endl;
                }
            } catch (const std::bad_alloc& e) {
                if (DEBUG) {
                    std::cout << "Memory allocation failed: " << e.what() << std::endl;
                }
                throw; // Re-throw the exception after logging
            }
        } else {
            if (DEBUG) {
                std::cout << "Current capacity is sufficient" << std::endl;
            }
        }
    }


    /**
     * @brief Trims the capacity of the ArrayList to its current size.
     *
     * This function reduces the capacity of the ArrayList to its current size, i.e., the number of items it contains.
     * If the ArrayList is dynamic and its capacity is greater than its size, it creates a new array with a capacity equal to the size, copies the items to the new array, and deletes the old array.
     * If the ArrayList is already trimmed or is fixed size, it prsize_ts an error message (if debug is true).
    */
    template <typename T>
     void ArrayList<T>::trimToSize() {
        if (DEBUG) {
            std::cout << "Attempting to trim array to size. Current count: " << count << ", current capacity: " << arrayCapacity << std::endl;
        }

        if (sizeType == DYNAMIC && count < arrayCapacity) {
            try {
                T* newArray = new T[count];
                if (array != nullptr) {
                    std::copy(array, array + count, newArray);
                }
                delete[] array;
                array = newArray;
                arrayCapacity = count;

                if (DEBUG) {
                    std::cout << "Array trimmed to size. New capacity: " << arrayCapacity << std::endl;
                }
            } catch (const std::bad_alloc& e) {
                if (DEBUG) {
                    std::cout << "Memory allocation failed: " << e.what() << std::endl;
                }
                throw; // Re-throw the exception after logging
            }
        } else {
            if (DEBUG) {
                std::cout << "No trimming needed. Current capacity is already equal to or less than count." << std::endl;
            }
        }
    }

    /**
     * @brief Sets the sorting algorithm to use for sorting the ArrayList.
     *
     * This function sets the sorting algorithm to use for sorting the ArrayList.
     * The sorting algorithm is specified by the SortAlgorithm enum value.
     *
     * @param algorithm The sorting algorithm to use.
    */
    template <typename T>
    void ArrayList<T>::setSortAlgorithm(SortAlgorithm algorithm){
        sortAlgorithm = algorithm;
        if(DEBUG){
            std::cout << "Sorting algorithm set to: " << algorithm << std::endl;
        }
    }

    /**
     * @brief Gets the sorting algorithm used for sorting the ArrayList.
     *
     * This function gets the sorting algorithm used for sorting the ArrayList.
     * The sorting algorithm is specified by the SortAlgorithm enum value.
     *
     * @return The sorting algorithm used for sorting the ArrayList.
    */
    template <typename T>
    typename ArrayList<T>::SortAlgorithm ArrayList<T>::getSortAlgorithm(){ 
    
        if(DEBUG){
            std::cout << "Sorting algorithm: " << sortAlgorithm << std::endl;
        }
        return sortAlgorithm;
    }

    /**
     * @brief Sorts the ArrayList.
     * 
     * This function sorts the ArrayList using the specified comparator function
     * Sorting Algorithm is determined by the sortAlgorithm variable (use the sort function to set the sorting algorithm)
    */
    template <typename T>
    void ArrayList<T>::sort(bool (*comparator)(T, T)) {
        if (DEBUG) {
            std::cout << "Attempting to sort the ArrayList" << std::endl;
        }

        if (comparator == nullptr) {
            if (DEBUG) {
                std::cout << "Comparator function is null" << std::endl;
            }
            throw std::invalid_argument("Error: Comparator function cannot be null");
        }

        try {
            switch (sortAlgorithm) {
                case BUBBLE_SORT:
                    if (DEBUG) {
                        std::cout << "Using Bubble Sort" << std::endl;
                    }
                    bubbleSort(comparator);
                    break;
                case QUICK_SORT:
                    if (DEBUG) {
                        std::cout << "Using Quick Sort" << std::endl;
                    }
                    quickSortHelper(comparator, 0, count - 1);
                    break;
                default:
                    if (DEBUG) {
                        std::cout << "Using Merge Sort" << std::endl;
                    }
                    mergeSort(*this, 0, count - 1);
                    break;
            }

            if (DEBUG) {
                std::cout << "ArrayList sorted successfully" << std::endl;
            }
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during sorting: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }

    /**
     * @brief Sorts the ArrayList.
     *
     * This function sorts the ArrayList using the specified comparator function and sorting algorithm.
     * The comparator function is a function that takes two items of type T and returns true if the first item is less than the second item, or false otherwise.
     * The sorting algorithm is an enum value that specifies the sorting algorithm to use. The available sorting algorithms are BUBBLE_SORT and QUICK_SORT.
     * The function uses the bubble sort algorithm by default.
     * The function can use the Predicates library, which can be found at:
    */ 
    template <typename T>
    void ArrayList<T>::sort(bool (*comparator)(T, T), SortAlgorithm algorithm) {
        if (DEBUG) {
            std::cout << "Attempting to sort the ArrayList using the specified algorithm" << std::endl;
        }

        if (comparator == nullptr) {
            if (DEBUG) {
                std::cout << "Comparator function is null" << std::endl;
            }
            throw std::invalid_argument("Error: Comparator function cannot be null");
        }

        try {
            switch (algorithm) {
                case BUBBLE_SORT:
                    if (DEBUG) {
                        std::cout << "Using Bubble Sort" << std::endl;
                    }
                    bubbleSort(comparator);
                    break;
                case QUICK_SORT:
                    if (DEBUG) {
                        std::cout << "Using Quick Sort" << std::endl;
                    }
                    quickSortHelper(comparator, 0, count - 1);
                    break;
                // Add cases for additional sorting algorithms
                default:
                    if (DEBUG) {
                        std::cout << "Using Merge Sort" << std::endl;
                    }
                    mergeSort(*this, 0, count - 1);
                    break;
            }

            if (DEBUG) {
                std::cout << "ArrayList sorted successfully" << std::endl;
            }
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during sorting: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }

    /**
     * @brief Sorts the ArrayList using the bubble sort algorithm.
     *
     * This function sorts the items in the ArrayList using the bubble sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
    */
    template <typename T>
    void ArrayList<T>::bubbleSort(bool (*comparator)(T, T)) {
        if (DEBUG) {
            std::cout << "Starting Bubble Sort" << std::endl;
        }

        if (comparator == nullptr) {
            if (DEBUG) {
                std::cout << "Comparator function is null" << std::endl;
            }
            throw std::invalid_argument("Error: Comparator function cannot be null");
        }

        try {
            for (int i = 0; i < count - 1; ++i) {
                bool swapped = false;
                for (int j = 0; j < count - i - 1; ++j) {
                    if (comparator(array[j + 1], array[j])) { // Corrected comparator logic
                        if (DEBUG) {
                            std::cout << "Swapping elements at indices " << j << " and " << j + 1 << std::endl;
                        }
                        T temp = array[j];
                        array[j] = array[j + 1];
                        array[j + 1] = temp;
                        swapped = true;
                    }
                }
                if (!swapped) {
                    break; // Stop if the array is already sorted
                }
            }

            if (DEBUG) {
                std::cout << "Bubble Sort completed" << std::endl;
            }
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during Bubble Sort: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }

    /**
     * @brief Sorts the ArrayList using the quick sort algorithm.
     *
     * This function sorts the items in the ArrayList using the quick sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
    */
    template <typename T>
    void ArrayList<T>::quickSort(bool (*comparator)(T, T)) {
        if (DEBUG) {
            std::cout << "Starting Quick Sort" << std::endl;
        }
        quickSortHelper(comparator, 0, count - 1);
    }


    /**
     * @brief Returns a pointer to the first item in the ArrayList.
     *
     * This function returns a posize_ter to the first item in the ArrayList.
     * This can be used to iterate over the ArrayList using posize_ter arithmetic.
     *
     * @return A posize_ter to the first item in the ArrayList.
    */
    template <typename T>
    T* ArrayList<T>::begin() const { 
        if(DEBUG){
            std::cout << "Returning posize_ter to first element" << &array[0] << std::endl;
        }
        return &array[0]; 
    }
    
    /**
     * @brief Returns a posize_ter to the item after the last item in the ArrayList.
     *
     * This function returns a posize_ter to the item after the last item in the ArrayList.
     * This can be used to iterate over the ArrayList using posize_ter arithmetic.
     *
     * @return A posize_ter to the item after the last item in the ArrayList.
    */
    template <typename T>
    T* ArrayList<T>::end() const { 
        if(DEBUG){
            std::cout << "Returning posize_ter to last element" << &array[count] << std::endl;
        }
        return &array[count]; 
    }


    /**
     * Recursively sorts the elements of an ArrayList using the Merge Sort algorithm.
     * This function splits the list size_to two halves, calls itself for the two halves,
     * and then merges the two sorted halves. The function uses the merge() function
     * for merging two halves. The recursive sorting continues until the sub-arrays
     * have only one element each, which are inherently sorted.
     * 
     * @param list Reference to an ArrayList of template type T to be sorted.
     * @param l The starting index of the sub-array to be sorted.
     * @param r The ending index of the sub-array to be sorted.
     * @template T The data type of the elements in the ArrayList.
    */
    template <typename T>
    void ArrayList<T>::mergeSort(ArrayList<T>& list, size_t l, size_t r) {
        if (DEBUG) {
            std::cout << "Starting merge sort for subarray: [" << l << ", " << r << "]" << std::endl;
        }

        try {
            if (l < r) {
                size_t m = l + (r - l) / 2;
                mergeSort(list, l, m);
                mergeSort(list, m + 1, r);
                merge(list, l, m, r);
            }

            if (DEBUG) {
                std::cout << "Merge sort completed for subarray: [" << l << ", " << r << "]" << std::endl;
            }
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during merge sort: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }

    /**
     * @brief Reverses the order of the items in the ArrayList.
     */
    template <typename T>
    void ArrayList<T>::reverse(){
        for(int i = 0; i < count/2; i++){
            T temp = array[i];
            array[i] = array[count - i - 1];
            array[count - i - 1] = temp;
        }
    }

    /**
     * @brief Resizes the ArrayList.
     *
     * This function resizes the ArrayList to 1.5 times its current capacity.
     * It creates a new array with the new capacity, copies the items to the new array, and deletes the old array.
     * If the new capacity is greater than the maximum size_t value, it prsize_ts an error message (if debug is true) and does not resize the ArrayList.
    */
    template <typename T>
    void ArrayList<T>::resize() {
        if (DEBUG) {
            std::cout << "Attempting to resize the ArrayList. Current capacity: " << arrayCapacity << ", current count: " << count << std::endl;
        }

        int newCapacity = static_cast<int>(arrayCapacity * RESIZE_FACTOR);
        if (newCapacity <= arrayCapacity) {
            if (DEBUG) {
                std::cout << "New capacity is not greater than current capacity. Resize aborted." << std::endl;
            }
            return;
        }

        try {
            T* newArray = new T[newCapacity];
            std::copy(array, array + count, newArray);
            delete[] array;
            array = newArray;
            arrayCapacity = newCapacity;

            if (DEBUG) {
                std::cout << "ArrayList resized successfully. New capacity: " << arrayCapacity << std::endl;
            }
        } catch (const std::bad_alloc& e) {
            if (DEBUG) {
                std::cout << "Memory allocation failed during resize: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }

    /**
     * @brief Sorts the ArrayList using the quick sort algorithm.
     *
     * This function sorts the items in the ArrayList using the quick sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
     * @param low The lowest index of the sublist to sort.
     * @param high The highest index of the sublist to sort.
    */
    template <typename T>
    size_t ArrayList<T>::partition(bool (*comparator)(T, T), size_t low, size_t high) {
        if (DEBUG) {
            std::cout << "Starting partition for subarray: [" << low << ", " << high << "]" << std::endl;
        }

        if (comparator == nullptr) {
            if (DEBUG) {
                std::cout << "Comparator function is null" << std::endl;
            }
            throw std::invalid_argument("Error: Comparator function cannot be null");
        }

        try {
            T pivot = array[high];
            int i = (low - 1);

            for (int j = low; j <= high - 1; j++) {
                if (comparator(array[j], pivot)) {
                    i++;
                    if (DEBUG) {
                        std::cout << "Swapping elements at indices " << i << " and " << j << std::endl;
                    }
                    swap(&array[i], &array[j]);
                }
            }

            if (DEBUG) {
                std::cout << "Swapping elements at indices " << i + 1 << " and " << high << std::endl;
            }
            swap(&array[i + 1], &array[high]);

            if (DEBUG) {
                std::cout << "Partition completed. Pivot index: " << i + 1 << std::endl;
            }

            return (i + 1);
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during partition: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }

    /**
     * @brief Sorts the ArrayList using the quick sort algorithm.
     *
     * This function sorts the items in the ArrayList using the quick sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
     * @param low The lowest index of the sublist to sort.
     * @param high The highest index of the sublist to sort.
    */
    template <typename T>
    void ArrayList<T>::quickSortHelper(bool (*comparator)(T, T), size_t low, size_t high) {
        if (DEBUG) {
            std::cout << "Starting quick sort for subarray: [" << low << ", " << high << "]" << std::endl;
        }

        if (comparator == nullptr) {
            if (DEBUG) {
                std::cout << "Comparator function is null" << std::endl;
            }
            throw std::invalid_argument("Error: Comparator function cannot be null");
        }

        try {
            if (low < high) {
                size_t pi = partition(comparator, low, high);
                if (DEBUG) {
                    std::cout << "Pivot index: " << pi << std::endl;
                }
                quickSortHelper(comparator, low, pi - 1);
                quickSortHelper(comparator, pi + 1, high);
            }

            if (DEBUG) {
                std::cout << "Quick sort completed for subarray: [" << low << ", " << high << "]" << std::endl;
            }
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during quick sort: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }
 
    /**
     * Merges two halves of an ArrayList that have been sorted independently.
     * The function takes three indices, l (left index), m (middle index), and r (right index).
     * It assumes that the sub-arrays ArrayList[l..m] and ArrayList[m+1..r] are sorted,
     * and merges them size_to a single sorted sub-array ArrayList[l..r].
     * 
     * @param list Reference to an ArrayList of template type T that is being sorted.
     * @param l The starting index of the first sub-array.
     * @param m The ending index of the first sub-array, which also serves as the middle posize_t.
     * @param r The ending index of the second sub-array.
    */
    template <typename T>
    void ArrayList<T>::merge(ArrayList<T>& list, size_t l, size_t m, size_t r) {
        if (DEBUG) {
            std::cout << "Merging subarrays: [" << l << ", " << m << "] and [" << m + 1 << ", " << r << "]" << std::endl;
        }
    
        try {
            std::vector<T> L(list.begin() + l, list.begin() + m + 1);
            std::vector<T> R(list.begin() + m + 1, list.begin() + r + 1);
    
            size_t i = 0, j = 0, k = l;
            while (i < L.size() && j < R.size()) {
                if (L[i] <= R[j]) {
                    list[k] = L[i];
                    i++;
                } else {
                    list[k] = R[j];
                    j++;
                }
                k++;
            }
    
            while (i < L.size()) {
                list[k] = L[i];
                i++;
                k++;
            }
    
            while (j < R.size()) {
                list[k] = R[j];
                j++;
                k++;
            }
    
            if (DEBUG) {
                std::cout << "Merge completed for subarrays: [" << l << ", " << m << "] and [" << m + 1 << ", " << r << "]" << std::endl;
            }
        } catch (const std::exception& e) {
            if (DEBUG) {
                std::cout << "Error during merge: " << e.what() << std::endl;
            }
            throw; // Re-throw the exception after logging
        }
    }


    /**
     * @brief Swaps two items in the ArrayList.
     *
     * This function swaps the items at the specified indices in the ArrayList.
     *
     * @param a The index of the first item to swap.
     * @param b The index of the second item to swap.
    */
    template <typename T>
    void ArrayList<T>::swap(T* a, T* b) { 
        T temp = *a; 
        *a = *b; 
        *b = temp; 
    }

#endif // ARRAYLIST_H
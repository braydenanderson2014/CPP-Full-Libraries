#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <exception>
#include <iostream>
#include <string>
#include <cstddef>
#include "SimpleVector.h"
#include <sstream>


template <typename T>
std::string to_string_helper(const T& t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template <typename K>
struct KeyHash{
    KeyHash() {}
};

template <>
struct KeyHash<std::string> {
    KeyHash() {}
    unsigned long operator()(const std::string& key) const {
        unsigned long hash = 0;
        for (char c : key) {
            hash = 31 * hash + c;
        }
        return hash;
    }
};

template <>
struct KeyHash<int> {
    unsigned long operator()(const int& key) const {
        return static_cast<unsigned long>(key);
    }
};

template <>
struct KeyHash<bool> {
    unsigned long operator()(const bool& key) const {
        return static_cast<unsigned long>(key);
    }
};

template <>
struct KeyHash<float> {
    unsigned long operator()(const float& key) const {
        unsigned long hash = *reinterpret_cast<const unsigned long*>(&key);
        return hash;
    }
};

template <>
struct KeyHash<double> {
    unsigned long operator()(const double& key) const {
        unsigned long hash = *reinterpret_cast<const unsigned long*>(&key);
        return hash;
    }
};

template<>
struct KeyHash<char> {
    unsigned long operator()(const char& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<long> {
    unsigned long operator()(const long& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<long long> {
    unsigned long operator()(const long long& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<unsigned long> {
    unsigned long operator()(const unsigned long& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<unsigned long long> {
    unsigned long operator()(const unsigned long long& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<unsigned int> {
    unsigned long operator()(const unsigned int& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<unsigned char> {
    unsigned long operator()(const unsigned char& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<unsigned short> {
    unsigned long operator()(const unsigned short& key) const {
        return static_cast<unsigned long>(key);
    }
};

template<>
struct KeyHash<short> {
    unsigned long operator()(const short& key) const {
        return static_cast<unsigned long>(key);
    }
};

template <typename K, typename V, typename Hash = KeyHash<K>>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    static const int INITIAL_TABLE_SIZE = 16; // The initial size of the table
    Entry** table; // The table itself
    int TABLE_SIZE; // The current size of the table
    int count;  // The number of elements in the table
    float loadFactorThreshold = 0.7; // The load factor threshold for resizing
    Hash hashFunction; // The hash function to use
    
    void resize();
    void clear(Entry** table, int capacity);
    int hash(const K& key);
public:
    HashTable();
    ~HashTable();
    void insert(const K& key, const V& value);
    void remove(const K& key);
    V& get(const K& key);
    bool contains(const K& key) const;
    bool isEmpty();
    int size();
    void clear();
    void print();

    struct KeyValuePair {
        K key;
        V value;
    };

    class HashtableIterator{
    private:
        const HashTable<K, V, Hash>* hashtable;
        int currentBucket;
        Entry* currentEntry;
        
        void goToNextEntry() {
            if (currentEntry && currentEntry->next) {
                currentEntry = currentEntry->next;
            } else {
                do {
                    currentBucket++;
                    if (currentBucket >= hashtable->TABLE_SIZE) {
                        currentEntry = nullptr;
                        return;
                    }
                    currentEntry = hashtable->table[currentBucket];
                } while (!currentEntry);
            }
        }
    public:
        HashtableIterator(const HashTable<K, V, Hash>* ht, int bucket, Entry* entry)
                : hashtable(ht), currentBucket(bucket), currentEntry(entry) {}
        
        KeyValuePair operator*() {
            if (currentEntry == nullptr) {
                throw std::runtime_error("Dereferencing end iterator");
            }
            return KeyValuePair{currentEntry->key, currentEntry->value};
        }
        
        HashtableIterator& operator++() {
            goToNextEntry();
            return *this;
        }
        
        bool operator!=(const HashtableIterator& other) {
            return currentBucket != other.currentBucket || currentEntry != other.currentEntry;
        }


        HashtableIterator find(const K& key){
            int index = hashtable->hash(key);
            currentBucket = index;
            currentEntry = hashtable->table[index];
            while (currentEntry != nullptr) {
                if (currentEntry->key == key) {
                    return *this;
                }
                currentEntry = currentEntry->next;
            }
            goToNextEntry();
            return *this;
        }

        SimpleVector<K> getKeys() const {
            SimpleVector<K> keys;
            for (int i = 0; i < hashtable->TABLE_SIZE; ++i) {
                for (Entry* entry = hashtable->table[i]; entry != nullptr; entry = entry->next) {
                    keys.push_back(entry->key);
                }
            }
            return keys;
        }

        SimpleVector<V> getValues() const {
            SimpleVector<V> values;
            for (int i = 0; i < hashtable->TABLE_SIZE; ++i) {
                for (Entry* entry = hashtable->table[i]; entry != nullptr; entry = entry->next) {
                    values.push_back(entry->value);
                }
            }
            return values;
        }
        
    };
    HashtableIterator begin();
    HashtableIterator end();
    HashtableIterator cbegin() const;
    HashtableIterator cend() const;
    
};

//===============================================================
    #ifndef KEYNOTFOUNDEXCEPTION
    #define KEYNOTFOUNDEXCEPTION
        class KeyNotFoundException : public std::exception {
        public:
            explicit KeyNotFoundException(const std::string& message) : msg_(message) {}
            virtual const char* what() const noexcept {
                return msg_.c_str();
            }
        private:
            std::string msg_;
        };
    #endif // KEYNOTFOUNDEXCEPTION

    #ifndef HASHTABLEEXCEPTION
    #define HASHTABLEEXCEPTION
        class HashtableException : public std::exception {
        public:
            explicit HashtableException(const std::string& message) : msg_(message) {}
            virtual const char* what() const noexcept {
                return msg_.c_str();
            }
        private:
            std::string msg_;
        };
    #endif // HASHTABLEEXCEPTION

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

    #endif // INDEXOUTOFBOUNDSEXCEPTION

//===============================================================


template <typename K, typename V, typename Hash>
HashTable<K, V, Hash>::HashTable() : TABLE_SIZE(INITIAL_TABLE_SIZE), count(0), hashFunction() {
    table = new Entry*[TABLE_SIZE]();
    if (!table) {
        throw HashtableException("Memory allocation failed.");
    }
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

template <typename K, typename V, typename Hash>
HashTable<K, V, Hash>::~HashTable() {
    clear();
    delete[] table;
}

template <typename K, typename V, typename Hash>
void HashTable<K, V, Hash>::insert(const K& key, const V& value) {
    if (count >= TABLE_SIZE * loadFactorThreshold) {
        resize();
    }
    
    int index = hashFunction(key) % TABLE_SIZE;
    Entry* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;  // Update existing entry
            return;
        }
        current = current->next;
    }
    
    // If we reach here, the key doesn't exist, so we insert a new entry
    Entry* newEntry = new Entry(key, value);
    newEntry->next = table[index];
    table[index] = newEntry;
    count++;
}

template <typename K, typename V, typename Hash>
void HashTable<K, V, Hash>::remove(const K& key) {
    int index = hashFunction(key) % TABLE_SIZE;
    Entry* current = table[index];
    Entry* prev = nullptr;
    while (current != nullptr) {
        if (current->key == key) {
            if (prev != nullptr) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            count--;
            return;
        }
        prev = current;
        current = current->next;
    }
    throw KeyNotFoundException("Key not found in hash table. Key: " + to_string_helper(key));
}

template <typename K, typename V, typename Hash>
V& HashTable<K, V, Hash>::get(const K& key) {
    int index = hashFunction(key) % TABLE_SIZE;
    Entry* entry = table[index];
    while (entry != nullptr) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = entry->next;
    }
    throw KeyNotFoundException("Key not found in hash table. Key: " + to_string_helper(key));
}

template <typename K, typename V, typename Hash>
bool HashTable<K, V, Hash>::contains(const K& key) const {
    int index = hashFunction(key) % TABLE_SIZE;
    Entry* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}


template <typename K, typename V, typename Hash>
bool HashTable<K, V, Hash>::isEmpty() {
    return count == 0;
}

template <typename K, typename V, typename Hash>
int HashTable<K, V, Hash>::size() {
    return count;
}

template <typename K, typename V, typename Hash>
void HashTable<K, V, Hash>::clear() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Entry* current = table[i];
        while (current != nullptr) {
            Entry* next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    count = 0;
}

template <typename K, typename V, typename Hash>
void HashTable<K, V, Hash>::print() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Entry* entry = table[i];
        while (entry != nullptr) {
            entry = entry->next;
        }
    }
}

template <typename K, typename V, typename Hash>
void HashTable<K, V, Hash>::resize() {
    if (TABLE_SIZE >= std::numeric_limits<int>::max() / 2) {
        throw HashtableException("Cannot resize: maximum table size reached.");
    }
    int newSize = TABLE_SIZE * 2;
    Entry** newTable = new Entry*[newSize]();
    if (!newTable) {
        throw HashtableException("Memory allocation failed during resize.");
    }
    for (int i = 0; i < newSize; ++i) {
        newTable[i] = nullptr;
    }
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Entry* entry = table[i];
        while (entry != nullptr) {
            Entry* next = entry->next;
            int newIndex = hashFunction(entry->key) % newSize;
            entry->next = newTable[newIndex];
            newTable[newIndex] = entry;
            entry = next;
        }
    }
    delete[] table;
    table = newTable;
    TABLE_SIZE = newSize;
}

template <typename K, typename V, typename Hash>
int HashTable<K, V, Hash>::hash(const K& key) {
    return hashFunction(key) % TABLE_SIZE;
}

template <typename K, typename V, typename Hash>
void HashTable<K, V, Hash>::clear(Entry** table, int capacity) {
    for (int i = 0; i < capacity; ++i) {
        Entry* entry = table[i];
        while (entry != nullptr) {
            Entry* next = entry->next;
            delete entry;
            entry = next;
        }
        table[i] = nullptr;
    }
}

template <typename K, typename V, typename Hash>
typename HashTable<K, V, Hash>::HashtableIterator HashTable<K, V, Hash>::begin() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (table[i] != nullptr) {
            return HashtableIterator(this, i, table[i]);
        }
    }
    return end();
}

template <typename K, typename V, typename Hash>
typename HashTable<K, V, Hash>::HashtableIterator HashTable<K, V, Hash>::end() {
    return HashtableIterator(this, TABLE_SIZE, nullptr);
}

template <typename K, typename V, typename Hash>
typename HashTable<K, V, Hash>::HashtableIterator HashTable<K, V, Hash>::cbegin() const {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (table[i] != nullptr) {
            return HashtableIterator(this, i, table[i]);
        }
    }
    return cend();
}

template <typename K, typename V, typename Hash>
typename HashTable<K, V, Hash>::HashtableIterator HashTable<K, V, Hash>::cend() const {
    return HashtableIterator(this, TABLE_SIZE, nullptr);
}


#endif // HASHTABLE_H
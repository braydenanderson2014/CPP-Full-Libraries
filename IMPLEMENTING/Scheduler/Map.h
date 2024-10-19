#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <exception>

#include "SimpleVector.h"


template <typename K, typename V>
class Map {
private:
    struct MapNode {
        K key;
        V value;
        MapNode* next;
        MapNode(const K& k, const V& v, MapNode* n = nullptr) : key(k), value(v), next(n) {}
    };

    int Count = 0;
    MapNode* head;
public:

    Map();
    Map(const Map& other);
    Map(std::initializer_list<std::pair<const K, V>> init);

    Map& operator=(const Map& other);
    Map& operator=(std::initializer_list<std::pair<const K, V>> init);
    ~Map();
    bool insert(const K& key, const V& value);
    bool remove(const K& key);
    V& get(const K& key);
    const V& get(const K& key) const;
    size_t size() const;
    bool isEmpty() const;
    void clear();
    bool contains(const K& key) const;
    V& operator[](const K& key);
    const V& at(const K& key) const;
    bool containsValue(const V& value) const;
    void print(std::ostream& os) const;
    template <typename... Args>
    bool emplace(const K& key, Args&&... args);
    SimpleVector<K> Keys() const;
    SimpleVector<V> Values() const;
    

    class MapIterator {
    private:
        MapNode* current;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const K&, V&>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        MapIterator(MapNode* head) : current(head) {}
        MapIterator& operator++() {
            if (current != nullptr) {
                current = current->next;
            }
            return *this;
        }
        bool operator!=(const MapIterator& other) const {
        return current != other.current;
    }
    const std::pair<const K&, V&> operator*() const {
        return std::make_pair(std::cref(current->key), std::ref(current->value));
    }

        
    };
    MapIterator begin() { return MapIterator(head); }
    MapIterator end() { return MapIterator(nullptr); }
};
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

    #ifndef MAPEXCEPTION
    #define MAPEXCEPTION
        class MapException : public std::exception {
        public:
            explicit MapException(const std::string& message) : msg_(message) {}
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


template <typename K, typename V>
Map<K, V>::Map() : head(nullptr){}

template <typename K, typename V>
Map<K, V>::Map(const Map& other) : head(nullptr){
    MapNode* current = other.head;
    while(current != nullptr){
        insert(current->key, current->value);
        current = current->next;
    }
}

template <typename K, typename V>
Map<K, V>& Map<K, V>::operator=(const Map& other){
    if(this == &other){
        return *this;
    }
    clear();
    MapNode* current = other.head;
    while(current != nullptr){
        insert(current->key, current->value);
        current = current->next;
    }
    return *this;
}

template <typename K, typename V>
Map<K, V>::~Map(){
    while(head != nullptr){
        MapNode* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename K, typename V>
bool Map<K, V>::insert(const K& key, const V& value) {
    MapNode** pp = &head;
    while (*pp != nullptr && (*pp)->key != key) {
        pp = &(*pp)->next;
    }
    if (*pp != nullptr) {
        return false; // Key already exists
    }
    try {
        *pp = new MapNode(key, value);
        Count++;
        return true;
    } catch (const std::bad_alloc& e) {
        std::ostringstream oss;
        oss << "Memory allocation failed during insert operation. Key: " << key << ", Value: " << value << ". " << e.what();
        throw MapException(oss.str());    }
}

template <typename K, typename V>
bool Map<K, V>::remove(const K& key){
    MapNode** pp = &head;
    while(*pp != nullptr && (*pp)->key != key){
        pp = &(*pp)->next;
    }
    if(*pp == nullptr){
        return false;
    }
    MapNode* temp = *pp;
    *pp = (*pp)->next;
    delete temp;
    Count--;
    return true;
}

template <typename K, typename V>
V& Map<K, V>::get(const K& key) {
    MapNode* current = head;
    while(current != nullptr){
        if(current->key == key){
            return current->value;
        }
        current = current->next;
    }
    throw KeyNotFoundException("Key not found in map.");
}

template <typename K, typename V>
const V& Map<K, V>::get(const K& key) const {
    const MapNode* current = head;
    while(current != nullptr){
        if(current->key == key){
            return current->value;
        }
        current = current->next;
    }
    throw KeyNotFoundException("Key not found in map.");
}

template <typename K, typename V>
size_t Map<K, V>::size() const{
    return Count;
}

template <typename K, typename V>
bool Map<K, V>::isEmpty() const{
    return Count == 0;
}

template <typename K, typename V>
void Map<K, V>::clear(){
    while(head != nullptr){
        MapNode* temp = head;
        head = head->next;
        delete temp;
    }
    Count = 0;
}

template <typename K, typename V>
bool Map<K, V>::contains(const K& key) const{
    MapNode* current = head;
    while(current != nullptr){
        if(current->key == key){
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename K, typename V>
V& Map<K,V>::operator[](const K& key) {
    if (!contains(key)) {
        insert(key, V());
    }
    return get(key);
}

template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const {
    return get(key);
}

template <typename K, typename V>
typename Map<K, V>::MapIterator begin(Map<K, V>& m) { return m.begin(); }

template <typename K, typename V>
typename Map<K, V>::MapIterator end(Map<K, V>& m) { return m.end(); }

template <typename K, typename V>
bool Map<K,V>::containsValue(const V& value) const {
    for (MapNode* current = head; current != nullptr; current = current->next) {
        if (current->value == value) return true;
    }
    return false;
}

template <typename K, typename V>
void Map<K,V>::print(std::ostream& os) const {
    for (MapNode* current = head; current != nullptr; current = current->next) {
        os << current->key << ": " << current->value << "\n";
    }
}


template <typename K, typename V>
template <typename... Args>
bool Map<K,V>::emplace(const K& key, Args&&... args) {
    return insert(key, V(std::forward<Args>(args)...));
}

template <typename K, typename V>
SimpleVector<K> Map<K, V>::Keys() const {
    SimpleVector<K> keys;
    for (MapNode* current = head; current != nullptr; current = current->next) {
        keys.push_back(current->key);
    }
    return keys;
}

template <typename K, typename V>
SimpleVector<V> Map<K, V>::Values() const {
    SimpleVector<V> values;
    for (MapNode* current = head; current != nullptr; current = current->next) {
        values.push_back(current->value);
    }
    return values;
}


template <typename K, typename V>
Map<K, V>::Map(std::initializer_list<std::pair<const K, V>> init) : head(nullptr), Count(0) {
    for (const auto& pair : init) {
        insert(pair.first, pair.second);
    }
}

// Optionally, you can also add an assignment operator for initializer lists
template <typename K, typename V>
Map<K, V>& Map<K, V>::operator=(std::initializer_list<std::pair<const K, V>> init) {
    clear();
    for (const auto& pair : init) {
        insert(pair.first, pair.second);
    }
    return *this;
}
#endif // MAP_H
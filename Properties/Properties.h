#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "HashTable.h"
#include <string>
#include <iostream>
#include <fstream>

class Properties {
private:
    HashTable<std::string, std::string> hashtable;

public:
    enum FileTypes { Property, XML, JSON, YAML, CSV, INI, TOML, HOCON, HCL, DOTENV, ENV, CFG, CONF, CNF, LST, LIST, TEXT, TXT, OTHER };

    /**
     * @brief Constructs a new Properties object.
     * 
     * @details This constructor creates a new Properties object.
     * 
     */
    Properties() {}

    /**
     * @brief Sets a property in the Properties object.
     * 
     * @details This method sets a property in the Properties object.
     * 
     * @param key The key of the property.
     * @param value The value of the property.
     * 
     */
    void setProperty(const std::string& key, const std::string& value) {
        hashtable.insert(key, value);
    }

    /**
     * @brief Gets the value of a property.
     * 
     * @details This method gets the value of a property.
     * 
     * @param key The key of the property.
     * 
     * @return The value of the property.
     * 
     * @throw KeyNotFoundException if the key is not found.
     * 
     */
    std::string getProperty(const std::string& key) const {
        return hashtable.get(key);
    }

    /**
     * @brief Checks if a property exists.
     * 
     * @details This method checks if a property exists.
     * 
     * @param key The key of the property.
     * 
     * @return true if the property exists, false otherwise.
     * 
     */
    bool containsProperty(const std::string& key) const {
        return hashtable.contains(key);
    }

    /**
     * @brief Removes a property.
     * 
     * @details This method removes a property.
     * 
     * @param key The key of the property.
     * 
     * @throw KeyNotFoundException if the key is not found.
     * 
     */
    void removeProperty(const std::string& key) {
        hashtable.remove(key);
    }

    /**
     * @brief Gets the number of properties.
     * 
     * @details This method gets the number of properties.
     * 
     * @return The number of properties.
     * 
     */
    int size() const {
        return hashtable.size();
    }

    /**
     * @brief Checks if the Properties object is empty.
     * 
     * @details This method checks if the Properties object is empty.
     * 
     * @return true if the Properties object is empty, false otherwise.
     * 
     */
    bool isEmpty() const {
        return hashtable.isEmpty();
    }

    /**
     * @brief Clears the Properties object.
     * 
     * @details This method clears the Properties object.
     * 
     */
    void clear() {
        hashtable.clear();
    }

    /**
     * @brief Gets the keys in the Properties object.
     * 
     * @details This method gets the keys in the Properties object.
     * 
     * @return The keys in the Properties object.
     * 
     */
    SimpleVector<std::string> keys() const {
        return hashtable.getKeys();
    }

    /**
     * @brief Gets the values in the Properties object.
     * 
     * @details This method gets the values in the Properties object.
     * 
     * @return The values in the Properties object.
     * 
     */
    SimpleVector<std::string> values() const {
        return hashtable.getValues();
    }

    /**
     * @brief Destructor for the Properties object.
     * 
     * @details This destructor destroys the Properties object.
     * 
     */
    ~Properties() {
        hashtable.clear();
    }

    bool store(std::string filename, FileTypes fileType) {
        std::ofstream os(filename.c_str());
        if (!os.is_open()) {
            return false;
        }
        if (fileType == Property) {
            for (const auto& kv : hashtable) {
                os << kv.first() << "=" << kv.second() << std::endl;
            }
            
        } else if(fileType == XML) {
            for (const auto& kv : hashtable) {
                os << "  <property>" << std::endl;
                os << "    <key>" << kv.first() << "</key>" << std::endl;
                os << "    <value>" << kv.second() << "</value>" << std::endl;
                os << "  </property>" << std::endl;
            }
            
            os << "</properties>" << std::endl;
        } else if(fileType == JSON) {
            for (const auto& kv : hashtable) {
                os << "  \"" << kv.first() << "\": \"" << kv.second() << "\"," << std::endl;
            }
            
            os << "}" << std::endl;
        } else if(fileType == YAML) {
            for (const auto& kv : hashtable) {
                os << "  - key: " << kv.first() << std::endl;
                os << "    value: " << kv.second() << std::endl;
            }
            
            for (const auto& kv : hashtable) {
                os << kv.first() << "," << kv.second() << std::endl;
            }
            
        } else if(fileType == INI) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == TOML) {
            for (const auto& kv : hashtable) {
                os << kv.first() << " = \"" << kv.second() << "\"" << std::endl;
            }
            
        } else if(fileType == HOCON) {
            for (const auto& kv : hashtable) {
                os << "  " << kv.first() << " = \"" << kv.second() << "\"" << std::endl;
            }
            os << "}" << std::endl;
        } else if(fileType == HCL) {
            os << "properties = {" << std::endl;
            for (const auto& kv : hashtable) {
                os << "  " << kv.key << " = \"" << kv.value << "\"" << std::endl;
            }
            os << "}" << std::endl;
        } else if(fileType == DOTENV) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == ENV) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == CFG) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == CONF) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == CNF) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == LST) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == LIST) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == TEXT) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else if(fileType == TXT) {
            for (const auto& kv : hashtable) {
                os << kv.key << "=" << kv.value << std::endl;
            }
        } else {
            return false;
        }
        return true;
    }

    bool load(std::istream& is, FileTypes fileType) {
        std::string line;
        if (fileType == Property || fileType == INI || fileType == DOTENV || fileType == ENV || fileType == CFG || fileType == CONF || fileType == CNF || fileType == LST || fileType == LIST || fileType == TEXT || fileType == TXT) {
            while (std::getline(is, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    hashtable[key] = value;
                }
            }
        } else if (fileType == XML) {
            // XML parsing logic
            // This is a simplified example. You might want to use an XML parser library for robust parsing.
            while (std::getline(is, line)) {
                if (line.find("<key>") != std::string::npos) {
                    std::string key = line.substr(line.find("<key>") + 5, line.find("</key>") - line.find("<key>") - 5);
                    std::getline(is, line);
                    std::string value = line.substr(line.find("<value>") + 7, line.find("</value>") - line.find("<value>") - 7);
                    hashtable[key] = value;
                }
            }
            
        } else if (fileType == JSON) {
            // JSON parsing logic
            // This is a simplified example. You might want to use a JSON parser library for robust parsing.
            while (std::getline(is, line)) {
                size_t keyStart = line.find('"') + 1;
                size_t keyEnd = line.find('"', keyStart);
                std::string key = line.substr(keyStart, keyEnd - keyStart);
                size_t valueStart = line.find('"', keyEnd + 1) + 1;
                size_t valueEnd = line.find('"', valueStart);
                std::string value = line.substr(valueStart, valueEnd - valueStart);
                hashtable[key] = value;
            }
        } else if (fileType == YAML) {
            // YAML parsing logic
            // This is a simplified example. You might want to use a YAML parser library for robust parsing.
            while (std::getline(is, line)) {
                if (line.find("key:") != std::string::npos) {
                    std::string key = line.substr(line.find("key:") + 5);
                    std::getline(is, line);
                    std::string value = line.substr(line.find("value:") + 7);
                    hashtable[key] = value;
                }
            }
        } else if (fileType == CSV) {
            while (std::getline(is, line)) {
                size_t pos = line.find(',');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    hashtable[key] = value;
                }
            }
        } else if (fileType == TOML) {
            // TOML parsing logic
            // This is a simplified example. You might want to use a TOML parser library for robust parsing.
            while (std::getline(is, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos - 1);
                    std::string value = line.substr(pos + 3, line.length() - pos - 4);
                    hashtable[key] = value;
                }
            }
        } else if (fileType == HOCON || fileType == HCL) {
            // HOCON and HCL parsing logic
            // This is a simplified example. You might want to use a HOCON or HCL parser library for robust parsing.
            while (std::getline(is, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos - 1);
                    std::string value = line.substr(pos + 3, line.length() - pos - 4);
                    hashtable[key] = value;
                }
            }
        } else {
            return false;
        }
        return true;
    }
    
    // The Properties object needs to be compatible with STL-style iteration
    class PropertiesIterator {
    private:
        HashTable<std::string, std::string>::HashtableIterator iterator;    

    public:
        PropertiesIterator(HashTable<std::string, std::string>::HashtableIterator it)
        : iterator(std::move(it)) {}


        std::pair<std::string, std::string> operator*() {
            auto kv = *iterator;
            return std::make_pair(kv.key, kv.value);
        }   

        PropertiesIterator& operator++() {
            ++iterator;
            return *this;
        }   

        bool operator!=(const PropertiesIterator& other) const {
            return iterator != other.iterator;
        }

        
    };

    PropertiesIterator begin() const {
        return PropertiesIterator(hashtable.cbegin());
    }

    PropertiesIterator end() const {
        return PropertiesIterator(hashtable.cend());
    }
    
};
#endif // PROPERTIES_H
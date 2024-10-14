#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "HashTable.h"
#include <string>
#include <iostream>
#include <fstream>

class Properties {
private:
    HashTable<std::string, std::string> hashtable;

    bool loadPropertyFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadXMLFormat(std::istream& is) {
        std::string line, key, value;
        bool inProperty = false;
        while (std::getline(is, line)) {
            // Trim leading and trailing whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);


            if (line == "<property>") {
                inProperty = true;
                key.clear();
                value.clear();
            } else if (line == "</property>") {
                inProperty = false;
                if (!key.empty() && !value.empty()) {
                    setProperty(key, value);
                }
            } else if (line.find("<key>") != std::string::npos) {
                size_t start = line.find(">") + 1;
                size_t end = line.find("</key>");
                if (end != std::string::npos) {
                    key = line.substr(start, end - start);
                }
            } else if (line.find("<value>") != std::string::npos) {
                size_t start = line.find(">") + 1;
                size_t end = line.find("</value>");
                if (end != std::string::npos) {
                    value = line.substr(start, end - start);
                }
            }
        }
        return true;
    }
    bool loadJSONFormat(std::istream& is) {
        std::string jsonContent((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        size_t pos = 0;
        while ((pos = jsonContent.find("\"", pos)) != std::string::npos) {
            size_t keyEnd = jsonContent.find("\"", pos + 1);
            if (keyEnd == std::string::npos) break;
            std::string key = jsonContent.substr(pos + 1, keyEnd - pos - 1);

            pos = jsonContent.find("\"", keyEnd + 1);
            if (pos == std::string::npos) break;
            size_t valueEnd = jsonContent.find("\"", pos + 1);
            if (valueEnd == std::string::npos) break;
            std::string value = jsonContent.substr(pos + 1, valueEnd - pos - 1);

            setProperty(key, value);
            pos = valueEnd + 1;
        }
        return true;
    }

    bool loadYAMLFormat(std::istream& is) {
        std::string line, key, value;
        bool inProperty = false, inKey = false, inValue = false;
        while (std::getline(is, line)) {
            if (line.find("- key:") != std::string::npos) {
                inProperty = true;
                key.clear();
                value.clear();
            } else if (line.find("- value:") != std::string::npos) {
                inProperty = false;
                if (!key.empty() && !value.empty()) {
                    setProperty(key, value);
                }
            } else if (line.find("key:") != std::string::npos) {
                inKey = true;
            } else if (line.find("value:") != std::string::npos) {
                inValue = true;
            } else if (inKey) {
                key += line;
            } else if (inValue) {
                value += line;
            }
        }
        return true;
    }

    bool loadINIFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadTOMLFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadHOCONFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadHCLFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadDOTENVFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadENVFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadCFGFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadCONFFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadCNFFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadLSTFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find(',');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadLISTFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find(',');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadTEXTFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find(',');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadTXTFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find(',');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }

    bool loadCSVFormat(std::istream& is) {
        std::string line;
        while (std::getline(is, line)) {
            size_t pos = line.find(',');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                setProperty(key, value);
            }
        }
        return true;
    }



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
            os << "<properties>" << std::endl;
            for (const auto& [key, value] : *this) {
                os << "  <property>" << std::endl
                    << "    <key>" << key << "</key>" << std::endl
                    << "    <value>" << value << "</value>" << std::endl
                    << "  </property>" << std::endl;
            }
            os << "</properties>" << std::endl;
            return true;
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


    bool Properties::store(std::ostream& out, FileTypes fileType) const {
        switch (fileType) {
            case FileTypes::Property:
            case FileTypes::INI:
            case FileTypes::DOTENV:
            case FileTypes::ENV:
            case FileTypes::CFG:
            case FileTypes::CONF:
            case FileTypes::CNF:
            case FileTypes::LST:
            case FileTypes::LIST:
            case FileTypes::TEXT:
            case FileTypes::TXT:
                for (const auto& [key, value] : *this) {
                    out << key << "=" << value << std::endl;
                }
                return true;
            case FileTypes::XML:
                out << "<properties>" << std::endl;
                for (const auto& [key, value] : *this) {
                    out << "  <property>" << std::endl
                        << "    <key>" << key << "</key>" << std::endl
                        << "    <value>" << value << "</value>" << std::endl
                        << "  </property>" << std::endl;
                }
                out << "</properties>" << std::endl;
                return true;
            case FileTypes::JSON:
                out << "{" << std::endl;
                {
                    auto it = this->begin();
                    auto end = this->end();
                    while (it != end) {
                        const auto& [key, value] = *it;
                        out << "  \"" << key << "\": \"" << value << "\"";
                        if (++it != end) out << ",";
                        out << std::endl;
                    }
                }
                out << "}" << std::endl;
            break;

            case FileTypes::YAML:
                for (auto it = hashtable.cbegin(); it != hashtable.cend(); ++it) {
                    out << it->first() << ": " << it->second() << std::endl;
                }
                break;

            case FileTypes::TOML:
                for (auto it = hashtable.cbegin(); it != hashtable.cend(); ++it) {
                    out << it->first() << " = \"" << it->second() << "\"" << std::endl;
                }
                break;

            case FileTypes::HOCON:
                out << "{" << std::endl;
                for (auto it = hashtable.cbegin(); it != hashtable.cend(); ++it) {
                    out << "  " << it->first() << " = \"" << it->second() << "\"" << std::endl;
                }
                out << "}" << std::endl;
                break;

            case FileTypes::HCL:
                out << "properties = {" << std::endl;
                for (auto it = hashtable.cbegin(); it != hashtable.cend(); ++it) {
                    out << "  " << it->first() << " = \"" << it->second() << "\"" << std::endl;
                }
                out << "}" << std::endl;
                break;

            default:
                return false;
        }
        return true;
    }
    bool load(std::string filename, FileTypes fileType) {
        std::ifstream is(filename.c_str());
        if (!is.is_open()) {
            return false;
        }
        return load(is, fileType);
    }


    bool Properties::load(std::istream& is, FileTypes fileType) {
        clear();
        std::string line;

        switch(fileType) {
            case FileTypes::Property:
            case FileTypes::INI:
            case FileTypes::DOTENV:
            case FileTypes::ENV:
            case FileTypes::CFG:
            case FileTypes::CONF:
            case FileTypes::CNF:
            case FileTypes::LST:
            case FileTypes::LIST:
            case FileTypes::TEXT:
            case FileTypes::TXT:
                return loadPropertyFormat(is);

            case FileTypes::XML:
                return loadXMLFormat(is);

            case FileTypes::JSON:
                return loadJSONFormat(is);

            case FileTypes::YAML:
                return loadYAMLFormat(is);

            case FileTypes::TOML:
                return loadTOMLFormat(is);

            case FileTypes::HOCON:
            case FileTypes::HCL:
                return loadHOCONFormat(is);

            case FileTypes::CSV:
                return loadCSVFormat(is);
            default:
                return false;
        }
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
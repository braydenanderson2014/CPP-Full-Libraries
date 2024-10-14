#include <gtest/gtest.h>
#include "Properties.h"
#include <sstream>

class PropertiesTest : public ::testing::Test {
protected:
    Properties props;

    void SetUp() override {
        // Set up some initial properties
        props.setProperty("key1", "value1");
        props.setProperty("key2", "value2");
        props.setProperty("key3", "value3");
    }
};

TEST_F(PropertiesTest, SetAndGetProperty) {
    EXPECT_EQ(props.getProperty("key1"), "value1");
    EXPECT_EQ(props.getProperty("key2"), "value2");
    EXPECT_EQ(props.getProperty("key3"), "value3");

    props.setProperty("key4", "value4");
    EXPECT_EQ(props.getProperty("key4"), "value4");
}

TEST_F(PropertiesTest, ContainsProperty) {
    EXPECT_TRUE(props.containsProperty("key1"));
    EXPECT_TRUE(props.containsProperty("key2"));
    EXPECT_TRUE(props.containsProperty("key3"));
    EXPECT_FALSE(props.containsProperty("nonexistent"));
}

TEST_F(PropertiesTest, RemoveProperty) {
    props.removeProperty("key2");
    EXPECT_FALSE(props.containsProperty("key2"));
    EXPECT_THROW(props.getProperty("key2"), KeyNotFoundException);
}

TEST_F(PropertiesTest, Size) {
    EXPECT_EQ(props.size(), 3);
    props.setProperty("key4", "value4");
    EXPECT_EQ(props.size(), 4);
    props.removeProperty("key1");
    EXPECT_EQ(props.size(), 3);
}

TEST_F(PropertiesTest, IsEmpty) {
    EXPECT_FALSE(props.isEmpty());
    props.clear();
    EXPECT_TRUE(props.isEmpty());
}

TEST_F(PropertiesTest, Clear) {
    props.clear();
    EXPECT_TRUE(props.isEmpty());
    EXPECT_EQ(props.size(), 0);
}

TEST_F(PropertiesTest, Keys) {
    auto keys = props.keys();
    EXPECT_EQ(keys.elements(), 3);
    EXPECT_TRUE(keys.contains("key1"));
    EXPECT_TRUE(keys.contains("key2"));
    EXPECT_TRUE(keys.contains("key3"));
}

TEST_F(PropertiesTest, Values) {
    auto values = props.values();
    EXPECT_EQ(values.elements(), 3);
    EXPECT_TRUE(values.contains("value1"));
    EXPECT_TRUE(values.contains("value2"));
    EXPECT_TRUE(values.contains("value3"));
}

TEST_F(PropertiesTest, Iterator) {
    int count = 0;
    for (const auto& [key, value] : props) {
        EXPECT_TRUE(props.containsProperty(key));
        EXPECT_EQ(props.getProperty(key), value);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(PropertiesTest, StoreAndLoadProperty) {
    std::stringstream ss;
    EXPECT_TRUE(props.store(ss, Properties::FileTypes::Property));
    
    Properties newProps;
    ss.seekg(0);  // Reset stringstream position to the beginning
    EXPECT_TRUE(newProps.load(ss, Properties::FileTypes::Property));
    
    EXPECT_EQ(newProps.size(), props.size());
    EXPECT_EQ(newProps.getProperty("key1"), "value1");
    EXPECT_EQ(newProps.getProperty("key2"), "value2");
    EXPECT_EQ(newProps.getProperty("key3"), "value3");
}

TEST_F(PropertiesTest, StoreAndLoadXML) {
    std::stringstream ss;
    EXPECT_TRUE(props.store(ss, Properties::FileTypes::XML));
    

    Properties newProps;
    ss.seekg(0);  // Reset stringstream position to the beginning
    EXPECT_TRUE(newProps.load(ss, Properties::FileTypes::XML));
    

    EXPECT_EQ(newProps.size(), props.size());
    EXPECT_EQ(newProps.getProperty("key1"), "value1");
    EXPECT_EQ(newProps.getProperty("key2"), "value2");
    EXPECT_EQ(newProps.getProperty("key3"), "value3");
}
TEST_F(PropertiesTest, StoreAndLoadJSON) {
    std::stringstream ss;
    EXPECT_TRUE(props.store(ss, Properties::FileTypes::JSON));
    
    Properties newProps;
    EXPECT_TRUE(newProps.load(ss, Properties::FileTypes::JSON));
    
    EXPECT_EQ(newProps.size(), props.size());
    EXPECT_EQ(newProps.getProperty("key1"), "value1");
    EXPECT_EQ(newProps.getProperty("key2"), "value2");
    EXPECT_EQ(newProps.getProperty("key3"), "value3");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
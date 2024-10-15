#include <gtest/gtest.h>
#include "Map.h" // Make sure this path is correct
#include <string>

class MapTest : public ::testing::Test {
protected:
    Map<std::string, int> map;

    void SetUp() override {
        // Set up code here
    }

    void TearDown() override {
        // Clean up code here
    }
};

TEST_F(MapTest, InsertAndGet) {
    EXPECT_TRUE(map.insert("one", 1));
    EXPECT_TRUE(map.insert("two", 2));
    EXPECT_EQ(map.get("one"), 1);
    EXPECT_EQ(map.get("two"), 2);
}

TEST_F(MapTest, InsertDuplicate) {
    EXPECT_TRUE(map.insert("one", 1));
    EXPECT_FALSE(map.insert("one", 2));
    EXPECT_EQ(map.get("one"), 1);
}

TEST_F(MapTest, Remove) {
    map.insert("one", 1);
    map.insert("two", 2);
    EXPECT_TRUE(map.remove("one"));
    EXPECT_FALSE(map.contains("one"));
    EXPECT_TRUE(map.contains("two"));
}

TEST_F(MapTest, Size) {
    EXPECT_EQ(map.size(), 0);
    map.insert("one", 1);
    EXPECT_EQ(map.size(), 1);
    map.insert("two", 2);
    EXPECT_EQ(map.size(), 2);
    map.remove("one");
    EXPECT_EQ(map.size(), 1);
}

TEST_F(MapTest, IsEmpty) {
    EXPECT_TRUE(map.isEmpty());
    map.insert("one", 1);
    EXPECT_FALSE(map.isEmpty());
}

TEST_F(MapTest, Clear) {
    map.insert("one", 1);
    map.insert("two", 2);
    map.clear();
    EXPECT_TRUE(map.isEmpty());
    EXPECT_EQ(map.size(), 0);
}

TEST_F(MapTest, Contains) {
    map.insert("one", 1);
    EXPECT_TRUE(map.contains("one"));
    EXPECT_FALSE(map.contains("two"));
}

TEST_F(MapTest, OperatorBrackets) {
    map["one"] = 1;
    EXPECT_EQ(map["one"], 1);
    map["one"] = 2;
    EXPECT_EQ(map["one"], 2);
}

TEST_F(MapTest, At) {
    map.insert("one", 1);
    EXPECT_EQ(map.at("one"), 1);
    EXPECT_THROW(map.at("two"), KeyNotFoundException);
}

TEST_F(MapTest, ContainsValue) {
    map.insert("one", 1);
    map.insert("two", 2);
    EXPECT_TRUE(map.containsValue(1));
    EXPECT_TRUE(map.containsValue(2));
    EXPECT_FALSE(map.containsValue(3));
}

TEST_F(MapTest, Emplace) {
    EXPECT_TRUE(map.emplace("one", 1));
    EXPECT_EQ(map.get("one"), 1);
}

TEST_F(MapTest, Keys) {
    map.insert("one", 1);
    map.insert("two", 2);
    auto keys = map.Keys();
    EXPECT_EQ(keys.elements(), 2);
    EXPECT_TRUE(keys.contains("one"));
    EXPECT_TRUE(keys.contains("two"));
}

TEST_F(MapTest, Values) {
    map.insert("one", 1);
    map.insert("two", 2);
    auto values = map.Values();
    EXPECT_EQ(values.elements(), 2);
    EXPECT_TRUE(values.contains(1));
    EXPECT_TRUE(values.contains(2));
}

TEST_F(MapTest, Iterator) {
    map.insert("one", 1);
    map.insert("two", 2);
    int sum = 0;
    for (const auto& pair : map) {
        sum += pair.second;
    }
    EXPECT_EQ(sum, 3);
}

// Run all the tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
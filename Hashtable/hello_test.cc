#include <gtest/gtest.h>
#include "HashTable.h" // Make sure this path is correct
#include "SimpleVector.h"

class HashTableTest : public ::testing::Test {
protected:
    HashTable<std::string, int> ht;

    void SetUp() override {
        // This method will be called before each test
        ht.clear();
    }
};

TEST_F(HashTableTest, InsertAndGet) {
    ht.insert("one", 1);
    ht.insert("two", 2);
    ht.insert("three", 3);

    EXPECT_EQ(ht.get("one"), 1);
    EXPECT_EQ(ht.get("two"), 2);
    EXPECT_EQ(ht.get("three"), 3);
}

TEST_F(HashTableTest, Update) {
    ht.insert("key", 1);
    EXPECT_EQ(ht.get("key"), 1);

    ht.insert("key", 2);
    EXPECT_EQ(ht.get("key"), 2);
}

TEST_F(HashTableTest, Remove) {
    ht.insert("key", 1);
    EXPECT_TRUE(ht.contains("key"));

    ht.remove("key");
    EXPECT_FALSE(ht.contains("key"));

    EXPECT_THROW(ht.get("key"), KeyNotFoundException);
}

TEST_F(HashTableTest, Contains) {
    EXPECT_FALSE(ht.contains("key"));

    ht.insert("key", 1);
    EXPECT_TRUE(ht.contains("key"));
}

TEST_F(HashTableTest, SizeAndEmpty) {
    EXPECT_TRUE(ht.isEmpty());
    EXPECT_EQ(ht.size(), 0);

    ht.insert("one", 1);
    EXPECT_FALSE(ht.isEmpty());
    EXPECT_EQ(ht.size(), 1);

    ht.insert("two", 2);
    EXPECT_EQ(ht.size(), 2);

    ht.remove("one");
    EXPECT_EQ(ht.size(), 1);

    ht.remove("two");
    EXPECT_TRUE(ht.isEmpty());
    EXPECT_EQ(ht.size(), 0);
}

TEST_F(HashTableTest, Clear) {
    ht.insert("one", 1);
    ht.insert("two", 2);
    ht.insert("three", 3);

    ht.clear();
    EXPECT_TRUE(ht.isEmpty());
    EXPECT_EQ(ht.size(), 0);
    EXPECT_FALSE(ht.contains("one"));
    EXPECT_FALSE(ht.contains("two"));
    EXPECT_FALSE(ht.contains("three"));
}

TEST_F(HashTableTest, Iterator) {
    ht.insert("one", 1);
    ht.insert("two", 2);
    ht.insert("three", 3);

    int count = 0;
    for (const auto& kv : ht) {
        EXPECT_TRUE(ht.contains(kv.key));
        EXPECT_EQ(ht.get(kv.key), kv.value);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(HashTableTest, GetKeysAndValues) {
    ht.insert("one", 1);
    ht.insert("two", 2);
    ht.insert("three", 3);

    auto it = ht.begin();
    auto keys = it.getKeys();
    auto values = it.getValues();

    EXPECT_EQ(keys.elements(), 3);
    EXPECT_EQ(values.elements(), 3);

    for (unsigned int i = 0; i < keys.elements(); i++) {
        EXPECT_TRUE(ht.contains(keys[i]));
        EXPECT_EQ(ht.get(keys[i]), values[i]);
    }
}
TEST(HashTableDifferentTypes, IntStringHashTable) {
    HashTable<int, std::string> ht;
    ht.insert(1, "one");
    EXPECT_EQ(ht.get(1), "one");
}

TEST(HashTableDifferentTypes, DoubleBoolHashTable) {
    HashTable<double, bool> ht;
    ht.insert(3.14, true);
    EXPECT_TRUE(ht.get(3.14));
}

TEST(HashTableDifferentTypes, CharFloatHashTable) {
    HashTable<char, float> ht;
    ht.insert('a', 1.5f);
    EXPECT_FLOAT_EQ(ht.get('a'), 1.5f);
}

TEST_F(HashTableTest, EdgeCases) {
    // Test with empty string key
    ht.insert("", 0);
    EXPECT_EQ(ht.get(""), 0);

    // Test with very long string key
    std::string longKey(1000, 'a');
    ht.insert(longKey, 1000);
    EXPECT_EQ(ht.get(longKey), 1000);

    // Test removing non-existent key
    EXPECT_THROW(ht.remove("non-existent"), KeyNotFoundException);

    // Test getting non-existent key
    EXPECT_THROW(ht.get("non-existent"), KeyNotFoundException);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
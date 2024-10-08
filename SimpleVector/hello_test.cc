#include <gtest/gtest.h>
#include "SimpleVector.h" // Make sure this path is correct
#include <string>

class SimpleVectorTest : public ::testing::Test {
protected:
    SimpleVector<int>* intVector;
    SimpleVector<std::string>* stringVector;

    void SetUp() override {
        intVector = new SimpleVector<int>();
        stringVector = new SimpleVector<std::string>();
    }

    void TearDown() override {
        delete intVector;
        delete stringVector;
    }
};

TEST_F(SimpleVectorTest, DefaultConstructor) {
    EXPECT_EQ(intVector->size(), 4);
    EXPECT_EQ(intVector->elements(), 0);
}

TEST_F(SimpleVectorTest, InitializerListConstructor) {
    SimpleVector<int> vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(vec.elements(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
}

TEST_F(SimpleVectorTest, CopyConstructor) {
    // Test copying a non-empty vector
    intVector->push_back(1);
    intVector->push_back(2);
    SimpleVector<int> copiedVector(*intVector);
    EXPECT_EQ(copiedVector.elements(), 2);
    EXPECT_EQ(copiedVector[0], 1);
    EXPECT_EQ(copiedVector[1], 2);

    // Test copying an empty vector
    SimpleVector<int> emptyVector;
    EXPECT_THROW({
        SimpleVector<int> copiedEmptyVector(emptyVector);
    }, SimpleVectorException);

    // Test copying a vector at full capacity
    SimpleVector<int> fullVector(2);
    fullVector.push_back(1);
    fullVector.push_back(2);
    SimpleVector<int> copiedFullVector(fullVector);
    EXPECT_EQ(copiedFullVector.elements(), 2);
    EXPECT_EQ(copiedFullVector[0], 1);
    EXPECT_EQ(copiedFullVector[1], 2);

    // Modify the original vector and check that the copy is independent
    intVector->push_back(3);
    EXPECT_EQ(copiedVector.elements(), 2);
    EXPECT_EQ(intVector->elements(), 3);
}

TEST_F(SimpleVectorTest, PushBack) {
    intVector->push_back(1);
    EXPECT_EQ(intVector->elements(), 1);
    EXPECT_EQ((*intVector)[0], 1);
}

TEST_F(SimpleVectorTest, Put) {
    intVector->put(1);
    EXPECT_EQ(intVector->elements(), 1);
    EXPECT_EQ((*intVector)[0], 1);
}

TEST_F(SimpleVectorTest, BulkAdd) {
    intVector->bulk_add(1, 2, 3, 4, 5);
    EXPECT_EQ(intVector->elements(), 5);
    EXPECT_EQ((*intVector)[0], 1);
    EXPECT_EQ((*intVector)[1], 2);
    EXPECT_EQ((*intVector)[2], 3);
    EXPECT_EQ((*intVector)[3], 4);
    EXPECT_EQ((*intVector)[4], 5);

    // Test adding more elements
    intVector->bulk_add(6, 7, 8);
    EXPECT_EQ(intVector->elements(), 8);
    EXPECT_EQ((*intVector)[5], 6);
    EXPECT_EQ((*intVector)[6], 7);
    EXPECT_EQ((*intVector)[7], 8);

    // Test adding a single element
    intVector->bulk_add(9);
    EXPECT_EQ(intVector->elements(), 9);
    EXPECT_EQ((*intVector)[8], 9);

    // Test adding different types (if your SimpleVector supports it)
    SimpleVector<std::string>* stringVector = new SimpleVector<std::string>();
    stringVector->bulk_add("Hello", "World");
    EXPECT_EQ(stringVector->elements(), 2);
    EXPECT_EQ((*stringVector)[0], "Hello");
    EXPECT_EQ((*stringVector)[1], "World");
    delete stringVector;
}

TEST_F(SimpleVectorTest, EmplaceBack) {
    intVector->emplace_back(1);
    EXPECT_EQ(intVector->elements(), 1);
    EXPECT_EQ((*intVector)[0], 1);
}

TEST_F(SimpleVectorTest, FrontAndBack) {
    intVector->push_back(1);
    intVector->push_back(2);
    EXPECT_EQ(intVector->front(), 1);
    EXPECT_EQ(intVector->back(), 2);
}

TEST_F(SimpleVectorTest, Remove) {
    intVector->push_back(1);
    intVector->push_back(2);
    intVector->push_back(3);
    intVector->remove(2);
    EXPECT_EQ(intVector->elements(), 2);
    EXPECT_EQ((*intVector)[0], 1);
    EXPECT_EQ((*intVector)[1], 3);
}

TEST_F(SimpleVectorTest, Clear) {
    intVector->push_back(1);
    intVector->push_back(2);
    intVector->clear();
    EXPECT_EQ(intVector->elements(), 0);
}

TEST_F(SimpleVectorTest, ShrinkToFit) {
    for (int i = 0; i < 10; i++) {
        intVector->push_back(i);
    }
    intVector->shrinkToFit();
    EXPECT_EQ(intVector->size(), intVector->elements());
}

TEST_F(SimpleVectorTest, IndexOf) {
    intVector->push_back(1);
    intVector->push_back(2);
    intVector->push_back(3);
    EXPECT_EQ(intVector->indexOf(2), 1);
    EXPECT_EQ(intVector->indexOf(4), -1);
}

TEST_F(SimpleVectorTest, IsEmpty) {
    EXPECT_TRUE(intVector->isEmpty());
    intVector->push_back(1);
    EXPECT_FALSE(intVector->isEmpty());
}

TEST_F(SimpleVectorTest, Iterator) {
    intVector->push_back(1);
    intVector->push_back(2);
    intVector->push_back(3);
    int sum = 0;
    for (SimpleVector<int>::iterator it = intVector->begin(); it != intVector->end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(SimpleVectorTest, OutOfBoundsAccess) {
    EXPECT_THROW(intVector->get(0), IndexOutOfBoundsException);
}

TEST_F(SimpleVectorTest, Resize) {
    for (int i = 0; i < 10; i++) {
        intVector->push_back(i);
    }
    EXPECT_GE(intVector->size(), 10);
}

TEST_F(SimpleVectorTest, StringVector) {
    stringVector->push_back("Hello");
    stringVector->push_back("World");
    EXPECT_EQ(stringVector->elements(), 2);
    EXPECT_EQ((*stringVector)[0], "Hello");
    EXPECT_EQ((*stringVector)[1], "World");
}
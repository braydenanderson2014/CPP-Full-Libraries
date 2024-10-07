#include <gtest/gtest.h>
#include "ArrayList.h"
#include <string>

class ArrayListTest : public ::testing::Test {
protected:
    ArrayList<int>* list;

    void SetUp() override {
        list = new ArrayList<int>();
    }

    void TearDown() override {
        delete list;
    }
};

TEST_F(ArrayListTest, Constructor) {
    EXPECT_EQ(list->size(), 0);
    EXPECT_EQ(list->capacity(), 8);
}

TEST_F(ArrayListTest, Add) {
    list->add(1);
    EXPECT_EQ(list->size(), 1);
    EXPECT_EQ(list->get(0), 1);
}

TEST_F(ArrayListTest, AddAll) {
    ArrayList<int> other;
    other.add(1);
    other.add(2);
    list->addAll(other);
    EXPECT_EQ(list->size(), 2);
    EXPECT_EQ(list->get(0), 1);
    EXPECT_EQ(list->get(1), 2);
}

TEST_F(ArrayListTest, Insert) {
    list->add(1);
    list->add(3);
    list->insert(1, 2);
    EXPECT_EQ(list->size(), 3);
    EXPECT_EQ(list->get(1), 2);
}

TEST_F(ArrayListTest, Remove) {
    list->add(1);
    list->add(2);
    list->add(3);
    list->remove(1);
    EXPECT_EQ(list->size(), 2);
    EXPECT_EQ(list->get(1), 3);
}

TEST_F(ArrayListTest, RemoveIf) {
    list->add(1);
    list->add(2);
    list->add(3);
    list->removeIf([](int x) { return x % 2 == 0; });
    EXPECT_EQ(list->size(), 2);
    EXPECT_EQ(list->get(0), 1);
    EXPECT_EQ(list->get(1), 3);
}

TEST_F(ArrayListTest, Clear) {
    list->add(1);
    list->add(2);
    list->clear();
    EXPECT_EQ(list->size(), 0);
}

TEST_F(ArrayListTest, Contains) {
    list->add(1);
    list->add(2);
    EXPECT_TRUE(list->contains(1));
    EXPECT_FALSE(list->contains(3));
}

TEST_F(ArrayListTest, IndexOf) {
    list->add(1);
    list->add(2);
    list->add(1);
    EXPECT_EQ(list->indexOf(1), 0);  // First occurrence of 1
    EXPECT_EQ(list->indexOf(2), 1);  // Only occurrence of 2
    EXPECT_EQ(list->indexOf(3), -1); // 3 is not in the list
}

TEST_F(ArrayListTest, Set) {
    list->add(1);
    list->set(0, 2);
    EXPECT_EQ(list->get(0), 2);
}

TEST_F(ArrayListTest, Sublist) {
    list->add(1);
    list->add(2);
    list->add(3);
    list->add(4);
    ArrayList<int>* sublist = list->sublist(1, 3);
    EXPECT_EQ(sublist->size(), 2);
    EXPECT_EQ(sublist->get(0), 2);
    EXPECT_EQ(sublist->get(1), 3);
    delete sublist;
}

TEST_F(ArrayListTest, Clone) {
    list->add(1);
    list->add(2);
    ArrayList<int>* clone = list->clone();
    EXPECT_EQ(clone->size(), 2);
    EXPECT_EQ(clone->get(0), 1);
    EXPECT_EQ(clone->get(1), 2);
    delete clone;
}

TEST_F(ArrayListTest, Sort) {
    list->add(3);
    list->add(1);
    list->add(4);
    list->add(2);
    list->sort([](int a, int b) { return a < b; });
    EXPECT_EQ(list->get(0), 1);
    EXPECT_EQ(list->get(1), 2);
    EXPECT_EQ(list->get(2), 3);
    EXPECT_EQ(list->get(3), 4);
}

TEST_F(ArrayListTest, Reverse) {
    list->add(1);
    list->add(2);
    list->add(3);
    list->reverse();
    EXPECT_EQ(list->get(0), 3);
    EXPECT_EQ(list->get(1), 2);
    EXPECT_EQ(list->get(2), 1);
}

TEST_F(ArrayListTest, Iterator) {
    list->add(1);
    list->add(2);
    list->add(3);
    int sum = 0;
    for (int* it = list->begin(); it != list->end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(ArrayListTest, EnsureCapacity) {
    list->ensureCapacity(20);
    EXPECT_GE(list->capacity(), 20);
}

TEST_F(ArrayListTest, TrimToSize) {
    list->add(1);
    list->add(2);
    list->trimToSize();
    EXPECT_EQ(list->size(), list->capacity());
}

TEST_F(ArrayListTest, FixedSizeConstructor) {
    ArrayList<int> fixedList(ArrayList<int>::FIXED, 5);
    EXPECT_EQ(fixedList.capacity(), 5);
    for (int i = 0; i < 5; ++i) {
        fixedList.add(i);
    }
    EXPECT_THROW(fixedList.add(5), ArrayListException);
}

TEST_F(ArrayListTest, CopyConstructor) {
    list->add(1);
    list->add(2);
    ArrayList<int> copiedList(*list);
    EXPECT_EQ(copiedList.size(), 2);
    EXPECT_EQ(copiedList.get(0), 1);
    EXPECT_EQ(copiedList.get(1), 2);
}

TEST_F(ArrayListTest, OperatorEquals) {
    list->add(1);
    list->add(2);
    ArrayList<int> otherList;
    otherList = *list;
    EXPECT_EQ(otherList.size(), 2);
    EXPECT_EQ(otherList.get(0), 1);
    EXPECT_EQ(otherList.get(1), 2);
}

TEST_F(ArrayListTest, OperatorSquareBrackets) {
    list->add(1);
    list->add(2);
    EXPECT_EQ((*list)[0], 1);
    EXPECT_EQ((*list)[1], 2);
    (*list)[0] = 3;
    EXPECT_EQ((*list)[0], 3);
}

// Add these new tests to your existing test file

// Helper function for custom comparison
bool descending(int a, int b) {
    return a > b;
}

TEST_F(ArrayListTest, BubbleSortAscending) {
    list->add(5);
    list->add(2);
    list->add(8);
    list->add(1);
    list->add(9);
    list->bubbleSort([](int a, int b) { return a < b; });
    EXPECT_EQ(list->get(0), 1);
    EXPECT_EQ(list->get(1), 2);
    EXPECT_EQ(list->get(2), 5);
    EXPECT_EQ(list->get(3), 8);
    EXPECT_EQ(list->get(4), 9);
}

TEST_F(ArrayListTest, BubbleSortDescending) {
    list->add(5);
    list->add(2);
    list->add(8);
    list->add(1);
    list->add(9);
    list->bubbleSort(descending);
    EXPECT_EQ(list->get(0), 9);
    EXPECT_EQ(list->get(1), 8);
    EXPECT_EQ(list->get(2), 5);
    EXPECT_EQ(list->get(3), 2);
    EXPECT_EQ(list->get(4), 1);
}

TEST_F(ArrayListTest, QuickSortAscending) {
    list->add(5);
    list->add(2);
    list->add(8);
    list->add(1);
    list->add(9);
    list->quickSort([](int a, int b) { return a < b; });
    EXPECT_EQ(list->get(0), 1);
    EXPECT_EQ(list->get(1), 2);
    EXPECT_EQ(list->get(2), 5);
    EXPECT_EQ(list->get(3), 8);
    EXPECT_EQ(list->get(4), 9);
}

TEST_F(ArrayListTest, QuickSortDescending) {
    list->add(5);
    list->add(2);
    list->add(8);
    list->add(1);
    list->add(9);
    list->quickSort(descending);
    EXPECT_EQ(list->get(0), 9);
    EXPECT_EQ(list->get(1), 8);
    EXPECT_EQ(list->get(2), 5);
    EXPECT_EQ(list->get(3), 2);
    EXPECT_EQ(list->get(4), 1);
}

TEST_F(ArrayListTest, MergeSortTest) {
    list->add(5);
    list->add(2);
    list->add(8);
    list->add(1);
    list->add(9);
    list->mergeSort(*list, 0, list->size() - 1);
    EXPECT_EQ(list->get(0), 1);
    EXPECT_EQ(list->get(1), 2);
    EXPECT_EQ(list->get(2), 5);
    EXPECT_EQ(list->get(3), 8);
    EXPECT_EQ(list->get(4), 9);
}

TEST_F(ArrayListTest, SortEmptyList) {
    list->bubbleSort([](int a, int b) { return a < b; });
    EXPECT_EQ(list->size(), 0);

    list->quickSort([](int a, int b) { return a < b; });
    EXPECT_EQ(list->size(), 0);

    list->mergeSort(*list, 0, list->size() - 1);
    EXPECT_EQ(list->size(), 0);
}

TEST_F(ArrayListTest, SortSingleElement) {
    list->add(1);
    
    list->bubbleSort([](int a, int b) { return a < b; });
    EXPECT_EQ(list->size(), 1);
    EXPECT_EQ(list->get(0), 1);

    list->quickSort([](int a, int b) { return a < b; });
    EXPECT_EQ(list->size(), 1);
    EXPECT_EQ(list->get(0), 1);

    list->mergeSort(*list, 0, list->size() - 1);
    EXPECT_EQ(list->size(), 1);
    EXPECT_EQ(list->get(0), 1);
}

TEST_F(ArrayListTest, SortAlreadySorted) {
    for (int i = 1; i <= 5; i++) {
        list->add(i);
    }

    list->bubbleSort([](int a, int b) { return a < b; });
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(list->get(i), i + 1);
    }

    list->quickSort([](int a, int b) { return a < b; });
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(list->get(i), i + 1);
    }

    list->mergeSort(*list, 0, list->size() - 1);
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(list->get(i), i + 1);
    }
}

TEST_F(ArrayListTest, SortReverseSorted) {
    for (int i = 5; i >= 1; i--) {
        list->add(i);
    }

    list->bubbleSort([](int a, int b) { return a < b; });
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(list->get(i), i + 1);
    }

    list->clear();
    for (int i = 5; i >= 1; i--) {
        list->add(i);
    }

    list->quickSort([](int a, int b) { return a < b; });
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(list->get(i), i + 1);
    }

    list->clear();
    for (int i = 5; i >= 1; i--) {
        list->add(i);
    }

    list->mergeSort(*list, 0, list->size() - 1);
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(list->get(i), i + 1);
    }
}

// Test sorting with custom object
class Person {
public:
    std::string name;
    int age;
    Person() : name(""), age(0) {}
    Person(std::string n, int a) : name(n), age(a) {}

    bool operator<(const Person& other) const { return age < other.age; }
    bool operator>(const Person& other) const { return age > other.age; }
    bool operator==(const Person& other) const { return age == other.age && name == other.name; }
    bool operator<=(const Person& other) const { return age <= other.age; }
    bool operator>=(const Person& other) const { return age >= other.age; }
};

bool comparePerson(Person a, Person b) {
    return a < b;
}

TEST_F(ArrayListTest, SortCustomObject) {
    ArrayList<Person>* personList = new ArrayList<Person>();
    personList->add(Person("Alice", 30));
    personList->add(Person("Bob", 25));
    personList->add(Person("Charlie", 35));

    // Test bubbleSort
    personList->bubbleSort(comparePerson);
    EXPECT_EQ(personList->get(0).name, "Bob");
    EXPECT_EQ(personList->get(1).name, "Alice");
    EXPECT_EQ(personList->get(2).name, "Charlie");

    // Test quickSort
    personList->clear();
    personList->add(Person("Alice", 30));
    personList->add(Person("Bob", 25));
    personList->add(Person("Charlie", 35));

    personList->quickSort(comparePerson);
    EXPECT_EQ(personList->get(0).name, "Bob");
    EXPECT_EQ(personList->get(1).name, "Alice");
    EXPECT_EQ(personList->get(2).name, "Charlie");

    // Test mergeSort
    personList->clear();
    personList->add(Person("Alice", 30));
    personList->add(Person("Bob", 25));
    personList->add(Person("Charlie", 35));

    personList->mergeSort(*personList, 0, personList->size() - 1);
    EXPECT_EQ(personList->get(0).name, "Bob");
    EXPECT_EQ(personList->get(1).name, "Alice");
    EXPECT_EQ(personList->get(2).name, "Charlie");

    delete personList;
}
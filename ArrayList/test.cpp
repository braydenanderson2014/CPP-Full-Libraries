#include "ArrayList.h"

int main() {
    // Example usage of ArrayList
    ArrayList<int> list = ArrayList<int>(ArrayList<int>::DYNAMIC, 8);
    list.add(1);
    list.add(2);
    list.add(3);
    list.sort([](int a, int b) { return a < b; });

    return 0;
}
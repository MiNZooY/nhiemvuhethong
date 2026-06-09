#include "../lib/LinkedList.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "Starting tests for Procedural LinkedList..." << std::endl;

    // Head pointer
    Node<int>* head = nullptr;

    // Test 1: Empty and size
    assert(empty(head));
    assert(size(head) == 0);

    // Test 2: insertFront
    insertFront(head, 2);
    insertFront(head, 1);
    assert(!empty(head));
    assert(size(head) == 2);
    assert(head->data == 1);
    assert(head->next->data == 2);

    // Test 3: insertBack
    insertBack(head, 3);
    assert(size(head) == 3);
    assert(find(head, 3));

    // Test 4: insertAt
    insertAt(head, 1, 99); // list: 1 -> 99 -> 2 -> 3
    assert(size(head) == 4);
    assert(find(head, 99));

    // Test 5: remove
    remove(head, 99); // list: 1 -> 2 -> 3
    assert(size(head) == 3);
    assert(!find(head, 99));

    // Test 6: removeAt
    removeAt(head, 1); // removes 2. list: 1 -> 3
    assert(size(head) == 2);
    assert(!find(head, 2));

    // Test 7: print
    std::cout << "Current list (should be 1 3): ";
    print(head);

    // Test 8: clear
    clear(head);
    assert(empty(head));
    assert(size(head) == 0);

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}

#include "../list.h"

int main() {
    List<int> list;

    list.add(1);
    list.add(2);
    list.add(3);
    list.add(4);
    list.add(5);

    std::cout << "Before delete \n";
    list.printState();

    list.remove(2);

    std::cout << "After delete \n";
    list.printState();
    return 0;
}
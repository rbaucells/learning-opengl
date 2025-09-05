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

    list.removeAt(3);

    std::cout << "After delete \n";
    list.printState();

    List<int> secondList;

    secondList.add(1);
    secondList.add(2);
    secondList.add(3);
    secondList.add(4);
    secondList.add(5);

    std::cout << "Before add \n";
    secondList.printState();

    // [1, 2, 3, 4, 5] -> [1, 5, 2, 3, 4, 5]

    secondList.addAt(5, 1);

    std::cout << "After add \n";
    secondList.printState();

    List thirdList = {1, 2, 3};
    thirdList.printState();

    return 0;
}
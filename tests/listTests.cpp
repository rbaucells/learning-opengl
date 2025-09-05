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

    list.remove(3);

    std::cout << "After delete \n";
    list.printState();

    List<int> secondList;

    secondList.add(1);
    secondList.add(2);
    secondList.add(3);
    secondList.add(4);
    secondList.add(5);

    std::cout << "Before delete \n";
    secondList.printState();

    secondList.removeNoAlloc(5);
    secondList.shrinkToFit();

    std::cout << "After delete \n";
    secondList.printState();
    return 0;
}
#include <iostream>
#include "sort_heaviest.h"

int main() {
    int comparisons = 0;

    std::vector<int> v1{1,2,3,4,5,6,7,8,9,10};
    sort_heaviest(v1, comparisons);
    std::cout << "# of Comparisons for v1: " << comparisons << std::endl;

    comparisons = 0;
    std::vector<int> v{10,9,8,7,6,5,4,3,2,1};
    sort_heaviest(v, comparisons);
    std::cout << "# of Comparisons for v: " << comparisons << std::endl;

}
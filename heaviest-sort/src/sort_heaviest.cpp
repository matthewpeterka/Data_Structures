// sorting items from the heaviest to lightest
// by selecting the heaviest item at each time  

#include <iostream>
#include <vector>
#include "sort_heaviest.h"

using namespace std;

void sort_heaviest(vector<int>& v, int& comparisons){
    // The index of the heaviest element
    size_t heaviest;
    size_t ind;

    //loop to go through vector 
    for (size_t i = 0; i < v.size(); i++) {
        //sets heaviest to the latest element
        heaviest = v.at(i);
        ind = i;

        //loops through the rest of the vector after the largest element just inserted
        for (size_t n = i + 1; n < v.size(); n++) {

            //if the value at n is larger then it will set the max and the index
            if (v.at(n) > heaviest) {
                ind = n;
                heaviest = v.at(n);
                
                
                
            }
            //adds to the comparisons
            comparisons++;
        }

        //swap the items
        int temp = v.at(i);
        v.at(i) = v.at(ind);
        v.at(ind) = temp;
        

        /*
        cout << max << endl;
        int temp = v.at(z);
        v.at(z) = v.at(max_ind);
        v.at(max_ind) = temp;
        
        for (int i = 0; i < v.size(); i++) {
            cout << v.at(i) << " ";
        }
        cout << endl;
        */
        
    }
    


}
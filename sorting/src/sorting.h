#pragma once

#include <functional> // std::less
#include <iterator> // std::iterator_traits

namespace sort {

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the 
	// iterator's value type will be float. std::less 
	// will select the < for sorting floats as the 
	// default comparator.
	template<typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template<typename T>
	void swap(T & a, T & b) noexcept {

		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);

	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;


		//checks if the list is empty
		if (begin != end) {
			//difference type to get the length and goes up until the last element
			for (difference_type i = 0; i < end - begin - 1; i++) {
				//boolean to see if it will swap, if not will terminate the loop to save time
				bool swapped = false;
				//loops through the unsorted portion of the vector
				for (difference_type z = 0; z < end - begin - 1 - i; z++) {
					//if the item is bigger than the one on the right then it will swap and set boolean to true
					if (comp(*(begin + z + 1), *(begin + z))) {
						swap(*(begin + z + 1), *(begin + z));
						swapped = true;
					}
				}
				//this checks if the vector is already sorted and will break if there is no swap
				if (!swapped) {
					break;
				}
			}
		}

	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		//if the list isn't empty
		if (begin != end) {
			//loops through the entire array
			for (RandomIter i = begin + 1; i != end; i++) {
				//iterator one behind the outer loop
				for (RandomIter r = i; r > begin; r--) {
					if (comp(*r, *(r - 1))) {
						swap(*r, *(r - 1));
					}
					else {
						break;
					}
				}
				/*
				//loops through and swaps as long as the temp variaible is less than the item in the vector
				while (r >= begin && comp(temp, *r)) {
					swap(*(r + 1), *r);
					r--;
				}
				//finally sets the new index value
				*/
			}
		}

	}

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		//if the list isn't empty
		if (begin != end) {
			//loops through the entire vector
			for (RandomIter i = begin; i != end; i++) {
				//sets the lowest to the first index that is not sorted
				//new iterator the the index in front
				RandomIter r = i + 1;
				RandomIter min_index = i;
				//loops to the end of the vector
				while (r != end) {
					//if the item is smaller than the current low it will set the new low index
					if(comp(*r, *min_index)) {
						min_index = r;
					}
					r++;
				}
				//if the low is already sorted then it will not enter the loop, if it isn't then it will swap the items
				if (min_index != i) {
					swap(*i, *min_index);
				}
			}
		}


	}
}




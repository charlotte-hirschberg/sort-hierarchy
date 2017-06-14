// Charlotte Hirschberger
// C++ II, Wk. 8, 3/12/17
// This program counts the number of comparisons and swaps made by a given sort algorithm
// when processing an array of strings or integers. The sort is implemented
// as an AbstractSort, so that derived classes can define the sort algorithm of 
// choice while reusing AbstractSort's members

#include "stdafx.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

/*****AbstractSort class*****/
// Defines attributes and functions for common use by multiple
// derived classes. sort() is a pure virtual function, so the client
// cannot instantiate AbstractSort. The specifics of sort() must be 
// implemented by the deriving classes.
template<typename T>
class AbstractSort
{
	// members not accessible from client
protected:
	int numComp;						// number of comparisons made by algorithm
	int numSwap;						// number of swaps made by algorithm
	int size;							// array size
	unique_ptr<T[]> uPtArr = nullptr;	// pointer to array of type T

	/*Constructor that initializes attributes. Can only be invoked via
	derived classes.*/
	AbstractSort(unique_ptr<T[]> uPtArr, int size)
	{
		this->size = size;
		this->uPtArr = move(uPtArr);
		numComp = 0;
		numSwap = 0;
	}

	// Return true if object a < b
	bool compare(int a, int b)
	{
		numComp++;
		return a < b;
	}

	// Ignore case and return true if a < b
	// Example: apple < cat
	bool compare(string a, string b)
	{
		transform(a.begin(), a.end(), a.begin(), ::toupper);
		transform(b.begin(), b.end(), b.begin(), ::toupper);
		numComp++;
		return a < b;
	}

	// Custom swap implementation increments numSwap
	void makeSwap(int idx1, int idx2)
	{
		T temp = uPtArr[idx1];
		uPtArr[idx1] = uPtArr[idx2];
		uPtArr[idx2] = temp;
		numSwap++;
	}

	// accessible from client
public:
	int getNumComp()
	{
		return numComp;
	}

	int getNumSwaps()
	{
		return numSwap;
	}

	// pure virtual functions, implemented in derivations
	virtual void sort() = 0;
	virtual string getSortName() = 0;

	// Format & display the sorted values
	virtual void displayArr()
	{
		// Array is 1+ elements, so display first element
		cout << uPtArr[0];

		// Display elements 2 through n - 2 after a comma
		for (int i = 1; i < size - 1; i++)
		{
			cout << ", " << uPtArr[i];
		}

		// Display element n-1
		if (size > 1)
		{
			cout << " and " << uPtArr[size - 1];
		}
	}
};


// An implementation of AbstractSort sort()
// Uses selection sort to put values of type T in ascending order
template<typename T>
class SelectSort : public AbstractSort<T>
{
private:
	string sortName = "Selection Sort";
public:
	string getSortName() override
	{
		return sortName;
	}

	// Override the pure virtual function in AbstractSort
	virtual void sort() override
	{
		int startScan, minInd;
		T minVal;	// value at minInd

		for (startScan = 0; startScan < (size - 1); startScan++)
		{
			// initially assume that the first element in the subarray is the lowest
			minInd = startScan;
			minVal = uPtArr[startScan];

			// compare current minimum to each additional element
			for (int i = startScan + 1; i < size; i++)
			{
				if (compare(uPtArr[i], minVal))
				{
					minVal = uPtArr[i];
					minInd = i;
				}
			}
			// complete the swap
			makeSwap(minInd, startScan);
			/*uPtArr[minInd] = uPtArr[startScan];
			uPtArr[startScan] = minVal;*/
		}
	}

	// Pass this constructor's parameters to the underlying class
	SelectSort(unique_ptr<T[]> uPtArr, int size) : AbstractSort(move(uPtArr), size)
	{

	}
};

/*Derivation of AbstractSort
Implements sort as QuickSort with Lamuto's partitioning*/
template<typename T>
class QuickSort : public AbstractSort<T>
{
protected:
	string sortName = "Standard QuickSort";
private:
	void sort(int start, int end)
	{
		// as long as subarray has 1 or more elements
		if (start < end) // true; true
		{
			int pivot = partition(start, end);

			sort(start, pivot - 1);	// sort subarray to left of pivot
			sort(pivot + 1, end);	// sort subarray to right
		}
	}
	virtual int partition(int start, int end)
	{
		// Initially set the pivot index and value to that of leftmost element
		int pivotIdx = start;
		T pivotVal = uPtArr[start];

		// Compare pivot to every value to its right
		for (int i = start + 1; i <= end; i++)
		{
			// if a lower value is discovered...
			if (compare(uPtArr[i], pivotVal))
			{
				// swap curr item with the value to pivot's right
				makeSwap(pivotIdx + 1, i);

				// swap curr item with pivot
				makeSwap(pivotIdx, pivotIdx + 1);
				pivotIdx++;
			}
		}
		return pivotIdx;
	}
public:
	virtual void sort() override
	{
		sort(0, size - 1);
	}

	virtual string getSortName() override
	{
		return sortName;
	}

	QuickSort(unique_ptr<T[]> uPtArr, int size) : AbstractSort(move(uPtArr), size)
	{
	}
};

/*Derivation of QuickSort
Implements sort() as QuickSort with Hoare's partition
and median-of-three pivot value*/
template<typename T>
class M3QuickSort : public QuickSort<T>
{
private:
	virtual int partition(int start, int end) override
	{
		// Examine & sort the leftmost, central, rightmost elements
		int medIdx = getMedian(start, end);

		// Put the subarray's central-most element at array end
		makeSwap(medIdx, end);

		// Use the new end value as the pivot
		T pivotVal = uPtArr[end];

		int i = start - 1;
		int j = end;
		
		while (true) // i < j
		{
			/*As long as arr[i] is less than pivotVal, move i right*/
			do
			{
				i++;
			} while (compare(uPtArr[i], pivotVal));

			/*As long as arr[j] is greater than pivotVal, move j left*/
			do
			{
				j--;
			} while (compare(pivotVal, uPtArr[j]));

			if (i >= j)
			{
				makeSwap(i, end);
				return i;
			}
			makeSwap(i, j);	// swap value that is greater than pivot with value that is less than it
		}
	}

	int getMedian(int start, int end)
	{
		int mid = start + ((end - start) / 2);

		// rightmost is less than leftmost
		if (compare(uPtArr[end], uPtArr[start]))
			makeSwap(end, start);
		// mid is less than leftmost
		if (compare(uPtArr[mid], uPtArr[start]))
			makeSwap(mid, start);
		// right is less than mid
		if (compare(uPtArr[end], uPtArr[mid]))
			makeSwap(end, mid);
		return mid;
	}

public:
	M3QuickSort(unique_ptr<T[]> uPtArr, int size) : QuickSort(move(uPtArr), size)
	{
		sortName = "Hoare's QuickSort with Median-of-Three";
	}
};

int main()
{
	char repeat;				// sort another set? Y/N
	const int arrSize = 50;		// array size

	srand(time(NULL));	// seed

	cout << "This program counts comparisons and swaps to indicate the efficiency of sorting algorithms.\n\n";
	do
	{	
		// Create 3 identical arrays to pass to each of 3 sort implementations
		unique_ptr<signed int[]> ptIntsSS = make_unique<signed int[]>(arrSize);
		unique_ptr<signed int[]> ptIntsQS = make_unique<signed int[]>(arrSize);
		unique_ptr<signed int[]> ptIntsM3 = make_unique<signed int[]>(arrSize);

		cout << "The unsorted set of random integers: ";
		int randInt;

		// Fill each array with the same random integers
		for (int i = 0; i < arrSize; i++)
		{
			randInt = rand() % 100 + 1;
			ptIntsSS[i] = ptIntsQS[i] = ptIntsM3[i] = randInt;
		}
		
		// vector of shared pointers to objects with base class AbstractSort
		// allows program to leverage polymorphism
		vector<shared_ptr<AbstractSort<signed int>>> sortObjects
		{
			make_shared<SelectSort<signed int>>
			(move(ptIntsSS), arrSize),
			make_shared<QuickSort<signed int>>
			(move(ptIntsQS), arrSize),
			make_shared<M3QuickSort<signed int>>
			(move(ptIntsM3), arrSize)
		};
		sortObjects[0]->displayArr(); // display unsorted set

		for (auto &sObj : sortObjects)
		{
			sObj->sort();
		}
		cout << "\n\nThe sorted set: ";
		sortObjects[0]->displayArr(); // display sorted set once

		for (auto &sObj : sortObjects)
		{
			cout << endl << endl << sObj->getSortName() << " made " << sObj->getNumComp() << " comparisons and "
				<< sObj->getNumSwaps() << " swaps.";
		}

		cout << "\n\nWould you like to sort another set? Y/N ";
		cin >> repeat;

		system("cls");
	} while (toupper(repeat) == 'Y');

	return 0;
}
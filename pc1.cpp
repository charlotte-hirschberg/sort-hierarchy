// Charlotte Hirschberger
// C++ II, Wk. 8, 3/8/17
// This program counts the number of comparisons made by a given sort algorithm
// when processing an array of strings or integers. The sort is implemented
// as an AbstractSort, so that derived classes can define the sort algorithm of 
// choice while reusing AbstractSort's members

#include "stdafx.h"
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// Prototypes
void displayMenu();
int getChoice();
int validateSize();

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
	int size;							// array size
	unique_ptr<T[]> uPtArr = nullptr;	// pointer to array of type T
	
	/*Constructor that initializes attributes. Can only be invoked via
	derived classes.*/
	AbstractSort(unique_ptr<T[]> uPtArr, int size)
	{
		this->size = size;
		this->uPtArr = move(uPtArr);
		numComp = 0;
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

	// pure virtual function, implemented in derivations
	virtual void sort() = 0;

	// accessible from client
public:
	int getNumComp()
	{
		return numComp;
	}

	// Format & display the sorted values
	virtual void displayArr()
	{
		cout << "The sorted set: ";

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
		string getSortName()
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
					uPtArr[minInd] = uPtArr[startScan];
					uPtArr[startScan] = minVal;
				}
		}

		// Pass this constructor's parameters to the underlying class
		SelectSort(unique_ptr<T[]> uPtArr, int size) : AbstractSort(move(uPtArr), size)
		{

		}
};

int main()
{	
	char repeat;	// sort another set? Y/N
	int mChoice;	// menu choice
	int size;		// selected array size

	cout << "This program counts comparisons to indicate the efficiency of sorting algorithms."
		<< "\nMake a choice below.\n";
	do
	{
		displayMenu();
		mChoice = getChoice();

		if (mChoice != 3)	// not Exit
		{
			cout << "How many elements do you want to enter? ";
			size = validateSize();
			cin.get();	// eat /n in buffer

			if (mChoice == 1)	// Sort integers
			{
				// allocate memory that will be automatically deletd
				unique_ptr<signed int[]> uPtArr(new signed int[size]);

				// Fill the array with integers
				for (int k = 0; k < size; k++)
				{
					cout << "Enter Value " << k + 1 << ": ";
					cin >> uPtArr[k];
				}

				// Direct SelectSort's unique_ptr parameter at uPtArr's memory bank
				SelectSort<signed int> s1(move(uPtArr), size);
				s1.sort();
				s1.displayArr();
				cout << endl << s1.getSortName() << " made " << s1.getNumComp() << " comparisons.";
			}

			else if (mChoice == 2)		// Sort strings
			{
				// allocate memory that will be automatically deleted
				unique_ptr<string[]> uPtArr(new string[size]);
				
					// Fill the array
					for (int k = 0; k < size; k++)
					{
						cout << "Enter String " << k + 1 << ": ";
						getline(cin, uPtArr[k]);
					}
					SelectSort<string> s1(move(uPtArr), size);
					s1.sort();
					s1.displayArr();
					cout << endl << s1.getSortName() << " made " << s1.getNumComp() << " comparisons.";
			}
		cout << "\nWould you like to sort another set? Y/N ";
		cin >> repeat;
		}



		system("cls");
	} while (mChoice != 3 && toupper(repeat) == 'Y');

    return 0;
}

/*****displayMenu*****/
// Show menu choices and prompt
void displayMenu()
{
	cout << "1. Sort integers"
		<< "\n2. Sort strings"
		<< "\n3. Exit"
		<< "\nYour choice: ";
}

/*****getChoice*****/
// Verify that user has made a valid menu choice
int getChoice()
{
	int choice;
	cin >> choice;
	while (choice < 1 || choice > 3)
	{
		cout << "That is not a valid choice. Please re-enter: ";
		cin >> choice;
	}

	return choice;
}

/*****validateSize*****/
/* Verify that size entered for array is at least 1*/
int validateSize()
{
	int size;
	cin >> size;
	while (size < 1)
	{
		cout << "That is not a valid size. Please re-enter: ";
		cin >> size;
	}
	return size;
}


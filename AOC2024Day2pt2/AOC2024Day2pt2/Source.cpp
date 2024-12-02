#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<int>& list, string nums);
bool safeOrUnsafe(const vector<int>& numList);
bool safeAlgorithm(const vector<int>& numList);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;
	int totSafe = 0;
	while (getline(fileRead, input))
	{
		// Make a vector for the numbers in the row
		vector<int> numberList;

		//Get the numbers
		readNumbers(numberList, input);

		//Reads out the number list for each row
		/*for (int i = 0; i < numberList.size(); i++)
		{
			cout << numberList[i] << " ";
		}
		cout << endl;*/

		bool isSafe = safeOrUnsafe(numberList);

		if (isSafe)
		{
			totSafe++;
		}

	}

	cout << "Total Safe: " << totSafe << endl;

	return 0;
}

void readNumbers(vector<int>& list, string nums)
{
	string numString = "";
	for (int i = 0; i < nums.size(); i++)
	{
		char c = nums[i];
		if (48 <= c && c <= 57)
		{
			numString += nums[i];
		}
		else if (numString != "" && c == ' ')
		{
			list.push_back(stoi(numString));
			numString = "";
		}
	}

	if (numString != "")
	{
		list.push_back(stoi(numString));
	}
}

bool safeOrUnsafe(const vector<int>& numList)
{
	bool safe = safeAlgorithm(numList);

	if (safe)
	{
		return safe;
	}
	else 
	{
		for (int i = 0; i < numList.size(); i++)
		{
			vector<int> copyList = numList;
			copyList.erase(copyList.begin() + i);

			bool newListSafe = safeAlgorithm(copyList);

			if (newListSafe)
			{
				return newListSafe;
			}
		}

		return false;
	}
}

bool safeAlgorithm(const vector<int>& numList)
{
	bool increase = true;
	bool decrease = true;
	int maxDiff = INT_MIN;
	int minDiff = INT_MAX;
	//Algorithm to see if the levels are safe
	for (int i = 0; i < numList.size() - 1; i++)
	{
		int diff = numList[i + 1] - numList[i];

		if (diff > 0)
		{
			decrease = false;
		}

		if (diff < 0)
		{
			increase = false;
		}

		if (abs(diff) > maxDiff)
		{
			maxDiff = abs(diff);
		}

		if (abs(diff) < minDiff)
		{
			minDiff = abs(diff);
		}
	}

	//Checks if safe or not
	if ((increase || decrease) && minDiff >= 1 && maxDiff <= 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}
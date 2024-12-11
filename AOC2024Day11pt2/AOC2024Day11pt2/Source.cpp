#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<long long>& list, string nums);
long long simulateBlinks(long long stone, int numBlinks);

//Global variable
//Entires in format { {stone1 #, numBlinksLeft, #resultingStones}, {stone2 #, ..., ...}, ... {} }
vector<vector<long long>> stoneDict; 
int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Get the input
	string input;
	getline(fileRead, input);

	//Get the stone list
	vector<long long>stoneList;
	readNumbers(stoneList, input);

	//Simulate each stone seperately
	long long totalStones = 0;
	int numBlinks = 75;
	for (int i = 0; i < stoneList.size(); i++)
	{
		totalStones += simulateBlinks(stoneList[i], numBlinks);
		//cout << stoneList[i] << " ";
	}

	for (long long i = 0; i < stoneDict.size(); i++)
	{
		cout << stoneDict[i][0] << " " << stoneDict[i][1] << " " << stoneDict[i][2] << endl;
	}

	cout << "Total stones = " << totalStones << endl;

	return 0;
}

long long simulateBlinks(long long stone, int numBlinks)
{
	//Check it isn't already in the dictionary
	for (long long i = 0; i < stoneDict.size(); i++)
	{
		if (stone == stoneDict[i][0] && numBlinks == stoneDict[i][1])
		{
			return stoneDict[i][2];
		}
	}

	//If there are no more blinks return a 1
	if (numBlinks == 0)
	{
		return 1;
	}

	long long totalStones = 0;
	string stringCurStone = to_string(stone);

	if (stone == 0)
	{
		totalStones += simulateBlinks(1, numBlinks - 1);
	}
	else if (stringCurStone.size() % 2 == 0)
	{
		string firstHalf = stringCurStone.substr(0, stringCurStone.size() / 2);
		string secondHalf = stringCurStone.substr(stringCurStone.size() / 2);

		string noLeadZero = "";
		bool foundNonZero = false;
		for (int k = 0; k < secondHalf.size(); k++)
		{
			if (secondHalf[k] != '0')
			{
				foundNonZero = true;
				noLeadZero += secondHalf[k];
			}
			else if (secondHalf[k] == '0' && foundNonZero)
			{
				noLeadZero += '0';
			}
		}

		if (noLeadZero == "")
		{
			secondHalf = "0";
		}
		else
		{
			secondHalf = noLeadZero;
		}

		//calc the first half
		totalStones += simulateBlinks(stoll(firstHalf), numBlinks-1);

		//calc the second half
		totalStones += simulateBlinks(stoll(secondHalf), numBlinks - 1);
	}
	else
	{
		totalStones += simulateBlinks(stone * 2024, numBlinks - 1);
	}
	
	stoneDict.push_back({ stone, numBlinks, totalStones });
	return totalStones;
}

void readNumbers(vector<long long>& list, string nums)
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
			list.push_back(stoll(numString));
			numString = "";
		}
	}

	if (numString != "")
	{
		list.push_back(stoll(numString));
	}
}

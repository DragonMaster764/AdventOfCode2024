#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<long long>& list, string nums);
long long simulateBlinks(int firstStone, int numBlinks);

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
	int numBlinks = 25;
	for (int i = 0; i < stoneList.size(); i++)
	{
		totalStones += simulateBlinks(stoneList[i], numBlinks);
		//cout << stoneList[i] << " ";
	}

	cout << "Total stones = " << totalStones << endl;

	return 0;
}

long long simulateBlinks(int firstStone, int numBlinks)
{
	vector<long long> simulationVector = { firstStone };

	//simulates however many blinks listed
	for (int i = 0; i < numBlinks; i++)
	{
		//Checks each stone
		for (long long j = 0; j < simulationVector.size(); j++)
		{
			long long curStone = simulationVector[j];
			string stringCurStone = to_string(curStone);

			if (curStone == 0)
			{
				simulationVector[j] = 1;
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

				//change first half to it's spot in the stonelist
				simulationVector[j] = stoll(firstHalf);

				//Insert second half right after
				simulationVector.insert(simulationVector.begin() + j + 1, stoll(secondHalf));

				//Move j an extra one to avoid doing work on the second half number
				j++;
			}
			else
			{
				simulationVector[j] = simulationVector[j] * 2024;
			}
		}
	}

	return simulationVector.size();
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

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<int>& list, string nums);
long long playClawMachine(const vector<int>& buttonA, const vector<int>& buttonB, const vector<long long>& target);
long long minVectorElement(const vector<long long>& v);
long long mostPossiblePlays(const vector<int>& buttonA, const vector<int>& buttonB, const vector<long long>& target);
void allPlayCombos(const vector<int>& buttonA, const vector<int>& buttonB,
	const vector<long long>& target, long long maxPlays, vector<vector<long long>>& workingButtonPressCombosX);
long long maxVectorElement(const vector<long long>& v);
void readTargetNumbers(vector<long long>& list, string nums);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;
	int totalTokenCost = 0;

	int clawNumber = 1; //Testing
	
	while (getline(fileRead, input))
	{
		//While spaces, get the next line
		while (!input.size())
		{
			getline(fileRead, input);
		}

		//Vector Initalization
		vector<int> buttonA;
		vector<int> buttonB;
		vector<long long> target;

		readNumbers(buttonA, input);
		getline(fileRead, input); //Get next line for button B
		readNumbers(buttonB, input);
		getline(fileRead, input); //Get next line for the target destination
		readTargetNumbers(target, input);
		
		totalTokenCost += playClawMachine(buttonA, buttonB, target);

		cout << "Claw # " << clawNumber << " done" << endl;
		clawNumber++;

	}

	cout << "Total token cost: " << totalTokenCost << endl;

	return 0;
}

long long playClawMachine(const vector<int>& buttonA, const vector<int>& buttonB, const vector<long long>& target)
{
	//Some variable intialization
	int totalTokenCost = 0;
	long long maxPlays = mostPossiblePlays(buttonA, buttonB, target);
	vector<vector<long long>> workingButtonPressCombosX;
	vector<vector<long long>> workingButtonPressCombos;

	//Go through possible combos of plays 
	allPlayCombos(buttonA, buttonB, target, maxPlays, workingButtonPressCombosX);

	//Go through each working button combo for xDirection. If it works for y as well
	//save it into the overall workingButtonPressCombo variable
	int yValueA = buttonA[1];
	int yValueB = buttonB[1];
	for (long i = 0; i < workingButtonPressCombosX.size(); i++)
	{
		long long finalYPos = (workingButtonPressCombosX[i][0] * yValueA) + (workingButtonPressCombosX[i][1] * yValueB);
		if (finalYPos == target[1])
		{
			workingButtonPressCombos.push_back({ workingButtonPressCombosX[i][0], workingButtonPressCombosX[i][1] });
		}
	}

	//Calc the lowest amount of tokens to win
	if (workingButtonPressCombos.size() != 0)
	{
		vector<long long> totTokensSpent;
		int buttonACost = 3;
		int buttonBCost = 1;

		for (long i = 0; i < workingButtonPressCombos.size(); i++)
		{
			totTokensSpent.push_back((buttonACost * workingButtonPressCombos[i][0])
				+ (buttonBCost * workingButtonPressCombos[i][1]));
		}

		return minVectorElement(totTokensSpent);
	}
	else
	{
		//If there are no working combos, return 0
		return 0;
	}
}

long long minVectorElement(const vector<long long>& v)
{
	long long minResult = LLONG_MAX;
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] < minResult)
		{
			minResult = v[i];
		}
	}

	return minResult;
}

long long maxVectorElement(const vector<long long>& v)
{
	long long maxResult = LLONG_MIN;
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] > maxResult)
		{
			maxResult = v[i];
		}
	}

	return maxResult;
}

long long mostPossiblePlays(const vector<int>& buttonA, const vector<int>& buttonB, const vector<long long>& target)
{
	vector<long long> divisionResults;

	//Go through x posibilities
	int xValue = buttonA[0];
	for (int i = 0; i < 2; i++)
	{
		divisionResults.push_back(target[0] / xValue);
		xValue = buttonB[0];
	}

	//Go through y posibilities
	int yValue = buttonA[1];
	for (int i = 0; i < 2; i++)
	{
		divisionResults.push_back(target[1] / yValue);
		xValue = buttonB[1];
	}

	//Find the most plays that are possible before you pass the prize
	//At the most you can play 100 times (part a at least)
	long long maxDivisionResults = maxVectorElement(divisionResults);

	return min((long long)100, maxDivisionResults);
}

//Test every combo of presses and saves ones that (possibly) work
void allPlayCombos(const vector<int>& buttonA, const vector<int>& buttonB,
	const vector<long long>& target, long long maxPlays, vector<vector<long long>>& workingButtonPressCombosX)
{
	int xValueA = buttonA[0];
	int xValueB = buttonB[0];
	int targetX = target[0];

	for (int i = 0; i <= maxPlays; i++)
	{
		for (int j = 0; j <= maxPlays; j++)
		{
			int buttonAPresses = i;
			int buttonBPresses = j;			

			int finalPos = (buttonAPresses * xValueA) + (buttonBPresses * xValueB);

			if (finalPos == targetX)
			{
				workingButtonPressCombosX.push_back({ buttonAPresses, buttonBPresses });
			}
		}
	}
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

void readTargetNumbers(vector<long long>& list, string nums)
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

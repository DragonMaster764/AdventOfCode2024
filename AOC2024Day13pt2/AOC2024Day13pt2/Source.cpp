#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<int>& list, string nums);
long long playClawMachine(const vector<int>& buttonA, const vector<int>& buttonB, const vector<long long>& target);
vector<long long> solveMatrix(const vector<vector<long long>>& matrix);
void readTargetNumbers(vector<long long>& list, string nums);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;
	long long totalTokenCost = 0;

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
	vector<vector<long long>> matrix = { {(long long)buttonA[0], (long long)buttonB[0], target[0]},
		{(long long)buttonA[1], (long long)buttonB[1], target[1]} };

	vector<long long> pressSolution = solveMatrix(matrix);

	//Calc the lowest amount of tokens to win
	if (pressSolution[0] != -1 && pressSolution[1] != -1)
	{
		int buttonACost = 3;
		int buttonBCost = 1;

		return (buttonACost * pressSolution[0]) + (buttonBCost * pressSolution[1]);
	}
	else
	{
		//If there are no working combos, return 0
		return 0;
	}
}

vector<long long> solveMatrix(const vector<vector<long long>>& matrix)
{
	vector<vector<long long>> matrixCopy = matrix;
	
	//Multiply by x's then subtract
	//e.g. 4x + 6y = 100
	//     2x + 8y = 20
	//    =0x - 20y = 120
	for (int i = 0; i < matrixCopy[0].size(); i++)
	{
		matrixCopy[0][i] *= matrix[1][0];
	}
	for (int i = 0; i < matrixCopy[1].size(); i++)
	{
		matrixCopy[1][i] *= matrix[0][0];
	}

	vector<long long> row;
	for (int i = 0; i < matrixCopy[0].size(); i++)
	{
		row.push_back(matrixCopy[0][i] - matrixCopy[1][i]);
	}

	if (row[2] % row[1] == 0)
	{
		long long bPresses = row[2] / row[1];

		if ((matrix[0][2] - matrix[0][1] * bPresses) % matrix[0][0] == 0)
		{
			long long aPresses = (matrix[0][2] - matrix[0][1] * bPresses) / matrix[0][0];

			return { aPresses, bPresses };
		}
		else
		{
			return { -1, -1 };
		}
	}
	else
	{
		return { -1, -1 };
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
			list.push_back(stoll(numString) + 10000000000000);
			numString = "";
		}
	}

	if (numString != "")
	{
		list.push_back(stoll(numString) + 10000000000000);
	}
}

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<int>& list, string nums);
int manualInOrder(const vector<int>& manual, const vector<vector<int>>& rules);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	//Rules Vector
	vector<vector<int>> rules;

	// Parse the data
	string input;
	bool isSpace = false; //To check for the second section
	while (!isSpace)
	{
		getline(fileRead, input);

		if (!input.size())  //skip blanks and signal we are on next section
		{
			isSpace = true;
		}
		else 
		{
			vector<int> rule;
			rule = { stoi(input.substr(0, input.find_first_of('|'))), stoi(input.substr(input.find_first_of('|') + 1)) };
			rules.push_back(rule);

			/*cout << rule[0] << " " << rule[1] << endl;*/
		}

	}
	
	int total = 0;
	while (getline(fileRead, input))
	{
		if (input.size()) //protection against blanks
		{
			// Get a manual
			vector<int> manual;
			readNumbers(manual, input);

			//Check manual if it's in order then adds
			total += manualInOrder(manual, rules);

			// TESTS

			/*for (int i = 0; i < manual.size(); i++)
			{
				cout << manual[i] << " ";
			}
			cout << endl;*/

			/*cout << input << endl;*/
		}
		
	}
	
	cout << "Total: " << total << endl;

	return 0;
}

//Method tests the manual is in order
int manualInOrder(const vector<int>& manual, const vector<vector<int>>& rules)
{
	bool inOrder = true;

	//Check every rule
	for (int i = 0; i < rules.size(); i++) 
	{
		int firstNumIndex = -1;
		int secondNumIndex = -1;

		for (int j = 0; j < manual.size(); j++)
		{
			if (manual[j] == rules[i][0])
			{
				firstNumIndex = j;
			}
			else if (manual[j] == rules[i][1])
			{
				secondNumIndex = j;
			}
		}

		//Makes sure both numbers are in manual
		if (firstNumIndex != -1 && secondNumIndex != -1)
		{
			// If the secondNumIndex is less than the first, in order is false
			// then break the loop
			if (secondNumIndex < firstNumIndex)
			{
				inOrder = false;
				break;
			}
		}
	}

	if (inOrder)
	{
		return manual[manual.size() / 2];
	}
	else 
	{
		return 0;
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
		else if (numString != "" && c == ',')
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

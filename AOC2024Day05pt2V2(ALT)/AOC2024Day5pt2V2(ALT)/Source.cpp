#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> revisedManual;

// Methods
void readNumbers(vector<int>& list, string nums);
int manualInOrder(const vector<int>& manual, const vector<vector<int>>& rules, bool prevNotOrder);
int sortManual(const vector<int>& manual, const vector<vector<int>>& appRules);

class Node
{
private:
	int value;
	Node* left;
	Node* right;
public:
	Node()
	{
		value = -1;
		left = nullptr;
		right = nullptr;
	}
	Node(int data)
	{
		value = data;
		left = nullptr;
		right = nullptr;
	}
	void insert(int data, const vector<int>& manual, const vector<vector<int>>& rules)
	{
		if (value != -1)
		{
			bool isBefore = before(data, manual, rules);

			if (isBefore)
			{
				if (left == nullptr)
				{
					left = new Node(data);
				}
				else
				{
					left->insert(data, manual, rules);
				}
			}
			else
			{
				if (right == nullptr)
				{
					right = new Node(data);
				}
				else
				{
					right->insert(data, manual, rules);
				}
			}
		}
		else 
		{
			value = data;
		}
	}
	bool before(int data, const vector<int>& manual, const vector<vector<int>>& rules)
	{
		for (int i = 0; i < rules.size(); i++)
		{

			if ((value == rules[i][0] || value == rules[i][1]) && (data == rules[i][0] || data == rules[i][1]))
			{
				if (value == rules[i][1])
				{
					return true;
				}
				else
				{
					return false;
				}
			}

		}

		return false;
	}
	void printManual()
	{
		if (left != nullptr)
		{
			left->printManual();
		}
		revisedManual.push_back(value);
		if (right != nullptr)
		{
			right->printManual();
		}
	}
};

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
			total += manualInOrder(manual, rules, false);

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
int manualInOrder(const vector<int>& manual, const vector<vector<int>>& rules, bool prevNotOrder)
{
	bool inOrder = true;
	vector<vector<int>> appRules;

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
			}

			appRules.push_back(rules[i]);
		}
	}

	if (inOrder && !prevNotOrder)
	{
		return 0;
	}
	else if (inOrder && prevNotOrder)
	{
		return manual[manual.size() / 2];
	}
	else
	{
		return sortManual(manual, appRules);
	}
}

int sortManual(const vector<int>& manual, const vector<vector<int>>& appRules)
{
	
	revisedManual = {};

	Node * root = new Node(manual[0]);

	for (int i = 1; i < manual.size(); i++)
	{
		root->insert(manual[i], manual, appRules);
	}

	root->printManual();

	for (int i = 0; i < revisedManual.size(); i++)
	{
		cout << revisedManual[i] << " ";
	}
	cout << endl;

	return manualInOrder(revisedManual, appRules, true);
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

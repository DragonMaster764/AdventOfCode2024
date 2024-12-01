#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() 
{
	// Intialize some vectors to store our inputs
	vector<int> list1;
	vector<int> list2;

	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;
	while (getline(fileRead, input))
	{
		list1.push_back(stoi(input.substr(0, input.find_first_of(' '))));
		list2.push_back(stoi(input.substr(input.find_last_of(' '))));
	}

	// Sort the data in ascending order
	sort(list1.begin(), list1.end());
	sort(list2.begin(), list2.end());


	//Double check they are same size before doing the math
	if (list1.size() == list2.size())
	{
		int total = 0;

		for (int i = 0; i < list1.size(); i++)
		{
			total += abs(list1[i] - list2[i]);
		}
		cout << total << endl;
	}
	else
	{
		cout << "Lists aren't the same size!" << endl;
	}
	

	return 0;
}
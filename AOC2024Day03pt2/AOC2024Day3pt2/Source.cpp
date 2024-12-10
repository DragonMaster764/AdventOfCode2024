#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
vector<int> readCommand(string command);
void calcCommand(vector<int> v, int& total, bool enabled);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;
	int total = 0;
	bool enabled = true;
	while (getline(fileRead, input))
	{
		bool hasMul = true;
		while (hasMul)
		{
			string firstMul = "";

			if (input.find("mul") == -1)
			{
				hasMul = false;
			}
			else
			{
				string doDontStr = "";
				bool gatherChar = false;
				for (int i = 0; i < input.find_first_of("mul"); i++) 
				{
					char c = input[i];
					
					if (c == 'd' && !gatherChar)
					{
						gatherChar = true;
						doDontStr += c;
					}
					else if (gatherChar){
						doDontStr += c;
					}

					if (doDontStr.find("()") != -1)
					{
						gatherChar = false;
						/*cout << doDontStr << endl;*/

						if (doDontStr.find("do()") != -1)
						{
							enabled = true;
						}
						else if (doDontStr.find("don't()") != -1)
						{
							enabled = false;
						}

						doDontStr = "";
					}
				}

				firstMul = input.substr(input.find_first_of("mul"));

				vector<int> result;
				result = readCommand(firstMul);
				calcCommand(result, total, enabled);

				if (result[2] != -1)
				{
					input = firstMul.substr(result[2] + 1);
					/*cout << input.substr(0, 20) << endl;*/

				}
				else
				{
					input = firstMul.substr(1);
				}
			}


		}
	}

	cout << "Total is: " << total << endl;

	return 0;
}

vector<int> readCommand(string command)
{
	vector<int> result;
	/*cout << command.substr(0, 20) << endl;*/

	if (command.find('(') != 3)
	{
		vector<int> invalid{ -1, -1, -1 };
		return invalid;
	}

	string inside = command.substr(command.find_first_of('(') + 1);
	bool onSecondNum = false;
	bool atEnd = false;
	int i = 0;

	string curNum = "";
	while (!atEnd)
	{
		char c = inside[i];

		if (!(48 <= c && c <= 57) && c != ',')
		{

			if (onSecondNum)
			{
				if (c == ')')
				{
					result.push_back(stoi(curNum));
					result.push_back(i);
					atEnd = true;
				}
				else
				{
					/*cout << "Error" << " " << c << endl;*/
					vector<int> invalid{ -1, -1, -1 };
					return invalid;
				}
			}
			else
			{
				/*cout << "Error" << " " << c << endl;*/
				vector<int> invalid{ -1, -1, -1 };
				return invalid;
			}

		}
		else
		{
			if (c == ',')
			{
				onSecondNum = true;
				result.push_back(stoi(curNum));
				curNum = "";
			}
			else
			{
				curNum += c;
			}
		}

		i++;
	}

	return result;

}

void calcCommand(vector<int> v, int& total, bool enabled)
{
	if (v[0] != -1 && v[1] != -1 && v[2] != -1 && enabled)
	{
		total += v[0] * v[1];
		/*cout << v[0] << " " << v[1] << endl;*/
	}
}
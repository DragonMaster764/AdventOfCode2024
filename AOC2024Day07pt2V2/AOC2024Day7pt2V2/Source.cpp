#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<int>& list, string nums);
bool allOPCombos(const vector<int>& rowOfNums, int sizeArray, long long total);
//long long allCombos(const vector<int>& rowOfNums, long long total);
long long vectorOperation(long long fNum, long long sNum, string op);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	vector<vector<int>> equationNumbers;
	vector<long long> totalNumbers;

	// Parse the data
	string input;
	while (getline(fileRead, input))
	{
		//This pushes the number that the other nums should be equaling if it works
		totalNumbers.push_back(stoll(input.substr(0, input.find_first_of(':'))));

		vector<int> rowOfNums;
		string otherNums = input.substr(input.find_first_of(':') + 1);
		readNumbers(rowOfNums, otherNums);

		/*cout << stoi(input.substr(0, input.find_first_of(':'))) << " : ";
		for (int i = 0; i < rowOfNums.size(); i++)
		{
			cout << rowOfNums[i] << " ";
		}
		cout << endl;*/

		equationNumbers.push_back(rowOfNums);
	}


	long long total = 0;

	//Do every combo and save the total if it works
	for (int i = 0; i < totalNumbers.size(); i++)
	{
		/*total += allCombos(equationNumbers[i], totalNumbers[i]);*/
		if (allOPCombos(equationNumbers[i], equationNumbers[i].size(), totalNumbers[i]))
		{
			total += totalNumbers[i];
		}
	}

	cout << "Total: " << total << endl;

	return 0;
}

//long long allCombos(const vector<int>& rowOfNums, long long total)
//{
//	bool doesWork = false;
//	vector<string> ops = { "+",  "*", "||" };
//
//	vector<vector<string>> opCombos = allOPCombos(ops, rowOfNums.size());
//
//	for (int i = 0; i < opCombos.size(); i++)
//	{
//		long long prevTotal = 0;
//		prevTotal = vectorOperation(rowOfNums[0], rowOfNums[1], opCombos[i][0]);
//
//		for (int j = 1; j < opCombos[i].size(); j++)
//		{
//			long long newTotal = 0;
//			newTotal = vectorOperation(prevTotal, rowOfNums[j + 1], opCombos[i][j]);
//			prevTotal = newTotal;
//		}
//
//		if (prevTotal == total)
//		{
//			doesWork = true;
//			/*cout << opCombos[i];*/
//			break;
//		}
//	}
//
//	/*cout << doesWork << endl;*/
//
//	if (doesWork)
//	{
//		return total;
//	}
//	else
//	{
//		return 0;
//	}
//}

bool allOPCombos(const vector<int> & rowOfNums, int sizeArray, long long total)
{
	vector<string> ops = { "+", "*", "||"};
	vector<vector<string>> combos;

	for (int i = 0; i < ops.size(); i++)
	{
		vector<string> opVector = { ops[i] };
		combos.push_back(opVector);
	}

	if (sizeArray == 2)
	{
		for (int i = 0; i < ops.size(); i++)
		{
			long long newTotal = 0;
			newTotal = vectorOperation(rowOfNums[0], rowOfNums[1], ops[i]);

			if (newTotal == total)
			{
				return true;
			}
		}
	}

	for (int i = 0; i < sizeArray - 2; i++)
	{
		vector<vector<string>> temp;

		for (int j = 0; j < ops.size(); j++)
		{

			for (int k = 0; k < combos.size(); k++)
			{
				vector<string> newOpString = combos[k];
				newOpString.push_back(ops[j]);
				temp.push_back(newOpString);

				if (newOpString.size() == sizeArray - 1)
				{
					long long prevTotal = 0;
					prevTotal = vectorOperation(rowOfNums[0], rowOfNums[1], newOpString[0]);

					for (int j = 1; j < newOpString.size(); j++)
					{
						long long newTotal = 0;
						newTotal = vectorOperation(prevTotal, rowOfNums[j + 1], newOpString[j]);
						prevTotal = newTotal;
					}

					if (prevTotal == total)
					{
						return true;
					}
				}
			}
		}

		combos = temp;
	}

	/*cout << "2^" << sizeArray-1 << "= " << pow(2, sizeArray - 1) << " | Actual size: " << combos.size() << " ";*/

	/*for (int i = 0; i < combos.size(); i++)
	{
		cout << combos[i] << " | ";
	}
	cout << endl;*/

	return false;
}

long long vectorOperation(long long fNum, long long sNum, string op)
{

	if (op == "+")
	{
		return fNum + sNum;
	}
	else if (op == "*")
	{
		return fNum * sNum;;
	}
	else if (op == "||")
	{
		string strFNum = to_string(fNum);
		string strSNum = to_string(sNum);
		string concatNum = strFNum + strSNum;

		return stoll(concatNum);
	}

	return -1;
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

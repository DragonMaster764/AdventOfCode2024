#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void readNumbers(vector<int>& list, string nums);
void readRegisterNumber(long long& registerNum, string registerString);
void computeOperation(int opCode, int operand, long long& registerA, long long& registerB, long long& registerC,
	int& pointer, string& output);
long long findComboOperand(int operand, long long registerA, long long registerB, long long registerC);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;

	//Important variables in problem
	long long registerA;
	long long registerB;
	long long registerC;

	getline(fileRead, input);
	readRegisterNumber(registerA, input);
	getline(fileRead, input);
	readRegisterNumber(registerB, input);
	getline(fileRead, input);
	readRegisterNumber(registerC, input);

	/*cout << registerA << endl;
	cout << registerB << endl;
	cout << registerC << endl;*/

	getline(fileRead, input);

	while (!input.size()) //While there are spaces, get next line
	{
		getline(fileRead, input);

	}

	// Make a vector for the numbers in the row
	vector<int> instructionList;

	//Get the numbers
	readNumbers(instructionList, input);

	////Reads out the number list 
	//for (int i = 0; i < instructionList.size(); i++)
	//{
	//	cout << instructionList[i] << " ";
	//}
	//cout << endl;

	//Starting pointer for instructions
	int instructPointer = 0;

	//String output string
	string output;

	/*int opCompleted = 1;*/

	//Keep going while instruction pointer is still in bounds 
	while (instructPointer + 1 < instructionList.size())
	{
		int opCode = instructionList[instructPointer];
		int operand = instructionList[instructPointer + 1];

		//Compute operation
		computeOperation(opCode, operand, registerA, registerB, registerC, instructPointer, output);

		if (opCode != 3)
		{
			instructPointer += 2;
		}

		/*cout << "Operation Completed" << opCompleted << endl;
		opCompleted++;*/
	}

	cout << "Output is below: " << endl;
	cout << output.substr(0, output.find_last_of(',')) << endl; //Gets rid of the extra ',' at the end

	return 0;
}

//In charge of doing the main beefy part of the problem (operations)
void computeOperation(int opCode, int operand, long long& registerA, long long& registerB, long long & registerC,
	int& pointer, string & output)
{
	if (opCode == 0)
	{
		long long numerator = registerA;
		long long denominator = pow(2, findComboOperand(operand, registerA, registerB, registerC));
		long long divResult = numerator / denominator;
		registerA = divResult;
	}
	else if (opCode == 1)
	{
		registerB = registerB ^ operand;
	}
	else if (opCode == 2)
	{
		long long comboOperand = findComboOperand(operand, registerA, registerB, registerC);
		registerB = comboOperand % 8;
	}
	else if (opCode == 3)
	{
		if (registerA != 0)
		{
			pointer = operand;
		}
		else
		{
			pointer += 2;
		}
	}
	else if (opCode == 4)
	{
		registerB = registerB ^ registerC;
	}
	else if (opCode == 5)
	{
		long long modResult = findComboOperand(operand, registerA, registerB, registerC) % 8;
		string modString = to_string(modResult);

		//If more than one digit, separate by comma
		for (char c: modString)
		{
			output += c;
			output += ',';
		}
	}
	else if (opCode == 6)
	{
		long long numerator = registerA;
		long long denominator = pow(2, findComboOperand(operand, registerA, registerB, registerC));
		long long divResult = numerator / denominator;
		registerB = divResult;
	}
	else if (opCode == 7)
	{
		long long numerator = registerA;
		long long denominator = pow(2, findComboOperand(operand, registerA, registerB, registerC));
		long long divResult = numerator / denominator;
		registerC = divResult;
	}
}

//Returns the "combo operand"
long long findComboOperand(int operand, long long registerA, long long registerB, long long registerC)
{
	if (operand >= 0 && operand <= 3)
	{
		return operand;
	}
	else if (operand == 4)
	{
		return registerA;
	}
	else if (operand == 5)
	{
		return registerB;
	}
	else if (operand == 6)
	{
		return registerC;
	}
	else if (operand == 7)
	{
		cout << "Invalid" << endl;
		return -1;
	}

	return -1;
}

void readRegisterNumber(long long& registerNum, string registerString)
{
	string numString = "";
	for (int i = 0; i < registerString.size(); i++)
	{
		char c = registerString[i];
		if (48 <= c && c <= 57)
		{
			numString += registerString[i];
		}
		else if (numString != "" && c == ' ')
		{
			registerNum = stoll(numString);
			return;
		}
	}

	if (numString != "")
	{
		registerNum = stoll(numString);
		return;
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
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct algoState
{
	long long posPosition;
	int workingDigit;

	algoState(long long pos, int curWorkingDigit)
	{
		posPosition = pos;
		workingDigit = curWorkingDigit;
	}
};

// Methods
void readNumbers(vector<int>& list, string nums);
void readRegisterNumber(long long& registerNum, string registerString);
void computeOperation(int opCode, int operand, long long& registerA, long long& registerB, long long& registerC,
	int& pointer, string& output);
long long findComboOperand(int operand, long long registerA, long long registerB, long long registerC);
bool numberListsEqual(string curOutput, const vector<int>& instructionList);
string algorithm(vector<int>& instructionList, long long registerA);
bool checkDigits(string curOutput, const vector<int>& instructionList, int curDigit);

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

	string output;

	getline(fileRead, input);

	while (!input.size()) //While there are spaces, get next line
	{
		getline(fileRead, input);

	}

	// Make a vector for the numbers in the row
	vector<int> instructionList;

	//Get the numbers
	readNumbers(instructionList, input);

	long long firstA = 0;

	/*for (int i = 0; i < 101; i++)
	{
		long long posA = curA + i;
		algorithm(instructionList, posA);
	}*/

	long long firstWorkingDigit = 1; // First looking for last digit

	queue<algoState> posStates;
	posStates.push(algoState(firstA, firstWorkingDigit));

	while (!posStates.empty())
	{
		algoState curState = posStates.front();
		posStates.pop();

		long long curA = curState.posPosition;
		int curWorkingDigit = curState.workingDigit;

		for (int i = 0; i < 8; i++)
		{
			long long posA = curA + i;
			output = algorithm(instructionList, posA);

			//If the amount of digits are the same from the end,
			//break and move on
			if (checkDigits(output, instructionList, curWorkingDigit))
			{

				if (curWorkingDigit == 16 && numberListsEqual(output, instructionList))
				{
					cout << "Solution A: " << posA << endl;
					cout << output << endl << endl;
				}
				else if (curWorkingDigit < 16)
				{
					long long nextA = posA;
					int nextWorkingDigit = curWorkingDigit + 1;

					//Find the next curA to start with
					if (curWorkingDigit == 1)
					{
						nextA = pow(8, curWorkingDigit) + (8 * posA) - 8;
						algoState newState(nextA, nextWorkingDigit);
						posStates.push(newState);
					}
					else
					{
						long long basisNum = posA - pow(8, curWorkingDigit - 1);
						nextA = pow(8, curWorkingDigit) + (basisNum * 8);
						algoState newState(nextA, nextWorkingDigit);
						posStates.push(newState);
					}
				}

			}
		}
	}

	return 0;
}

bool checkDigits(string curOutput, const vector<int>& instructionList, int curDigit)
{
	vector<int> outputList;
	readNumbers(outputList, curOutput);

	int curOutputIndex = outputList.size() - 1;

	for (int i = instructionList.size() - 1; i > instructionList.size() - 1 - curDigit; i--)
	{
		if (instructionList[i] != outputList[curOutputIndex])
		{
			return false;
		}

		curOutputIndex--;
	}

	return true;
}

//Checks to see if output and program are the same
bool numberListsEqual(string curOutput, const vector<int>& instructionList)
{
	vector<int> outputList;
	readNumbers(outputList, curOutput);

	if (instructionList.size() != outputList.size())
	{
		return false;
	}

	for (int i = 0; i < instructionList.size(); i++)
	{
		if (outputList[i] != instructionList[i])
		{
			return false;
		}
	}

	return true;
}

string algorithm(vector<int>& instructionList, long long registerA)
{

	long long i = registerA;

	long long registerB = 0;
	long long registerC = 0;

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

	//cout << "A: " << i << " " << "Output is below: " << endl;
	//cout << output.substr(0, output.find_last_of(',')) << endl; //Gets rid of the extra ',' at the end

	return output;
}

//In charge of doing the main beefy part of the problem (operations)
void computeOperation(int opCode, int operand, long long& registerA, long long& registerB, long long& registerC,
	int& pointer, string& output)
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
		for (char c : modString)
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
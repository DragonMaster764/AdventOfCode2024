#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
vector<int> findGuard(const vector<vector<char>>& map);
vector<int> guardAlgorithm(vector<vector<char>>& map, vector<int> curPos, int& curDir);
vector<vector<int>> findXs(const vector<vector<char>>& map);
vector<int> newGuardAlgorithm(vector<vector<char>>& map, vector<int> curPos, int& curDir);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;

	//Map 
	vector<vector<char>> ogMap;

	while (getline(fileRead, input))
	{
		vector<char> row;

		for (int i = 0; i < input.size(); i++)
		{
			row.push_back(input[i]);
		}

		ogMap.push_back(row);
	}

	fileRead.close();

	//Find starting coords
	vector<int> startPos = findGuard(ogMap);

	//Used in algorithm
	int curDirection = 0;
	vector<vector<char>> xMap = ogMap;

	//While the position isn't outside the boundaires, keep going
	vector<int> curPos = startPos;
	while ((curPos[0] >= 0 && curPos[0] < xMap.size()) && (curPos[1] >= 0 && curPos[1] < xMap[0].size()))
	{
		//Set curPos to nextPos
		vector<int> nextPos = guardAlgorithm(xMap, curPos, curDirection);

		if (nextPos[0] == -1 && nextPos[1] == -1)
		{
			break;
		}
		else
		{
			curPos = nextPos;
		}
	}

	/*for (int i = 0; i < xMap.size(); i++)
	{
		for (int j = 0; j < xMap[i].size(); j++)
		{
			cout << xMap[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/

	vector<vector<int>> xLocations = findXs(xMap);

	//Part two, loops
	curDirection = 0;
	curPos = startPos;

	int totalLoops = 0;

	for (int i = 0; i < xLocations.size(); i++)
	{
		vector<vector<char>> newMap = ogMap;
		newMap[xLocations[i][0]].erase(newMap[xLocations[i][0]].begin() + xLocations[i][1]);
		newMap[xLocations[i][0]].insert(newMap[xLocations[i][0]].begin() + xLocations[i][1], 'O');
		bool inLoop = true;

		while ((curPos[0] >= 0 && curPos[0] < newMap.size()) && (curPos[1] >= 0 && curPos[1] < newMap[0].size()))
		{
			//Set curPos to nextPos
			vector<int> nextPos = newGuardAlgorithm(newMap, curPos, curDirection);

			if (nextPos[0] == -1 && nextPos[1] == -1)
			{
				inLoop = false;
				break;
			}
			else if (nextPos[0] == -2 && nextPos[1] == -2)
			{
				break;
			}
			else
			{
				curPos = nextPos;
			}
		}

		if (!(curPos[0] >= 0 && curPos[0] < newMap.size()) || !(curPos[1] >= 0 && curPos[1] < newMap[0].size()))
		{
			inLoop = false;
		}


		if (inLoop)
		{
			totalLoops++;

			/*for (int i = 0; i < newMap.size(); i++)
			{
				for (int j = 0; j < newMap[i].size(); j++)
				{
					cout << newMap[i][j] << " ";
				}
				cout << endl;
			}
			cout << endl << endl;*/
		}

		curPos = startPos;
		curDirection = 0;
	}


	//for (int i = 0; i < ogMap.size(); i++)
	//{
	//	for (int j = 0; j < ogMap[i].size(); j++)
	//	{
	//		vector<vector<char>> newMap = ogMap;
	//		newMap[i].erase(newMap[i].begin() + j);
	//		newMap[i].insert(newMap[i].begin() + j, 'O');
	//		bool inLoop = true;

	//		while ((curPos[0] >= 0 && curPos[0] < newMap.size()) && (curPos[1] >= 0 && curPos[1] < newMap[0].size()))
	//		{
	//			//Set curPos to nextPos
	//			vector<int> nextPos = newGuardAlgorithm(newMap, curPos, curDirection);

	//			if (nextPos[0] == -1 && nextPos[1] == -1)
	//			{
	//				inLoop = false;
	//				break;
	//			}
	//			else if (nextPos[0] == -2 && nextPos[1] == -2)
	//			{
	//				break;
	//			}
	//			else
	//			{
	//				curPos = nextPos;
	//			}
	//		}

	//		if (!(curPos[0] >= 0 && curPos[0] < newMap.size()) || !(curPos[1] >= 0 && curPos[1] < newMap[0].size()))
	//		{
	//			inLoop = false;
	//		}


	//		if (inLoop)
	//		{
	//			totalLoops++;

	//			for (int i = 0; i < newMap.size(); i++)
	//			{
	//				for (int j = 0; j < newMap[i].size(); j++)
	//				{
	//					cout << newMap[i][j] << " ";
	//				}
	//				cout << endl;
	//			}
	//			cout << endl << endl;
	//		}

	//		curPos = startPos;
	//		curDirection = 0;
	//	}
	//}

	cout << "Total loops: " << totalLoops << endl;

	return 0;
}

vector<int> findGuard(const vector<vector<char>>& map)
{
	vector<int> startCoords;

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == '^')
			{
				return startCoords = { i, j }; // row, column in map
			}
		}
	}

	return { -1, -1 };
}

vector<int> guardAlgorithm(vector<vector<char>>& map, vector<int> curPos, int& curDir)
{
	//Algorithm to change the direction 
	vector<int> xDir = { 0, 1, 0, -1 };
	vector<int> yDir = { -1, 0, 1, 0 };

	//If cur pos is not X, replace it
	if (map[curPos[0]][curPos[1]] != 'X')
	{
		map[curPos[0]].erase(map[curPos[0]].begin() + curPos[1]);
		map[curPos[0]].insert(map[curPos[0]].begin() + curPos[1], 'X');
	}

	//Move one direction in direction faced
	vector<int> nextPos = { curPos[0] + yDir[curDir], curPos[1] + xDir[curDir] };

	//If next position is out of bounds, break loop
	if (!(nextPos[0] >= 0 && nextPos[0] < map.size()) || !(nextPos[1] >= 0 && nextPos[1] < map[0].size()))
	{
		return { -1, -1 };
	}

	//If there is a # in spot, move 90 degrees right (curDir++)
	while (map[nextPos[0]][nextPos[1]] == '#')
	{
		if (curDir < xDir.size() - 1)
		{
			curDir++;

			//new spot
			nextPos = { curPos[0] + yDir[curDir], curPos[1] + xDir[curDir] };
		}
		else
		{
			curDir = 0;

			//new spot
			nextPos = { curPos[0] + yDir[curDir], curPos[1] + xDir[curDir] };
		}
	}

	return nextPos;
}

vector<vector<int>> findXs(const vector<vector<char>>& map)
{
	vector<vector<int>> xLocations;

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == 'X')
			{
				xLocations.push_back({ i, j });
				cout << i << " " << j << endl;
			}
		}
	}

	return xLocations;
}

vector<int> newGuardAlgorithm(vector<vector<char>>& map, vector<int> curPos, int& curDir)
{
	//Algorithm to change the direction 
	vector<int> xDir = { 0, 1, 0, -1 };
	vector<int> yDir = { -1, 0, 1, 0 };

	//Move one direction in direction faced
	vector<int> nextPos = { curPos[0] + yDir[curDir], curPos[1] + xDir[curDir] };
	vector<int> newPlusPos = { -1, -1 };

	//If next position is out of bounds, break loop
	if (!(nextPos[0] >= 0 && nextPos[0] < map.size()) || !(nextPos[1] >= 0 && nextPos[1] < map[0].size()))
	{
		return { -1, -1 };
	}

	//If cur pos is not X or +, replace it, if nextPos is # make it a +
	if (map[curPos[0]][curPos[1]] != 'X' && map[curPos[0]][curPos[1]] != '+')
	{

		if (map[nextPos[0]][nextPos[1]] == '#' || map[nextPos[0]][nextPos[1]] == 'O')
		{
			map[curPos[0]].erase(map[curPos[0]].begin() + curPos[1]);
			map[curPos[0]].insert(map[curPos[0]].begin() + curPos[1], '+');
			newPlusPos = curPos;
		}
		else
		{
			map[curPos[0]].erase(map[curPos[0]].begin() + curPos[1]);
			map[curPos[0]].insert(map[curPos[0]].begin() + curPos[1], 'X');
		}

	}

	if (map[curPos[0]][curPos[1]] == 'X' && map[curPos[0]][curPos[1]] != '+'
		&& map[nextPos[0]][nextPos[1]] == '#' || map[nextPos[0]][nextPos[1]] == 'O')
	{
		map[curPos[0]].erase(map[curPos[0]].begin() + curPos[1]);
		map[curPos[0]].insert(map[curPos[0]].begin() + curPos[1], '+');
		newPlusPos = curPos;
	}


	//If cur spot is a + and next is a # or O we are in a loop
	if ((map[curPos[0]][curPos[1]] == '+' && (map[nextPos[0]][nextPos[1]] == '#') || map[nextPos[0]][nextPos[1]] == 'O')
		&& newPlusPos != curPos)
	{
		return { -2, -2 };
	}

	//If there is a # in spot, move 90 degrees right (curDir++)
	while (map[nextPos[0]][nextPos[1]] == '#' || map[nextPos[0]][nextPos[1]] == 'O')
	{
		if (curDir < xDir.size() - 1)
		{
			curDir++;

			//new spot
			nextPos = { curPos[0] + yDir[curDir], curPos[1] + xDir[curDir] };
		}
		else
		{
			curDir = 0;

			//new spot
			nextPos = { curPos[0] + yDir[curDir], curPos[1] + xDir[curDir] };
		}
	}

	return nextPos;
}
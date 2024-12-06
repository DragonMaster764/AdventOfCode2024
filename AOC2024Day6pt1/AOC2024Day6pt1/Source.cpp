#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
vector<int> findGuard(const vector<vector<char>>& map);
vector<int> guardAlgorithm(vector<vector<char>>& map, vector<int> curPos, int& curDir);
int countXs(const vector<vector<char>>& map);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;
	
	//Map 
	vector<vector<char>> map;

	while (getline(fileRead, input))
	{
		vector<char> row;

		for (int i = 0; i < input.size(); i++)
		{
			row.push_back(input[i]);
		}

		map.push_back(row);
	}

	//Find starting coords
	vector<int> startPos = findGuard(map);

	//Used in algorithm
	int curDirection = 0;

	//While the position isn't outside the boundaires, keep going
	vector<int> curPos = startPos;
	while ((curPos[0] >= 0 && curPos[0] < map.size()) && (curPos[1] >= 0 && curPos[1] < map[0].size()))
	{
		//Set curPos to nextPos
		vector<int> nextPos = guardAlgorithm(map, curPos, curDirection);
		
		if (nextPos[0] == -1 && nextPos[1] == -1)
		{
			break;
		}
		else
		{
			curPos = nextPos;
		}
	}

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Distinct Positions: " << countXs(map);

	return 0;
}

vector<int> findGuard(const vector<vector<char>> & map)
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

vector<int> guardAlgorithm(vector<vector<char>>& map, vector<int> curPos, int & curDir)
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

int countXs(const vector<vector<char>>& map)
{
	int totalX = 0;

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == 'X')
			{
				totalX++;
			}
		}
	}

	return totalX;
}
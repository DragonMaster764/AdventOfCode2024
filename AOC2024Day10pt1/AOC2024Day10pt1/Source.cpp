#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
int findTrailHeads(const vector<vector<int>>& topoMap, const vector<int>& zeroCoords);
int moveOnTrail(const vector<vector<int>>& topoMap, const vector<int>& curCoords, int curElevation, vector<vector<int>> & history);
bool inHistory(vector<int> coords, const vector<vector<int>>& history);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	//Map of values
	vector<vector<int>> topoMap;

	// Parse the data
	string input;
	while (getline(fileRead, input))
	{
		vector<int> row;

		for (int i = 0; i < input.size(); i++)
		{
			row.push_back(input[i] - '0');
		}

		topoMap.push_back(row);
	}

	//Testing
	for (int i = 0; i < topoMap.size(); i++)
	{
		for (int j = 0; j < topoMap[i].size(); j++)
		{
			cout << topoMap[i][j];
		}
		cout << endl;
	}

	//total variable
	long totalPaths = 0;

	//Find the zero and pass the coords to the zero to find trails
	for (int i = 0; i < topoMap.size(); i++)
	{
		for (int j = 0; j < topoMap[i].size(); j++)
		{
			if (topoMap[i][j] == 0)
			{
				totalPaths += findTrailHeads(topoMap, { i, j });
			}
		}

	}

	cout << "Total paths from trailheads = " << totalPaths << endl;

	return 0;
}

int findTrailHeads(const vector<vector<int>>& topoMap, const vector<int>& zeroCoords)
{
	cout << "Zero Coords: " << zeroCoords[0] << ", " << zeroCoords[1] << endl;

	vector<int> xDir = { -1, 0, 1, 0 }; //left, down, right, up
	vector<int> yDir = { 0, 1, 0, -1 };

	int curElevationNum = 0;
	int total = 0;
	vector<vector<int>> history;
	history.push_back(zeroCoords);

	//Check each direction for a number one above curElevation
	for (int i = 0; i < xDir.size(); i++)
	{
		vector<int> newCoords = { zeroCoords[0] + yDir[i], zeroCoords[1] + xDir[i] };
		
		//Check it's in bounds
		if ((newCoords[0] >= 0 && newCoords[0] < topoMap.size())
			&& (newCoords[1] >= 0 && newCoords[1] < topoMap[0].size()))
		{
			//Check to the see if the new coords is one above cur elevation
			if (topoMap[newCoords[0]][newCoords[1]] == (curElevationNum + 1))
			{
				// Continue on the path
				history.push_back(newCoords);
				total += moveOnTrail(topoMap, newCoords, curElevationNum + 1, history);
			}
		}
		
	}

	cout << "-----------------" << endl;

	return total;
}

int moveOnTrail(const vector<vector<int>>& topoMap, const vector<int>& curCoords, int curElevation, vector<vector<int>> & history)
{
	vector<int> xDir = { -1, 0, 1, 0 }; //left, down, right, up
	vector<int> yDir = { 0, 1, 0, -1 };

	int total = 0;

	//If we hit the end, return a one
	if (curElevation == 9)
	{
		cout << "Found a nine at: " << curCoords[0] << ", " << curCoords[1] << endl;
		return 1;
	}

	//Check each direction for a number one above curElevation
	for (int i = 0; i < xDir.size(); i++)
	{
		vector<int> newCoords = { curCoords[0] + yDir[i], curCoords[1] + xDir[i] };

		//Check it's in bounds 
		if ((newCoords[0] >= 0 && newCoords[0] < topoMap.size())
			&& (newCoords[1] >= 0 && newCoords[1] < topoMap[0].size()))
		{
			//Check to the see if the new coords is one above cur elevation 
			//and not one we have gone to before
			if (topoMap[newCoords[0]][newCoords[1]] == (curElevation + 1) && !inHistory(newCoords, history))
			{
				// Continue on the path
				history.push_back(newCoords);
				total += moveOnTrail(topoMap, newCoords, curElevation + 1, history);
			}
		}
		
	}

	return total;
}

bool inHistory(vector<int> coords, const vector<vector<int>>& history)
{
	for (int i = 0; i < history.size(); i++)
	{
		if (history[i][0] == coords[0] && history[i][1] == coords[1])
		{
			return true;
		}
	}

	return false;
}

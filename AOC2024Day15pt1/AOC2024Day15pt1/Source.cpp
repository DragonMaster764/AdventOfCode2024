#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void followDirections(vector<vector<char>>& map, vector<int>& robotCoords, string directions);
bool inBounds(const vector<vector<char>>& map, const vector<int>& coords);
bool boxIsMovable(vector<vector<char>>& map, const vector<int>& initalBoxCoords, char direction);
long long calcGPSCoords(const vector<vector<char>>& map);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Map
	vector<vector<char>> map;

	//Robot position
	vector<int> robotCoords;

	// Parse the data
	string input;
	int curMapRow = 0;
	while (getline(fileRead, input))
	{
		vector<char> row;
		
		if (!input.size())
		{
			break;
		}

		for (int i = 0; i < input.size(); i++)
		{
			row.push_back(input[i]);

			//If we find the robot, mark where it is
			if (input[i] == '@')
			{
				robotCoords = { curMapRow, i };
			}
		}

		map.push_back(row);
		curMapRow++;
	}

	//After the space we need to get the directions
	while (getline(fileRead, input))
	{
		followDirections(map, robotCoords, input);
	}

	//Print final map
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			cout << map[i][j];
		}
		cout << endl;
	}

	//After all directions are followed, we need to calculate the GPSCoords
	long long totalGPSCoords = calcGPSCoords(map);

	cout << "Total GPS Coords Score: " << totalGPSCoords << endl;

	return 0;
}

void followDirections(vector<vector<char>>& map, vector<int>& robotCoords, string directions)
{
	//Follow each individual direction
	for (int i = 0; i < directions.size(); i++)
	{
		int posNewCoordRow = 0;
		int posNewCoordCol = 0;
		char dir = ' ';

		if (directions[i] == '^') //UP
		{
			posNewCoordRow = robotCoords[0] - 1;
			posNewCoordCol = robotCoords[1];
			dir = 'U';
		}
		else if (directions[i] == 'v') //DOWN
		{
			posNewCoordRow = robotCoords[0] + 1;
			posNewCoordCol = robotCoords[1];
			dir = 'D';
		}
		else if (directions[i] == '<') //LEFT
		{
			posNewCoordRow = robotCoords[0];
			posNewCoordCol = robotCoords[1] - 1;
			dir = 'L';
		}
		else if (directions[i] == '>') //RIGHT
		{
			posNewCoordRow = robotCoords[0];
			posNewCoordCol = robotCoords[1] + 1;
			dir = 'R';
		}

		vector<int> posCoords = { posNewCoordRow, posNewCoordCol };

		//Check posCoords are in bounds and that position isn't a wall #
		if (inBounds(map, posCoords))
		{
			if (map[posCoords[0]][posCoords[1]] != '#')
			{
				//If there is a box, we need to see if it is movable, and move it in 
				//the same direction
				if (map[posCoords[0]][posCoords[1]] == 'O')
				{
					//If you can move the box, the box will be moved, and the robot can move to the new position
					if (boxIsMovable(map, posCoords, dir))
					{
						//Move to the new position
						map[robotCoords[0]].erase(map[robotCoords[0]].begin() + robotCoords[1]);
						map[robotCoords[0]].insert(map[robotCoords[0]].begin() + robotCoords[1], '.'); //Replace old with '.'
						robotCoords = posCoords;
						map[robotCoords[0]].erase(map[robotCoords[0]].begin() + robotCoords[1]);
						map[robotCoords[0]].insert(map[robotCoords[0]].begin() + robotCoords[1], '@'); //Place new positon on grid
					}
				}
				else
				{
					//Just move to the new position
					map[robotCoords[0]].erase(map[robotCoords[0]].begin() + robotCoords[1]);
					map[robotCoords[0]].insert(map[robotCoords[0]].begin() + robotCoords[1], '.'); //Replace old with '.'
					robotCoords = posCoords;
					map[robotCoords[0]].erase(map[robotCoords[0]].begin() + robotCoords[1]);
					map[robotCoords[0]].insert(map[robotCoords[0]].begin() + robotCoords[1], '@'); //Place new positon on grid
				}
			}
		}

	}
}

bool inBounds(const vector<vector<char>> & map, const vector<int>& coords)
{
	if ((coords[0] >= 0 && coords[0] < map.size())
		&& (coords[1] >= 0 && coords[1] < map[0].size()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool boxIsMovable(vector<vector<char>>& map, const vector<int>& initalBoxCoords, char direction)
{
	vector<int> posNewBoxCoords = { -1, -1 };

	if (direction == 'U')
	{
		posNewBoxCoords[0] = initalBoxCoords[0] - 1;
		posNewBoxCoords[1] = initalBoxCoords[1];
	}
	else if (direction == 'D')
	{
		posNewBoxCoords[0] = initalBoxCoords[0] + 1;
		posNewBoxCoords[1] = initalBoxCoords[1];
	}
	else if (direction == 'L')
	{
		posNewBoxCoords[0] = initalBoxCoords[0];
		posNewBoxCoords[1] = initalBoxCoords[1] - 1;
	}
	else if (direction == 'R')
	{
		posNewBoxCoords[0] = initalBoxCoords[0];
		posNewBoxCoords[1] = initalBoxCoords[1] + 1;
	}
	else
	{
		return false;
	}

	//Check if the box is in bounds and doesn't hit a wall
	if (inBounds(map, posNewBoxCoords))
	{
		if (map[posNewBoxCoords[0]][posNewBoxCoords[1]] != '#')
		{
			//If there is another box in the way, check to see if it can move
			if (map[posNewBoxCoords[0]][posNewBoxCoords[1]] == 'O')
			{
				if (boxIsMovable(map, posNewBoxCoords, direction))
				{
					//Move
					//Remove the old spot and move box
					map[initalBoxCoords[0]].erase(map[initalBoxCoords[0]].begin() + initalBoxCoords[1]);
					map[initalBoxCoords[0]].insert(map[initalBoxCoords[0]].begin() + initalBoxCoords[1], '.');

					//Move box symbol
					map[posNewBoxCoords[0]].erase(map[posNewBoxCoords[0]].begin() + posNewBoxCoords[1]);
					map[posNewBoxCoords[0]].insert(map[posNewBoxCoords[0]].begin() + posNewBoxCoords[1], 'O');

					//Return true that it moved
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				//Just move
				//Remove the old spot and move box
				map[initalBoxCoords[0]].erase(map[initalBoxCoords[0]].begin() + initalBoxCoords[1]);
				map[initalBoxCoords[0]].insert(map[initalBoxCoords[0]].begin() + initalBoxCoords[1], '.');

				//Move box symbol
				map[posNewBoxCoords[0]].erase(map[posNewBoxCoords[0]].begin() + posNewBoxCoords[1]);
				map[posNewBoxCoords[0]].insert(map[posNewBoxCoords[0]].begin() + posNewBoxCoords[1], 'O');

				//Return true that it moved
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

long long calcGPSCoords(const vector<vector<char>>& map)
{
	long long totalGPSScore = 0;

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			//If it is a box, add the gps score
			if (map[i][j] == 'O')
			{
				totalGPSScore += 100 * i + j;
			}
		}
	}

	//Return score
	return totalGPSScore;
}
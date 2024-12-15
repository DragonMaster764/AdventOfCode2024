#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void followDirections(vector<vector<char>>& map, vector<int>& robotCoords, string directions, ofstream & fileWrite);
bool inBounds(const vector<vector<char>>& map, const vector<int>& coords);
bool boxIsMovable(vector<vector<char>>& map, const vector<int>& initalBoxCoords, char direction);
void moveBox(vector<vector<char>>& map, const vector<int>& initalBoxCoords, char direction);
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

			//If we find the robot, mark where it is
			if (input[i] == '@')
			{
				row.push_back('@');
				row.push_back('.');
			}
			else if (input[i] == '#')
			{
				row.push_back('#');
				row.push_back('#');
			}
			else if (input[i] == '.')
			{
				row.push_back('.');
				row.push_back('.');
			}
			else if (input[i] == 'O')
			{
				row.push_back('[');
				row.push_back(']');
			}
		}

		map.push_back(row);
		curMapRow++;
	}

	
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == '@')
			{
				robotCoords = { i, j };
			}
		}
	}

	ofstream fileWrite("output.txt");

	//After the space we need to get the directions
	while (getline(fileRead, input))
	{
		followDirections(map, robotCoords, input, fileWrite);
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

void followDirections(vector<vector<char>>& map, vector<int>& robotCoords, string directions, ofstream & fileWrite)
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
				if (map[posCoords[0]][posCoords[1]] == '[' || map[posCoords[0]][posCoords[1]] == ']')
				{
					//If you can move the box, the box will be moved, and the robot can move to the new position
					if (boxIsMovable(map, posCoords, dir))
					{
						//Move Box
						moveBox(map, posCoords, dir);

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

		//TESTING
		/*for (int i = 0; i < map.size(); i++)
		{
			for (int j = 0; j < map[i].size(); j++)
			{
				fileWrite << map[i][j];
			}
			fileWrite << endl;
		}
		fileWrite << directions[i] << endl << endl;*/
	}
}

bool inBounds(const vector<vector<char>>& map, const vector<int>& coords)
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
	vector<int> posLeftBoxCoord = { -1, -1 };
	vector<int> posRightBoxCoord = { -1, -1 };

	bool boxInLWay = false;
	bool boxInRWay = false;

	if (direction == 'U')
	{
		posLeftBoxCoord[0] = initalBoxCoords[0] - 1;
		posRightBoxCoord[0] = initalBoxCoords[0] - 1;

		if (map[initalBoxCoords[0]][initalBoxCoords[1]] == ']')
		{
			posLeftBoxCoord[1] = initalBoxCoords[1] - 1;
			posRightBoxCoord[1] = initalBoxCoords[1];
		}
		else
		{
			posLeftBoxCoord[1] = initalBoxCoords[1];
			posRightBoxCoord[1] = initalBoxCoords[1] + 1;
		}

		//See if box is in it's posPosition
		if (map[posLeftBoxCoord[0]][posLeftBoxCoord[1]] == '[' && map[posRightBoxCoord[0]][posRightBoxCoord[1]] == ']')
		{
			//Only move one box since it's one box above
			boxInLWay = true;
		}
		else
		{
			if (map[posLeftBoxCoord[0]][posLeftBoxCoord[1]] == ']')
			{
				boxInLWay = true;
			}

			if (map[posRightBoxCoord[0]][posRightBoxCoord[1]] == '[')
			{
				boxInRWay = true;
			}
		}
	}
	else if (direction == 'D')
	{
		posLeftBoxCoord[0] = initalBoxCoords[0] + 1;
		posRightBoxCoord[0] = initalBoxCoords[0] + 1;

		if (map[initalBoxCoords[0]][initalBoxCoords[1]] == ']')
		{
			posLeftBoxCoord[1] = initalBoxCoords[1] - 1;
			posRightBoxCoord[1] = initalBoxCoords[1];
		}
		else
		{
			posLeftBoxCoord[1] = initalBoxCoords[1];
			posRightBoxCoord[1] = initalBoxCoords[1] + 1;
		}

		//See if box is in it's posPosition
		if (map[posLeftBoxCoord[0]][posLeftBoxCoord[1]] == '[' && map[posRightBoxCoord[0]][posRightBoxCoord[1]] == ']')
		{
			//Only move one box since it's one box above
			boxInLWay = true;
		}
		else
		{
			if (map[posLeftBoxCoord[0]][posLeftBoxCoord[1]] == ']')
			{
				boxInLWay = true;
			}

			if (map[posRightBoxCoord[0]][posRightBoxCoord[1]] == '[')
			{
				boxInRWay = true;
			}
		}
	}
	else if (direction == 'L')
	{
		posRightBoxCoord[0] = initalBoxCoords[0];
		posRightBoxCoord[1] = initalBoxCoords[1] - 1;
		posLeftBoxCoord[0] = initalBoxCoords[0];
		posLeftBoxCoord[1] = initalBoxCoords[1] - 2;

		//See if box is in it's posPosition
		if (map[posLeftBoxCoord[0]][posLeftBoxCoord[1]] == ']')
		{
			boxInLWay = true;
		}
	}
	else if (direction == 'R')
	{
		posLeftBoxCoord[0] = initalBoxCoords[0];
		posLeftBoxCoord[1] = initalBoxCoords[1] + 1;
		posRightBoxCoord[0] = initalBoxCoords[0];
		posRightBoxCoord[1] = initalBoxCoords[1] + 2;

		//See if box is in it's posPosition
		if (map[posRightBoxCoord[0]][posRightBoxCoord[1]] == '[')
		{
			boxInRWay = true;
		}
	}
	else
	{
		return false;
	}

	//Check if the box is in bounds and doesn't hit a wall
	if (inBounds(map, posLeftBoxCoord) && inBounds(map, posRightBoxCoord))
	{
		if (map[posLeftBoxCoord[0]][posLeftBoxCoord[1]] != '#' && map[posRightBoxCoord[0]][posRightBoxCoord[1]] != '#')
		{
			//If there is another box in the way, check to see if it can move
			if (boxInLWay || boxInRWay)
			{
				bool boxesAreMovable = false;

				if (boxInLWay)
				{
					boxesAreMovable = boxIsMovable(map, posLeftBoxCoord, direction);

					if (boxesAreMovable == false)
					{
						return false;
					}
				}

				if (boxInRWay)
				{
					boxesAreMovable = boxIsMovable(map, posRightBoxCoord, direction);

					if (boxesAreMovable == false)
					{
						return false;
					}
				}

				if (boxesAreMovable)
				{
					//Move all boxes
					if (boxInLWay)
					{
						moveBox(map, posLeftBoxCoord, direction);
					}

					if (boxInRWay)
					{
						moveBox(map, posRightBoxCoord, direction);
					}

					//Return true that it can move
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
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

void moveBox(vector<vector<char>>& map, const vector<int>& initalBoxCoords, char direction)
{
	vector<int> posLeftBoxCoord = { -1, -1 };
	vector<int> posRightBoxCoord = { -1, -1 };
	bool initalIsRightSide = false;

	if (direction == 'U')
	{
		posLeftBoxCoord[0] = initalBoxCoords[0] - 1;
		posRightBoxCoord[0] = initalBoxCoords[0] - 1;

		if (map[initalBoxCoords[0]][initalBoxCoords[1]] == ']')
		{
			posLeftBoxCoord[1] = initalBoxCoords[1] - 1;
			posRightBoxCoord[1] = initalBoxCoords[1];
			initalIsRightSide = true;
		}
		else
		{
			posLeftBoxCoord[1] = initalBoxCoords[1];
			posRightBoxCoord[1] = initalBoxCoords[1] + 1;
		}
	}
	else if (direction == 'D')
	{
		posLeftBoxCoord[0] = initalBoxCoords[0] + 1;
		posRightBoxCoord[0] = initalBoxCoords[0] + 1;

		if (map[initalBoxCoords[0]][initalBoxCoords[1]] == ']')
		{
			posLeftBoxCoord[1] = initalBoxCoords[1] - 1;
			posRightBoxCoord[1] = initalBoxCoords[1];
			initalIsRightSide = true;
		}
		else
		{
			posLeftBoxCoord[1] = initalBoxCoords[1];
			posRightBoxCoord[1] = initalBoxCoords[1] + 1;
		}
	}
	else if (direction == 'L')
	{
		posRightBoxCoord[0] = initalBoxCoords[0];
		posRightBoxCoord[1] = initalBoxCoords[1] - 1;

		posLeftBoxCoord[0] = initalBoxCoords[0];
		posLeftBoxCoord[1] = initalBoxCoords[1] - 2;

		initalIsRightSide = true;
	}
	else if (direction == 'R')
	{
		posLeftBoxCoord[0] = initalBoxCoords[0];
		posLeftBoxCoord[1] = initalBoxCoords[1] + 1;
		posRightBoxCoord[0] = initalBoxCoords[0];
		posRightBoxCoord[1] = initalBoxCoords[1] + 2;
	}

	//Erase Box
	map[initalBoxCoords[0]].erase(map[initalBoxCoords[0]].begin() + initalBoxCoords[1]);
	map[initalBoxCoords[0]].insert(map[initalBoxCoords[0]].begin() + initalBoxCoords[1], '.');

	if (initalIsRightSide)
	{
		//Erase left side
		map[initalBoxCoords[0]].erase(map[initalBoxCoords[0]].begin() + initalBoxCoords[1] - 1);
		map[initalBoxCoords[0]].insert(map[initalBoxCoords[0]].begin() + initalBoxCoords[1] - 1, '.');
	}
	else
	{
		//Erase right side
		map[initalBoxCoords[0]].erase(map[initalBoxCoords[0]].begin() + initalBoxCoords[1] + 1);
		map[initalBoxCoords[0]].insert(map[initalBoxCoords[0]].begin() + initalBoxCoords[1] + 1, '.');
	}

	//Move to new spot
	map[posLeftBoxCoord[0]].erase(map[posLeftBoxCoord[0]].begin() + posLeftBoxCoord[1]);
	map[posLeftBoxCoord[0]].insert(map[posLeftBoxCoord[0]].begin() + posLeftBoxCoord[1], '[');
	map[posRightBoxCoord[0]].erase(map[posRightBoxCoord[0]].begin() + posRightBoxCoord[1]);
	map[posRightBoxCoord[0]].insert(map[posRightBoxCoord[0]].begin() + posRightBoxCoord[1], ']');

}

long long calcGPSCoords(const vector<vector<char>>& map)
{
	long long totalGPSScore = 0;

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			//If it is a box, add the gps score
			if (map[i][j] == '[')
			{
				totalGPSScore += 100 * i + j;
			}
		}
	}

	//Return score
	return totalGPSScore;
}
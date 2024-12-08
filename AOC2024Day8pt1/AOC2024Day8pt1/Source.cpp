#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
int countAntinodes(const vector<vector<char>>& map);
void findSameLetters(const vector<vector<char>>& grid, vector<vector<int>>& antennaCoords);
void placeAntinode(int fNumRow, int fNumColumn, int sNumRow, int sNumColumn, vector<vector<char>>& antiGrid);
void testPairs(const vector<vector<char>>& grid, vector<vector<char>>& antiGrid, const vector<vector<int>>& antennaCoords);

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

	//Make an identical but only . for placing #
	vector<vector<char>> antinodeMap;

	for (int i = 0; i < map.size(); i++)
	{
		vector<char> newRow;

		for (int j = 0; j < map[i].size(); j++)
		{
			newRow.push_back('.');
			/*cout << "." << " ";*/
		}
		/*cout << endl;*/
		antinodeMap.push_back(newRow);
	}

	//Gets the groups of coords representing each unique character
	vector<vector<int>> antennaCoordsSameChar;
	findSameLetters(map, antennaCoordsSameChar);


	//Now take the same coords and find each unique pair between similar chars
	//Then, place a # in the correct spots if they fit on the grid
	testPairs(map, antinodeMap, antennaCoordsSameChar);

	for (int i = 0; i < antinodeMap.size(); i++)
	{
		for (int j = 0; j < antinodeMap[i].size(); j++)
		{
			cout << antinodeMap[i][j] << " ";
		}
		cout << endl;
	}

	//Now show the total unique spots
	cout << "Total: " << countAntinodes(antinodeMap) << endl;

	return 0;
}

void testPairs(const vector<vector<char>>& grid, vector<vector<char>>& antiGrid, const vector<vector<int>>& antennaCoords)
{
	//For each unique letter...
	for (int i = 0; i < antennaCoords.size(); i++)
	{
		//if there are more than just one pair of antennas...
		if (antennaCoords[i].size() > 4)
		{
			for (int j = 0; j < antennaCoords[i].size(); j += 2)
			{
				//This for loop k = j because once a coord has been paired with the others, we don't want
				//to revisit it again
				for (int k = j; k < antennaCoords[i].size(); k += 2)
				{
					//We don't want to pair the same coords with itself
					if (j != k)
					{
						placeAntinode(antennaCoords[i][j], antennaCoords[i][j + 1],
							antennaCoords[i][k], antennaCoords[i][k + 1], antiGrid);
					}
				}
			}
		}
		else if (antennaCoords[i].size() > 2)
		{
			placeAntinode(antennaCoords[i][0], antennaCoords[i][1],
				antennaCoords[i][1], antennaCoords[i][2], antiGrid);
		}
	}
}

void placeAntinode(int fNumRow, int fNumColumn, int sNumRow, int sNumColumn, vector<vector<char>>& antiGrid)
{
	//OLD CODE
	////Get the slope
	//int slopeRow = sNumRow - fNumRow;
	//int slopeColumn = sNumColumn - fNumColumn;

	//If one antiNode is further right, that one adds a # at a position equal to it's position
	//plus the slope, the other will get - slope. If one is not further right, the one that is higher row
	if ((fNumColumn > sNumColumn) || (fNumColumn == sNumColumn && fNumRow < sNumRow))
	{
		//Get the slope and add
		int firstAntiRow = fNumRow + (fNumRow - sNumRow);
		int firstAntiColumn = fNumColumn + (fNumColumn - sNumColumn);
		int secondAntiRow = sNumRow - (fNumRow - sNumRow);
		int secondAntiColumn = sNumColumn - (fNumColumn - sNumColumn);

		//Make sure it's in bounds
		if ((firstAntiRow >= 0 && firstAntiRow < antiGrid.size()) 
			&& (firstAntiColumn >= 0 && firstAntiColumn < antiGrid[0].size()))
		{
			if (antiGrid[firstAntiRow][firstAntiColumn] != '#')
			{
				antiGrid[firstAntiRow].erase(antiGrid[firstAntiRow].begin() + firstAntiColumn);
				antiGrid[firstAntiRow].insert(antiGrid[firstAntiRow].begin() + firstAntiColumn, '#');
			}
		}

		if ((secondAntiRow >= 0 && secondAntiRow < antiGrid.size())
			&& (secondAntiColumn >= 0 && secondAntiColumn < antiGrid[0].size()))
		{
			if (antiGrid[secondAntiRow][secondAntiColumn] != '#')
			{
				antiGrid[secondAntiRow].erase(antiGrid[secondAntiRow].begin() + secondAntiColumn);
				antiGrid[secondAntiRow].insert(antiGrid[secondAntiRow].begin() + secondAntiColumn, '#');
			}
		}
	}
	else
	{
		//Get the slope and add
		int firstAntiRow = sNumRow + (sNumRow - fNumRow);
		int firstAntiColumn = sNumColumn + (sNumColumn - fNumColumn);
		int secondAntiRow = fNumRow - (sNumRow - fNumRow);
		int secondAntiColumn = fNumColumn - (sNumColumn - fNumColumn);

		//Make sure it's in bounds
		if ((firstAntiRow >= 0 && firstAntiRow < antiGrid.size())
			&& (firstAntiColumn >= 0 && firstAntiColumn < antiGrid[0].size()))
		{
			if (antiGrid[firstAntiRow][firstAntiColumn] != '#')
			{
				antiGrid[firstAntiRow].erase(antiGrid[firstAntiRow].begin() + firstAntiColumn);
				antiGrid[firstAntiRow].insert(antiGrid[firstAntiRow].begin() + firstAntiColumn, '#');
			}
		}

		if ((secondAntiRow >= 0 && secondAntiRow < antiGrid.size())
			&& (secondAntiColumn >= 0 && secondAntiColumn < antiGrid[0].size()))
		{
			if (antiGrid[secondAntiRow][secondAntiColumn] != '#')
			{
				antiGrid[secondAntiRow].erase(antiGrid[secondAntiRow].begin() + secondAntiColumn);
				antiGrid[secondAntiRow].insert(antiGrid[secondAntiRow].begin() + secondAntiColumn, '#');
			}
		}
	}
}

void findSameLetters(const vector<vector<char>>& grid, vector<vector<int>>& antennaCoords)
{
	vector<char> uniqueChars;

	//Traverse grid looking for unique characters and marking them down in antennaCoords
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			bool isUnique = true;

			//check unique chars, if it's unique, a new letter is added / coords written down
			//else it will add the coords on to others like it
			for (int k = 0; k < uniqueChars.size(); k++)
			{
				if (uniqueChars[k] == grid[i][j] && grid[i][j] != '.')
				{
					isUnique = false;
					antennaCoords[k].push_back(i);
					antennaCoords[k].push_back(j);
					break;
				}
			}

			if (isUnique && grid[i][j] != '.')
			{
				antennaCoords.push_back({ i, j });
				uniqueChars.push_back(grid[i][j]);
			}
		}
	}

	//Testing
	/*for (int i = 0; i < antennaCoords.size(); i++)
	{
		for (int j = 0; j < antennaCoords[i].size(); j++)
		{
			cout << antennaCoords[i][j] << " ";
		}

		cout << endl;
	}*/
}


int countAntinodes(const vector<vector<char>>& map)
{
	int totalAnti = 0;

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == '#')
			{
				totalAnti++;
			}
		}
	}

	return totalAnti;
}
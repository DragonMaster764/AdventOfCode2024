#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

//Classes
class Region
{
private:
	int regionNum;
	vector<vector<int>> regionCoords;
	long long edges;
	long long corners;
public:
	Region()
	{
		regionNum = 0;
		regionCoords;
		edges = 0;
		corners = 0;
	}
	Region(int areaNum)
	{
		regionNum = areaNum;
		regionCoords;
		edges = 0;
		corners = 0;
	}
	void insertCoords(vector<int> coords)
	{
		regionCoords.push_back(coords);
	}
	void addEdges(long long count)
	{
		edges += count;
	}
	bool coordInRegion(vector<int> coords) const
	{
		for (int i = 0; i < regionCoords.size(); i++)
		{
			if (regionCoords[i] == coords)
			{
				return true;
			}
		}

		return false;
	}
	void addCorners(long long x)
	{
		corners += x;
	}
	long long calcPrice() const
	{
		//Testing
		/*cout << regionNum << endl;
		for (int i = 0; i < regionCoords.size(); i++)
		{
			cout << regionCoords[i][0] << ", " << regionCoords[i][1] << endl;
		}*/
		cout << regionNum << " " << corners * regionCoords.size() << " " << corners << endl;
		return corners * regionCoords.size();
	}
};

// Methods
void regionFinder(const vector<vector<char>>& grid, vector<Region>& allRegions);
bool coordsInEstablishedRegion(const vector<Region>& allRegions, const vector<int>& coords);
void createNewRegion(const vector<vector<char>>& grid, vector<Region>& allRegions, vector<int> & startCoords, int regionNum,
	char regionLetter);
void addLetterToRegion(const vector<vector<char>>& grid, Region& curRegion, vector<int> & coords, char regionLetter);
int calcSubtractNum(const vector<vector<char>>& grid, const vector<int>& coords, char regionLetter, int& up, int& down, int& right, int& left);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	//Grid of letters
	vector<vector<char>> grid;

	// Parse the data
	string input;
	while (getline(fileRead, input))
	{
		vector<char> row;

		for (int i = 0; i < input.size(); i++)
		{
			row.push_back(input[i]);
		}

		grid.push_back(row);
	}

	//List of all regions
	vector<Region> allRegions;

	regionFinder(grid, allRegions);

	//After finding all regions get the total
	long long total = 0;
	for (int i = 0; i < allRegions.size(); i++)
	{
		total += allRegions[i].calcPrice();
	}

	cout << "Total Price: " << total << endl;

	//TESTING
	/*for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			cout << grid[i][j];
		}
		cout << endl;
	}*/


	return 0;
}

void regionFinder(const vector<vector<char>>& grid, vector<Region>& allRegions)
{

	//Keeps track of region number
	int regionNum = 1;

	//Search the whole grid
	for (int i = 0; i < grid.size(); i++)
	{
		//Each row start with a blank "last letter"
		char lastLetter = NULL;

		for (int j = 0; j < grid[i].size(); j++)
		{
			//Get cur letter and coord
			char curLetter = grid[i][j];
			vector<int> curCoords = { i, j };

			if (!coordsInEstablishedRegion(allRegions, curCoords) && curLetter != lastLetter)
			{
				createNewRegion(grid, allRegions, curCoords, regionNum, curLetter);
				regionNum++;
				lastLetter = curLetter;
			}
			else
			{
				lastLetter = curLetter;
			}

		}
	}
}

bool coordsInEstablishedRegion(const vector<Region>& allRegions, const vector<int>& coords)
{
	for (int i = 0; i < allRegions.size(); i++)
	{
		if (allRegions[i].coordInRegion(coords))
		{
			return true;
		}
	}

	return false;
}

void createNewRegion(const vector<vector<char>>& grid, vector<Region>& allRegions, vector<int> & startCoords, int regionNum,
	char regionLetter)
{
	Region createdRegion(regionNum);
	createdRegion.insertCoords(startCoords);

	vector<int> xDir = { 1, 0, -1 , 0 };
	vector<int> yDir = { 0, 1, 0, -1 };

	int numEdges = 0;
	int numNeighbors = 0;
	bool vertical = false;
	bool horizontal = false;
	int up = 1;
	int right = -1;
	int left = 1;
	int down = -1;

	//Check right, down, left, and up of the letter
	for (int i = 0; i < xDir.size(); i++)
	{
		vector<int> nextCoords = { startCoords[0] + yDir[i], startCoords[1] + xDir[i] };

		//Check coords are in bounds 
		if ((nextCoords[0] >= 0 && nextCoords[0] < grid.size())
			&& (nextCoords[1] >= 0 && nextCoords[1] < grid[0].size()))
		{
			if (grid[nextCoords[0]][nextCoords[1]] != regionLetter)
			{
				//If it's not the letter, add a edge of perimeter
				numEdges++;
			}
			else if (grid[nextCoords[0]][nextCoords[1]] == regionLetter &&
				!createdRegion.coordInRegion(nextCoords))
			{
				//If it is the same letter, move to that letter and do calculations
				addLetterToRegion(grid, createdRegion, nextCoords, regionLetter);
			}
			
			if (grid[nextCoords[0]][nextCoords[1]] == regionLetter)
			{
				if (i == 0 || i == 2)
				{
					horizontal = true;

					if (i == 0)
					{
						right = 1;
					}
					else
					{
						left = -1;
					}
				}

				if (i == 1 || i == 3)
				{
					vertical = true;

					if (i == 3)
					{
						up = -1;
					}
					else
					{
						down = 1;
					}
				}

				numNeighbors++;
			}
		}
		else
		{
			numEdges++;
		}
	}

	//After checking each direction, add edges to the region num
	createdRegion.addEdges(numEdges);

	//Count the corners
	int numDiags = 0;
	xDir = { -1, 1, 1, -1 };
	yDir = { -1, -1, 1, 1 };

	for (int i = 0; i < xDir.size(); i++)
	{
		vector<int> diagCoords = { startCoords[0] + yDir[i], startCoords[1] + xDir[i] };

		//Check coords are in bounds 
		if ((diagCoords[0] >= 0 && diagCoords[0] < grid.size())
			&& (diagCoords[1] >= 0 && diagCoords[1] < grid[0].size()))
		{
			if (grid[diagCoords[0]][diagCoords[1]] == regionLetter)
			{
				numDiags++;
			}
		}
	}

	int numCorners = 0;
	if (numNeighbors == 4)
	{
		numCorners = 4 - numDiags;
	}
	else if (numNeighbors == 3)
	{
		int subtractionNum = calcSubtractNum(grid, startCoords, regionLetter, up, down, right, left);

		numCorners = 2 - subtractionNum;
	}
	else if (numNeighbors == 2)
	{
		if (vertical && horizontal && numDiags == 0)
		{
			numCorners = 2;
		}
		else if (vertical && horizontal && numDiags > 0)
		{
			vector<int> checkDiag = { startCoords[0] + up, startCoords[1] + right };

			if ((checkDiag[0] >= 0 && checkDiag[0] < grid.size())
				&& (checkDiag[1] >= 0 && checkDiag[1] < grid[0].size()))
			{
				if (grid[checkDiag[0]][checkDiag[1]] == regionLetter)
				{
					numCorners = 1;
				}
				else
				{
					numCorners = 2;
				}
			}
			else
			{
				numCorners = 2;
			}
		}
		else if ((vertical && !horizontal) || (!vertical && horizontal))
		{
			numCorners = 0;
		}
	}
	else if (numNeighbors == 1)
	{
		numCorners = 2;
	}
	else if (numNeighbors == 0)
	{
		numCorners = 4;
	}

	if (numCorners < 0)
	{
		numCorners = 0;
	}

	cout << "Coords: " << startCoords[0] << ", " << startCoords[1] << " Corners: " << numCorners << endl;
	createdRegion.addCorners(numCorners);
	cout << "^ Region " << regionNum << endl;
	cout << "-------------------" << endl;

	//Put the region in the regionList
	allRegions.push_back(createdRegion);
}

void addLetterToRegion(const vector<vector<char>>& grid, Region& curRegion, vector<int> & coords, char regionLetter)
{
	curRegion.insertCoords(coords);

	vector<int> xDir = { 1, 0, -1 , 0 };
	vector<int> yDir = { 0, 1, 0, -1 };

	int numEdges = 0;
	int numNeighbors = 0;
	bool vertical = false;
	bool horizontal = false;
	int up = 1;
	int right = -1;
	int left = 1;
	int down = -1;

	//Check right, down, left, and up of the letter
	for (int i = 0; i < xDir.size(); i++)
	{
		vector<int> nextCoords = { coords[0] + yDir[i], coords[1] + xDir[i] };

		//Check coords are in bounds 
		if ((nextCoords[0] >= 0 && nextCoords[0] < grid.size())
			&& (nextCoords[1] >= 0 && nextCoords[1] < grid[0].size()))
		{
			if (grid[nextCoords[0]][nextCoords[1]] != regionLetter)
			{
				//If it's not the letter, add a edge of perimeter
				numEdges++;
			}
			else if (grid[nextCoords[0]][nextCoords[1]] == regionLetter &&
				!curRegion.coordInRegion(nextCoords))
			{
				//If it is the same letter, move to that letter and do calculations
				addLetterToRegion(grid, curRegion, nextCoords, regionLetter);
			}

			if (grid[nextCoords[0]][nextCoords[1]] == regionLetter)
			{
				if (i == 0 || i == 2)
				{
					horizontal = true;

					if (i == 0)
					{
						right = 1;
					}
					else
					{
						left = -1;
					}
				}

				if (i == 1 || i == 3)
				{
					vertical = true;

					if (i == 3)
					{
						up = -1;
					}
					else
					{
						down = 1;
					}
				}

				numNeighbors++;
			}
		}
		else
		{
			numEdges++;
		}
	}

	//After checking each direction, add edges to the region num
	curRegion.addEdges(numEdges);

	//Count the corners
	int numDiags = 0;
	xDir = {-1, 1, 1, -1};
	yDir = {-1, -1, 1, 1};

	for (int i = 0; i < xDir.size(); i++)
	{
		vector<int> diagCoords = { coords[0] + yDir[i], coords[1] + xDir[i] };

		//Check coords are in bounds 
		if ((diagCoords[0] >= 0 && diagCoords[0] < grid.size())
			&& (diagCoords[1] >= 0 && diagCoords[1] < grid[0].size()))
		{
			if (grid[diagCoords[0]][diagCoords[1]] == regionLetter)
			{
				numDiags++;
			}
		}
	}

	int numCorners = 0;

	if (numNeighbors == 4)
	{
		numCorners = 4 - numDiags;
	}
	else if (numNeighbors == 3)
	{
		int subtractionNum = calcSubtractNum(grid, coords, regionLetter, up, down, right, left);

		numCorners = 2 - subtractionNum;
	} 
	else if (numNeighbors == 2)
	{
		if (vertical && horizontal && numDiags == 0)
		{
			numCorners = 2;
		}
		else if (vertical && horizontal && numDiags > 0)
		{
			vector<int> checkDiag = { coords[0] + up, coords[1] + right };

			if ((checkDiag[0] >= 0 && checkDiag[0] < grid.size())
				&& (checkDiag[1] >= 0 && checkDiag[1] < grid[0].size()))
			{
				if (grid[checkDiag[0]][checkDiag[1]] == regionLetter)
				{
					numCorners = 1;
				}
				else
				{
					numCorners = 2;
				}
			}
			else
			{
				numCorners = 2;
			}
		}
		else if ((vertical && !horizontal) || (!vertical && horizontal))
		{
			numCorners = 0;
		}
	}
	else if (numNeighbors == 1)
	{
		numCorners = 2;
	}

	if (numCorners < 0)
	{
		numCorners = 0;
	}

	cout << "Coords: " << coords[0] << ", " << coords[1] << " Corners: " << numCorners << endl;
	curRegion.addCorners(numCorners);
}

int calcSubtractNum(const vector<vector<char>>& grid, const vector<int>& coords, char regionLetter, int & up, int & down, int & right, int & left)
{
	int subtractionNum = 0;

	if (up == -1 && down == 1)
	{
		int firstCheckRow = coords[0] + 1;
		int firstCheckCol = coords[1] + right;
		int secondCheckRow = coords[0] - 1;
		int secondCheckCol = coords[1] + right;

		if ((firstCheckRow >= 0 && firstCheckRow < grid.size())
			&& (firstCheckCol >= 0 && firstCheckCol < grid[0].size()))
		{
			if (grid[firstCheckRow][firstCheckCol] == regionLetter)
			{
				subtractionNum++;
			}
		}

		if ((secondCheckRow >= 0 && secondCheckRow < grid.size())
			&& (secondCheckCol >= 0 && secondCheckCol < grid[0].size()))
		{
			if (grid[secondCheckRow][secondCheckCol] == regionLetter)
			{
				subtractionNum++;
			}
		}
	}
	else
	{

		int firstCheckRow = coords[0] + up;
		int firstCheckCol = coords[1] + 1;
		int secondCheckRow = coords[0] + up;
		int secondCheckCol = coords[1] - 1;

		if ((firstCheckRow >= 0 && firstCheckRow < grid.size())
			&& (firstCheckCol >= 0 && firstCheckCol < grid[0].size()))
		{
			if (grid[firstCheckRow][firstCheckCol] == regionLetter)
			{
				subtractionNum++;
			}
		}

		if ((secondCheckRow >= 0 && secondCheckRow < grid.size())
			&& (secondCheckCol >= 0 && secondCheckCol < grid[0].size()))
		{
			if (grid[secondCheckRow][secondCheckCol] == regionLetter)
			{
				subtractionNum++;
			}
		}
	}

	return subtractionNum;
}
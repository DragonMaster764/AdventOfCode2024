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
	int edges;
public:
	Region()
	{
		regionNum = 0;
		regionCoords;
		edges = 0;
	}
	Region(int areaNum)
	{
		regionNum = areaNum;
		regionCoords;
		edges = 0;
	}
	void insertCoords(vector<int> coords)
	{
		regionCoords.push_back(coords);
	}
	void addEdges(int count)
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
	long long calcPrice() const
	{
		//Testing
		/*cout << regionNum << endl;
		for (int i = 0; i < regionCoords.size(); i++)
		{
			cout << regionCoords[i][0] << ", " << regionCoords[i][1] << endl;
		}*/
		return edges * regionCoords.size();
	}
};

// Methods
void regionFinder(const vector<vector<char>>& grid, vector<Region>& allRegions);
bool coordsInEstablishedRegion(const vector<Region>& allRegions, const vector<int>& coords);
void createNewRegion(const vector<vector<char>>& grid, vector<Region>& allRegions, vector<int> startCoords, int regionNum,
	char regionLetter);
void addLetterToRegion(const vector<vector<char>>& grid, Region& curRegion, vector<int> coords, char regionLetter);

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

void regionFinder(const vector<vector<char>> & grid, vector<Region> & allRegions)
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

bool coordsInEstablishedRegion(const vector<Region> & allRegions, const vector<int> & coords)
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

void createNewRegion(const vector<vector<char>>& grid, vector<Region>& allRegions, vector<int> startCoords, int regionNum,
	char regionLetter)
{
	Region createdRegion(regionNum);
	createdRegion.insertCoords(startCoords);

	vector<int> xDir = { 1, 0, -1 , 0 };
	vector<int> yDir = { 0, 1, 0, -1 };

	int numEdges = 0;

	//Check right, down, left, and up of the letter
	for (int i = 0; i < xDir.size(); i++)
	{
		vector<int> nextCoords = { startCoords[0] + yDir[i], startCoords[1] + xDir[i]};

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
		}
		else
		{
			numEdges++;
		}
	}

	//After checking each direction, add edges to the region num
	createdRegion.addEdges(numEdges);

	//Put the region in the regionList
	allRegions.push_back(createdRegion);
}

void addLetterToRegion(const vector<vector<char>>& grid, Region& curRegion, vector<int> coords, char regionLetter)
{
	curRegion.insertCoords(coords);

	vector<int> xDir = { 1, 0, -1 , 0 };
	vector<int> yDir = { 0, 1, 0, -1 };

	int numEdges = 0;

	//Check left, up, right, and down of the letter
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
		}
		else
		{
			numEdges++;
		}
	}

	//After checking each direction, add edges to the region num
	curRegion.addEdges(numEdges);
}
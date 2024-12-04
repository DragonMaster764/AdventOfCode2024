#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Methods
void printGrid(vector<vector<char>> g);
int wordSearch(vector<vector<char>> grid, string word);
int coordSearch(vector<vector<char>> grid, int x, int y, string restWord);
bool outOfBounds(vector<vector<char>> grid, int x, int y);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data and make grid
	vector<vector<char>> grid;
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

	//Testing
	printGrid(grid);

	//Search for the word
	string word = "XMAS";
	int total = wordSearch(grid, word);

	cout << "Total: " << total << endl;

	return 0;
}

//For Testing
void printGrid(vector<vector<char>> g)
{
	for (int i = 0; i < g.size(); i++)
	{
		for (int j = 0; j < g[i].size(); j++)
		{
			cout << g[i][j] << " ";
		}
		cout << endl;
	}
}

int wordSearch(vector<vector<char>> grid, string word)
{

	char firstChar = word[0];
	word = word.substr(1, word.size());
	
	int total = 0;
	
	//Goes through every coordinate in the grid
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{

			//Only search directions if the first character is the firstChar of word
			if (grid[i][j] == firstChar)
			{
				total += coordSearch(grid, j, i, word);

			}
			
		}
	}

	return total;
}

int coordSearch(vector<vector<char>> grid, int x, int y, string restWord)
{
	vector<int> xDir{ -1, -1, -1, 0, 0, 1, 1, 1 };
	vector<int> yDir{ -1, 0, 1, -1, 1, 1, 0, -1 };

	int total = 0;

	//Check one in each direction if first letter is x
	for (int dir = 0; dir < xDir.size(); dir++)
	{
		// Find the new direction to check 
		vector<int> nextCoord{x + xDir[dir], y + yDir[dir]};

		//Make sure it's not out of bounds
		if (!(outOfBounds(grid, nextCoord[0], nextCoord[1])))
		{
			//Checks to see if it's the next letter
			if (grid[nextCoord[1]][nextCoord[0]] == restWord[0])
			{
				//Check in same direction until the end of the word (or out of 
				//bounds)
				bool isWord = true;
				vector<int> last = nextCoord;
				for (int i = 1; i < restWord.size(); i++)
				{
					vector<int> nextCheckCoord{ last[0] + xDir[dir], last[1] + yDir[dir]};

					if (!(outOfBounds(grid, nextCheckCoord[0], nextCheckCoord[1])))
					{
						if (!(grid[nextCheckCoord[1]][nextCheckCoord[0]] == restWord[i]))
						{
							isWord = false;
							break;
						}
					}
					else
					{
						isWord = false;
						break;
					}

					last = nextCheckCoord;
				}

				if (isWord)
				{
					total++;
				}
			}
		}
	}

	//Return total after checking each direction
	return total;
}

bool outOfBounds(vector<vector<char>> grid, int x, int y)
{
	//Checks to see if the coordinate is outside the grid
	if (x < 0 || x > grid[0].size() - 1 || y < 0 || y > grid.size() - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
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
	//printGrid(grid);

	//Search for the word
	string word = "MAS";
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

	char firstChar = word[1];

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
	vector<int> xDir{ -1, 1, -1, 1 };
	vector<int> yDir{ -1, 1, 1, -1 };

	vector<char> letters = { restWord[0], restWord[2]};

	int total = 0;

	//Check one in each direction 
	for (int dir = 0; dir < xDir.size(); dir++)
	{
		// Find the new direction to check 
		vector<int> nextCoord{ x + xDir[dir], y + yDir[dir] };

		//Make sure it's not out of bounds
		if (!(outOfBounds(grid, nextCoord[0], nextCoord[1])))
		{
			char c = grid[nextCoord[1]][nextCoord[0]];
			bool hasLetter = false;
			for (int i = 0; i < letters.size(); i++)
			{
				if (letters[i] == c)
				{
					hasLetter = true;
					letters.erase(letters.begin() + i);
				}
			}

			if (!hasLetter)
			{
				return 0;
			}
		}
		else 
		{
			return 0;
		}

		//If we are on the next diagonal, replace the letters
		if (dir == 1)
		{
			letters = { restWord[0], restWord[2] };
		}
	}

	//Return total after checking each direction
	return 1;
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

//Flips the direction to see the other diagonal
int flip(int x)
{
	if (x == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
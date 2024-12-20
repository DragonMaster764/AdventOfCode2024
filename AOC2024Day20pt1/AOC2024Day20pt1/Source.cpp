#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Methods
void readNumbers(vector<int>& list, string nums);
bool inBounds(const vector<int>& coords, const vector<vector<char>>& track);
void benchmarkToFinish(const vector<vector<char>>& track, const vector<int>& startCoords, const vector<int>& endCoords,
	unordered_map<string, long long>& coordTime);
void findCheats(const vector<vector<char>>& track, const vector<int>& startCoords, const vector<int>& endCoords,
	unordered_map<string, long long>& coordTime, long long& numberOfCheats);
bool hasVisited(const vector<int>& coords, const vector<vector<int>> visited);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	//Make the map of the racetrack
	vector<vector<char>> racetrack;

	//Get start and end coords
	vector<int> startCoords;
	vector<int> endCoords;

	// Parse the data
	string input;
	int rowNum = 0;
	while (getline(fileRead, input))
	{
		vector<char> row;

		for (int i = 0; i < input.size(); i++)
		{
			row.push_back(input[i]);

			if (input[i] == 'S')
			{
				startCoords = { rowNum, i };
			}
			else if (input[i] == 'E')
			{
				endCoords = { rowNum, i };
			}
		}

		racetrack.push_back(row);
		rowNum++;
	}

	//Benchmark finding the length (time) to the end
	unordered_map<string, long long> coordTime;

	//Start is time '0'
	string coordsToInsert = to_string(startCoords[0]) + "," + to_string(startCoords[1]);
	coordTime[coordsToInsert] = 0;
	benchmarkToFinish(racetrack, startCoords, endCoords, coordTime);

	cout << "Time to reach finish line: " << coordTime[to_string(endCoords[0]) + "," + to_string(endCoords[1])] 
		<< " picoseconds" << endl;

	long long numberOfCheats = 0;
	findCheats(racetrack, startCoords, endCoords, coordTime, numberOfCheats);

	cout << "Number cheats that save at least 100 picoseconds: " << numberOfCheats << endl;

	return 0;
}

void findCheats(const vector<vector<char>>& track, const vector<int>& startCoords, const vector<int>& endCoords,
	unordered_map<string, long long>& coordTime, long long & numberOfCheats)
{

	//visited vector
	vector<vector<int>> visited;

	//Intial Position
	vector<int> curCoords = startCoords;

	//Direction vectors
	vector<int> xDir = { 1, 0, -1, 0 };
	vector<int> yDir = { 0, -1, 0, 1 };
	int numDirections = xDir.size();

	long long spotsChecked = 0;

	//Unless we are at the end, check each direction to see if we can save time
	while (curCoords != endCoords)
	{
		vector<int> nextCoordToVisit; //Save the next free space to move to after this one

		for (int curDir = 0; curDir < numDirections; curDir++)
		{
			vector<int> posCoords = { curCoords[0] + yDir[curDir], curCoords[1] + xDir[curDir]};

			if (inBounds(posCoords, track))
			{
				//If it's a wall, see if there is an empty space 
				if (track[posCoords[0]][posCoords[1]] == '#')
				{
					vector<int> phaseEnding = { posCoords[0] + yDir[curDir], posCoords[1] + xDir[curDir] };

					if (inBounds(phaseEnding, track))
					{
						if (track[phaseEnding[0]][phaseEnding[1]] == '.' || track[phaseEnding[0]][phaseEnding[1]] == 'E')
						{
							/*cout << "Calcing time " << to_string(curCoords[0]) + ',' + to_string(curCoords[1]) << " " 
								<< to_string(phaseEnding[0]) + ',' + to_string(phaseEnding[1]) << endl;*/
							//How much time do we save?
							long long timeFinal = coordTime[to_string(phaseEnding[0]) + ',' + to_string(phaseEnding[1])];
							long long timeInital = coordTime[to_string(curCoords[0]) + ',' + to_string(curCoords[1])];

							long long savedTime = timeFinal - timeInital - 2;
							/*cout << "Done calcing time" << endl;*/

							if (savedTime >= 0)
							{
								/*cout << "Found a cheat that saves " << savedTime << " picoseconds! :)" << endl;*/
								if (savedTime >= 100)
								{
									//Increase number of cheats
									numberOfCheats++;
								}
							}
						}
					}
					
				}
				else if ((track[posCoords[0]][posCoords[1]] == '.' || track[posCoords[0]][posCoords[1]] == 'E') && !hasVisited(posCoords, visited))
				{
					//Next free space to check for cheats
					nextCoordToVisit = posCoords;
				}
			}

		}

		//Set visited to curCoords
		visited.push_back(curCoords);

		spotsChecked++;
		cout << "Spots Checked: " << spotsChecked << endl;
		/*cout << "Next Coord to visit: " << to_string(nextCoordToVisit[0]) + ',' + to_string(nextCoordToVisit[1]) << endl;*/

		//Next Coords to visit
		curCoords = nextCoordToVisit;

	}

}

bool hasVisited(const vector<int>& coords, const vector<vector<int>> visited)
{
	for (vector<int> visitedCoord : visited)
	{
		if (coords == visitedCoord)
		{
			return true;
		}
	}

	return false;
}

void benchmarkToFinish(const vector<vector<char>>& track, const vector<int>& startCoords, const vector<int>& endCoords,
	unordered_map<string, long long>& coordTime)
{
	//Starting values
	vector<int> curCoords = startCoords;
	long long picoTime = 0;

	//Direction vectors
	vector<int> xDir = { 1, 0, -1, 0 };
	vector<int> yDir = { 0, -1, 0, 1 };
	int numDirections = xDir.size();

	while (curCoords != endCoords)
	{
		for (int curDir = 0; curDir < numDirections; curDir++)
		{
			vector<int> posCoords = { curCoords[0] + yDir[curDir], curCoords[1] + xDir[curDir] };
			string keyToFind = to_string(posCoords[0]) + "," + to_string(posCoords[1]);
			//Check it's in bounds of the track
			if (inBounds(posCoords, track))
			{
				//Check if it's a '.' or 'E' and we HAVEN'T been there
				if ((track[posCoords[0]][posCoords[1]] == '.' || track[posCoords[0]][posCoords[1]] == 'E')
					&& coordTime.find(keyToFind) == coordTime.end())
				{
					//Increase picotime
					picoTime++;

					//Add the coords and time to coordTime hashmap
					coordTime[keyToFind] = picoTime;

					//Move the curCoords to posCoords and break
					curCoords = posCoords;
					break;
				}
			}
		}

	}
}

bool inBounds(const vector<int>& coords, const vector<vector<char>>& track)
{
	if ((coords[0] >= 0 && coords[0] < track.size()) && (coords[1] >= 0 && coords[1] < track[0].size()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void readNumbers(vector<int>& list, string nums)
{
	string numString = "";
	for (int i = 0; i < nums.size(); i++)
	{
		char c = nums[i];
		if (48 <= c && c <= 57)
		{
			numString += nums[i];
		}
		else if (numString != "" && c == ' ')
		{
			list.push_back(stoi(numString));
			numString = "";
		}
	}

	if (numString != "")
	{
		list.push_back(stoi(numString));
	}
}

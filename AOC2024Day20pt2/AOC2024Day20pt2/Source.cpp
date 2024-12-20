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
long long manhattanDistance(long long x1, long long x2, long long y1, long long y2);
void searchRadius(const vector<vector<char>>& track, const vector<int> curCoords, int radius,
	unordered_map<string, long long>& coordTime, vector<long long>& cheatDistances);

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
	unordered_map<string, long long>& coordTime, long long& numberOfCheats)
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
			vector<int> posCoords = { curCoords[0] + yDir[curDir], curCoords[1] + xDir[curDir] };

				if ((track[posCoords[0]][posCoords[1]] == '.' || track[posCoords[0]][posCoords[1]] == 'E') && !hasVisited(posCoords, visited))
				{
					//Next free space to check for cheats
					nextCoordToVisit = posCoords;
				}

		}

		//Get all cheats that lead to a non wall item in set radius
		vector<long long> cheatDistances;
		int radius = 20;
		searchRadius(track, curCoords, radius, coordTime, cheatDistances);

		for (long long timeSaved : cheatDistances)
		{
			if (timeSaved >= 100)
			{
				numberOfCheats++;
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

long long manhattanDistance(long long x1, long long x2, long long y1, long long y2)
{
	return abs(x2 - x1) + abs(y2 - y1);
}

void searchRadius(const vector<vector<char>>& track, const vector<int> curCoords, int radius,
	unordered_map<string, long long>& coordTime, vector<long long>& cheatDistances)
{
	long long posRow = curCoords[0];
	long long posCol = curCoords[1];

	long long minCol = max((long long) 0, posCol - radius);
	long long maxCol = min((long long)track[0].size() - 1, posCol + radius);
	long long minRow = max((long long)0, posRow - radius);
	long long maxRow = min((long long)track.size() - 1, posRow + radius);

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minCol; j <= maxCol; j++)
		{
			long long manhatDistance = manhattanDistance(posRow, i, posCol, j);
			if (manhatDistance <= radius && manhatDistance != 0)
			{
				//Find the coords in this radius
				vector<int> coordsInRadius = { i, j };
				
				//See if it's a shortcut
				if (track[coordsInRadius[0]][coordsInRadius[1]] == '.' || track[coordsInRadius[0]][coordsInRadius[1]] == 'E')
				{
					/*cout << "Calcing time " << to_string(curCoords[0]) + ',' + to_string(curCoords[1]) << " "
						<< to_string(phaseEnding[0]) + ',' + to_string(phaseEnding[1]) << endl;*/
						//How much time do we save?
					long long timeFinal = coordTime[to_string(coordsInRadius[0]) + ',' + to_string(coordsInRadius[1])];
					long long timeInital = coordTime[to_string(curCoords[0]) + ',' + to_string(curCoords[1])];

					long long savedTime = timeFinal - timeInital - manhatDistance;
					/*cout << "Done calcing time" << endl;*/

					if (savedTime >= 0)
					{
						//Add to cheat distances
						cheatDistances.push_back(savedTime);
					}
				}
			}
		}
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

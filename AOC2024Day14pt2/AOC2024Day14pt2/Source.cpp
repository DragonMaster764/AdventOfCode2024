#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

//Robot Class
class Robot
{
private:
	int intPosX;
	int intPosY;
	int curPosX;
	int curPosY;
	int velocityX;
	int velocityY;
public:
	Robot()
	{
		intPosX = 0;
		intPosY = 0;
		curPosX = intPosX;
		curPosY = intPosY;
		velocityX = 0;
		velocityY = 0;
	}
	Robot(int posX, int posY, int intVelX, int intVelY)
	{
		intPosX = posX;
		intPosY = posY;
		velocityX = intVelX;
		velocityY = intVelY;
		curPosX = posX;
		curPosY = posY;
	}
	void moveTiles(int sideLimit, int verticalLimit)
	{
		curPosX = curPosX + velocityX;
		curPosY = curPosY + velocityY;

		if (curPosX >= sideLimit)
		{
			curPosX = curPosX - sideLimit;
		}
		else if (curPosX < 0)
		{
			curPosX = sideLimit + curPosX;
		}

		if (curPosY >= verticalLimit)
		{
			curPosY = curPosY - verticalLimit;
		}
		else if (curPosY < 0)
		{
			curPosY = verticalLimit + curPosY;
		}

	}
	int getCurPosX() const
	{
		return curPosX;
	}
	int getCurPosY() const
	{
		return curPosY;
	}
	bool inSameSpotAsIntial() const
	{
		if (curPosX == intPosX && curPosY == intPosY)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void resetRobot()
	{
		curPosX = intPosX;
		curPosY = intPosY;
	}
};

// Methods
vector<vector<int>> getRobotGrid(const vector<Robot>& robots, int xSize, int ySize);
void printRobotGrid(const vector<vector<int>>& numberGrid);
long long calcSafetyFactor(const vector<vector<int>>& numGrid);
void placeIntoQuadrant(int middleX, int middleY, int numRobots, int row, int col,
	int& tLeft, int& tRight, int& bLeft, int& bRight);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	//Robot list
	vector<Robot> robots;

	// Parse the data
	string input;
	while (getline(fileRead, input))
	{
		int robotX = stoi(input.substr(input.find("p=") + 2, input.find_first_of(',')));
		int robotY = stoi(input.substr(input.find_first_of(',') + 1, input.find_first_of(' ')));
		int robotVelX = stoi(input.substr(input.find("v=") + 2, input.find_last_of(',')));
		int robotVelY = stoi(input.substr(input.find_last_of(',') + 1));

		//Testing
		/*cout << robotX << " " << robotY << " " << robotVelX << " " << robotVelY << endl;*/

		robots.push_back(Robot(robotX, robotY, robotVelX, robotVelY));
	}

	int gridXSize = 101;
	int gridYSize = 103;
	int numberOfSeconds = 20000;
	int timeRobotsBackInIntialSpot = 0;

	for (int i = 0; i < numberOfSeconds; i++)
	{
		bool robotsInStart = true;

		//Move every robot
		for (int j = 0; j < robots.size(); j++)
		{
			robots[j].moveTiles(gridXSize, gridYSize);
			
			if (!robots[j].inSameSpotAsIntial())
			{
				robotsInStart = false;
			}
		}

		if (robotsInStart)
		{
			timeRobotsBackInIntialSpot = i + 1;
			break;
		}

	}

	//Reset Robots
	for (int i = 0; i < robots.size(); i++)
	{
		robots[i].resetRobot();
	}

	long long minSafety = LLONG_MAX;
	int seconds = 0;

	for (int i = 0; i < timeRobotsBackInIntialSpot; i++)
	{
		for (int j = 0; j < robots.size(); j++)
		{
			robots[j].moveTiles(gridXSize, gridYSize);
		}

		vector<vector<int>> numGrid = getRobotGrid(robots, gridXSize, gridYSize);
		long long curSafetyFactor = calcSafetyFactor(numGrid);

		if (curSafetyFactor < minSafety)
		{
			minSafety = curSafetyFactor;
			seconds = i + 1;
		}
	}

	//Reset Robots
	for (int i = 0; i < robots.size(); i++)
	{
		robots[i].resetRobot();
	}

	for (int i = 0; i < seconds; i++)
	{
		for (int j = 0; j < robots.size(); j++)
		{
			robots[j].moveTiles(gridXSize, gridYSize);
		}
	}

	
	//Print the grid
	cout << seconds << " seconds have passed: " << endl;
	vector<vector<int>> numGrid = getRobotGrid(robots, gridXSize, gridYSize);
	printRobotGrid(numGrid);
	cout << calcSafetyFactor(numGrid) << endl;
	cout << "---------------------------------------" << endl << endl;

	return 0;
}

vector<vector<int>> getRobotGrid(const vector<Robot>& robots, int xSize, int ySize)
{
	//Make one a grid with the num of robots
	vector<vector<int>> numberGrid;

	for (int i = 0; i < ySize; i++)
	{
		vector<int> row;

		for (int j = 0; j < xSize; j++)
		{
			row.push_back(0);
		}

		numberGrid.push_back(row);
	}

	//Add one to each space where a robot is
	for (int i = 0; i < robots.size(); i++)
	{
		int robX = robots[i].getCurPosX();
		int robY = robots[i].getCurPosY();

		numberGrid[robY][robX] += 1;
	}

	return numberGrid;
}

void printRobotGrid(const vector<vector<int>>& numberGrid)
{
	//Print a grid with 0's as '.' and numbers as numbers
	for (int i = 0; i < numberGrid.size(); i++)
	{
		for (int j = 0; j < numberGrid[i].size(); j++)
		{
			if (numberGrid[i][j] == 0)
			{
				cout << '.';
			}
			else
			{
				cout << numberGrid[i][j];
			}
		}
		cout << endl;
	}
}

long long calcSafetyFactor(const vector<vector<int>>& numGrid)
{
	long long totalRobots = 0;
	int middleX = numGrid[0].size() / 2;
	int middleY = numGrid.size() / 2;

	//Quadrants
	int topLeft = 0;
	int topRight = 0;
	int bottomLeft = 0;
	int bottomRight = 0;

	//Traverse grid, and only count ones in the quadrants
	for (int i = 0; i < numGrid.size(); i++)
	{
		for (int j = 0; j < numGrid[i].size(); j++)
		{
			if (i != middleY && j != middleX && numGrid[i][j] != 0)
			{
				placeIntoQuadrant(middleX, middleY, numGrid[i][j], i, j, topLeft, topRight, bottomLeft, bottomRight);
			}
		}
	}

	totalRobots = topLeft * topRight * bottomLeft * bottomRight;

	return totalRobots;
}

//Method places the number of robots into the right quadrant counter
void placeIntoQuadrant(int middleX, int middleY, int numRobots, int row, int col,
	int& tLeft, int& tRight, int& bLeft, int& bRight)
{
	if (row < middleY && col < middleX)
	{
		tLeft += numRobots;
	}
	else if (row < middleY && col > middleX)
	{
		tRight += numRobots;
	}
	else if (row > middleY && col < middleX)
	{
		bLeft += numRobots;
	}
	else if (row > middleY && col > middleX)
	{
		bRight += numRobots;
	}
	else
	{
		cout << "Error" << endl;
	}
}
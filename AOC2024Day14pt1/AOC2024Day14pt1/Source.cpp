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
	int posX;
	int posY;
	int velocityX;
	int velocityY;
public:
	Robot()
	{
		posX = 0;
		posY = 0;
		velocityX = 0;
		velocityY = 0;
	}
	Robot(int intPosX, int intPosY, int intVelX, int intVelY)
	{
		posX = intPosX;
		posY = intPosY;
		velocityX = intVelX;
		velocityY = intVelY;
	}
	void moveTiles(int sideLimit, int verticalLimit)
	{
		int newPosX = posX + velocityX;
		int newPosY = posY + velocityY;

		if (newPosX >= sideLimit)
		{
			newPosX = newPosX - sideLimit;
		}
		else if (newPosX < 0)
		{
			newPosX = sideLimit + newPosX;
		}

		if (newPosY >= verticalLimit)
		{
			newPosY = newPosY - verticalLimit;
		}
		else if (newPosY < 0)
		{
			newPosY = verticalLimit + newPosY;
		}

		//Change the robot's actual variables
		posX = newPosX;
		posY = newPosY;
	}
	int getPosX() const
	{
		return posX;
	}
	int getPosY() const
	{
		return posY;
	}
};

// Methods
vector<vector<int>> printRobotGrid(const vector<Robot>& robots, int xSize, int ySize);
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
	int numberOfSeconds = 100;

	for (int i = 0; i < numberOfSeconds; i++)
	{
		//Move every robot
		for (int j = 0; j < robots.size(); j++)
		{
			robots[j].moveTiles(gridXSize, gridYSize);
		}
	}

	//Print the grid
	vector<vector<int>> numberGrid = printRobotGrid(robots, gridXSize, gridYSize);

	//Find the safety factor
	int safetyFactor = calcSafetyFactor(numberGrid);

	cout << "Safety Factor: " << safetyFactor << endl;

	return 0;
}

vector<vector<int>> printRobotGrid(const vector<Robot>& robots, int xSize, int ySize)
{
	//Make one with numbers, then one in char form for easy reading ability
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
		int robX = robots[i].getPosX();
		int robY = robots[i].getPosY();

		numberGrid[robY][robX] += 1;
	}

	//Now print a grid with 0's as '.' and numbers as numbers
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

	return numberGrid;
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
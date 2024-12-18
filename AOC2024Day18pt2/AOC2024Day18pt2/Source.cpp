#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

//Node class
class Node
{
private:
	vector<int> coords;
	long long score;
	int direction;
public:
	Node()
	{
		coords = { -1, -1 };
		score = LLONG_MAX;
		direction = -1;
	}
	Node(vector<int> curCoords, long long curScore, int curDirection)
	{
		coords = curCoords;
		score = curScore;
		direction = curDirection;
	}
	Node(vector<int> curCoords) : Node(curCoords, LLONG_MAX, -1) {};
	Node(vector<int> curCoords, int curScore) : Node(curCoords, curScore, -1) {};

	//Getters and setters
	vector<int> getCoords() const
	{
		return coords;
	}
	long long getScore() const
	{
		return score;
	}
	int getDirection() const
	{
		return direction;
	}
	void setScore(int newScore)
	{
		score = newScore;
	}
	void setDirection(int curDirection)
	{
		direction = curDirection;
	}

	//Friends
	friend class compareNodeScores;
};

class compareNodeScores
{
public:
	bool operator()(const Node& lNode, const Node& rNode) const
	{
		return lNode.score > rNode.score; // Min-heap
	}
};

// Methods
bool inBounds(const vector<vector<Node>>& map, const vector<int>& coords);
bool hasVisited(const vector<int>& coords, const vector<vector<int>>& visited);
void updateNeighbors(vector<vector<Node>>& maze, priority_queue<Node, vector<Node>, compareNodeScores>& queue, Node& curNode);
void readNumbers(vector<int>& list, string nums);
long long dijsktra(const vector<vector<Node>>& map, vector<int>& startPosition, vector<int>& endPosition);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	//Get all corrupted coords
	vector<vector<int>> corruptedCoords;
	string input;

	while (getline(fileRead, input))
	{
		vector<int> xyCorrupt;
		readNumbers(xyCorrupt, input);
		//Switch to row column (yx) instead for easier array travel
		vector<int> yxCorrupt = { xyCorrupt[1], xyCorrupt[0] };
		corruptedCoords.push_back(yxCorrupt);
	}

	// Maze
	vector<vector<Node>> maze;

	//Maze Size
	int widthAndHeight = 71;

	//Historian start / end position
	vector<int> startPosition = { 0, 0 };
	vector<int> endPosition = { widthAndHeight - 1, widthAndHeight - 1 };

	// Make a map where every available space starts with "infinite" score
	// Walls are -1 to be ignored later
	// Start gets a 0 score
	for (int i = 0; i < widthAndHeight; i++)
	{
		vector<Node> row;

		for (int j = 0; j < widthAndHeight; j++)
		{
			vector<int> curPosition = { i, j };

			if (curPosition == startPosition)
			{
				row.push_back(Node(curPosition, 0));
			}
			else
			{
				row.push_back(Node(curPosition));
			}
		}

		maze.push_back(row);
	}

	//Corrupt some of the places
	for (int i = 0; i < 1024; i++)
	{
		vector<int> curCorrupted = corruptedCoords[i];
		maze[curCorrupted[0]].erase(maze[curCorrupted[0]].begin() + curCorrupted[1]);
		maze[curCorrupted[0]].insert(maze[curCorrupted[0]].begin() + curCorrupted[1], Node(curCorrupted, -1));

	}

	long long result = dijsktra(maze, startPosition, endPosition);

	//Corrupt some of the places
	for (int i = 1024; i < corruptedCoords.size(); i++)
	{
		vector<int> curCorrupted = corruptedCoords[i];
		maze[curCorrupted[0]].erase(maze[curCorrupted[0]].begin() + curCorrupted[1]);
		maze[curCorrupted[0]].insert(maze[curCorrupted[0]].begin() + curCorrupted[1], Node(curCorrupted, -1));

		long long result = dijsktra(maze, startPosition, endPosition);

		if (result == LLONG_MAX)
		{
			cout << "Problematic Coord: " << curCorrupted[1] << "," << curCorrupted[0] << endl;
			break;
		}
	}


	return 0;
}

long long dijsktra(const vector<vector<Node>> & map, vector<int> & startPosition, vector<int> & endPosition)
{
	// Maze
	vector<vector<Node>> maze = map;

	//Have a list for visited locations
	vector<vector<int>> visited;

	//Make a queue
	priority_queue<Node, vector<Node>, compareNodeScores> pq;

	//Start with first node in queue
	pq.push(Node(startPosition, 0, 0));

	//Keep going while the queue is not empty
	while (!pq.empty())
	{
		//Pop the first node
		Node curNode = pq.top();
		pq.pop();

		bool nodeIsEnd = false;

		//Check if node is the end
		if (endPosition == curNode.getCoords())
		{
			nodeIsEnd = true;
		}
		if (!nodeIsEnd && !hasVisited(curNode.getCoords(), visited))
		{
			visited.push_back(curNode.getCoords());
			updateNeighbors(maze, pq, curNode);
		}
	}

	//After going through the Dijkstra’s algorithm, the end should have the lowest score
	//possible
	cout << "Lowest score to reach end: " << maze[endPosition[0]][endPosition[1]].getScore() << endl;
	return maze[endPosition[0]][endPosition[1]].getScore();
}

void updateNeighbors(vector<vector<Node>>& maze, priority_queue<Node, vector<Node>, compareNodeScores>& queue, Node& curNode)
{

	//Directions
	vector<int> xDir = { 1, 0, -1, 0 };
	vector<int> yDir = { 0, -1, 0, 1 };

	//Get the curNode's coords / direction
	int curNodeRow = curNode.getCoords()[0];
	int curNodeCol = curNode.getCoords()[1];
	int curNodeDirection = curNode.getDirection();

	//Intitalize newDirection for later
	int newDirection;

	//Initalize the possible new coords if it can move there
	vector<int> posNewCoords;

	//Go in same direction 
	posNewCoords = { curNodeRow + yDir[curNodeDirection], curNodeCol + xDir[curNodeDirection] };

	// Make sure it isn't a wall and in bounds of the maze
	if (inBounds(maze, posNewCoords) && maze[posNewCoords[0]][posNewCoords[1]].getScore() != -1)
	{
		long long tenative_score = curNode.getScore() + 1;

		if (tenative_score < maze[posNewCoords[0]][posNewCoords[1]].getScore())
		{
			//If the tenative score is less than the score at that node, change the score on the map,
			//and push it into the queue 
			maze[posNewCoords[0]][posNewCoords[1]].setScore(tenative_score);
			queue.push(Node(posNewCoords, tenative_score, curNodeDirection));
		}
	}

	//----------------------------------------//

	//Go 90 in one direction
	newDirection = curNodeDirection + 1;
	newDirection = newDirection % xDir.size(); //Makes sure it wraps around if it goes over direction vector size

	posNewCoords = { curNodeRow + yDir[newDirection], curNodeCol + xDir[newDirection] };

	// Make sure it isn't a wall and in bounds of the maze
	if (inBounds(maze, posNewCoords) && maze[posNewCoords[0]][posNewCoords[1]].getScore() != -1)
	{
		long long tenative_score = curNode.getScore() + 1;

		if (tenative_score < maze[posNewCoords[0]][posNewCoords[1]].getScore())
		{
			//If the tenative score is less than the score at that node, change the score on the map,
			//and push it into the queue 
			maze[posNewCoords[0]][posNewCoords[1]].setScore(tenative_score);
			queue.push(Node(posNewCoords, tenative_score, newDirection));
		}
	}

	//----------------------------------------//

	//Go 90 in other direction
	newDirection = curNodeDirection - 1;
	newDirection = newDirection % xDir.size(); //Makes sure it wraps around if it goes over direction vector size

	posNewCoords = { curNodeRow + yDir[newDirection], curNodeCol + xDir[newDirection] };

	// Make sure it isn't a wall and in bounds of the maze
	if (inBounds(maze, posNewCoords) && maze[posNewCoords[0]][posNewCoords[1]].getScore() != -1)
	{
		long long tenative_score = curNode.getScore() + 1;

		if (tenative_score < maze[posNewCoords[0]][posNewCoords[1]].getScore())
		{
			//If the tenative score is less than the score at that node, change the score on the map,
			//and push it into the queue 
			maze[posNewCoords[0]][posNewCoords[1]].setScore(tenative_score);
			queue.push(Node(posNewCoords, tenative_score, newDirection));
		}
	}

	//----------------------------------------//

	//Go 180 from node
	newDirection = curNodeDirection + 2;
	newDirection = newDirection % xDir.size(); //Makes sure it wraps around if it goes over direction vector size

	posNewCoords = { curNodeRow + yDir[newDirection], curNodeCol + xDir[newDirection] };

	// Make sure it isn't a wall and in bounds of the maze
	if (inBounds(maze, posNewCoords) && maze[posNewCoords[0]][posNewCoords[1]].getScore() != -1)
	{
		long long tenative_score = curNode.getScore() + 1;

		if (tenative_score < maze[posNewCoords[0]][posNewCoords[1]].getScore())
		{
			//If the tenative score is less than the score at that node, change the score on the map,
			//and push it into the queue 
			maze[posNewCoords[0]][posNewCoords[1]].setScore(tenative_score);
			queue.push(Node(posNewCoords, tenative_score, newDirection));
		}
	}
}

bool hasVisited(const vector<int>& coords, const vector<vector<int>>& visited)
{
	for (int i = 0; i < visited.size(); i++)
	{
		if (coords[0] == visited[i][0] && coords[1] == visited[i][1])
		{
			return true;
		}
	}

	return false;
}

bool inBounds(const vector<vector<Node>>& map, const vector<int>& coords)
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
		else if (numString != "" && c == ',')
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

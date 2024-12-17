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

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Maze
	vector<vector<Node>> maze;

	//Reindeer / end position
	vector<int> reindeerCoords;
	vector<int> endPosition;

	// Parse the data
	string input;
	int curMapRow = 0;

	// Make a map where every available space starts with "infinite" score
	// Walls are -1 to be ignored later
	// Start gets a 0 score
	while (getline(fileRead, input))
	{
		vector<Node> row;

		for (int i = 0; i < input.size(); i++)
		{

			//If we find the reindeer, mark where it is
			if (input[i] == 'S')
			{
				reindeerCoords = { curMapRow, i };
				row.push_back(Node(reindeerCoords, 0));
			}
			else if (input[i] == 'E')
			{
				endPosition = { curMapRow, i };
				row.push_back(endPosition);
			}
			else if (input[i] == '#')
			{
				row.push_back(Node({ curMapRow, i }, -1));
			}
			else
			{
				row.push_back(Node({ curMapRow, i }));
			}
		}

		maze.push_back(row);
		curMapRow++;
	}

	//Have a list for visited locations
	vector<vector<int>> visited;

	//Make a queue
	priority_queue<Node, vector<Node>, compareNodeScores> pq;
	
	//Start with first node in queue
	pq.push(Node(reindeerCoords, 0, 0));

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


	return 0;
}

void updateNeighbors(vector<vector<Node>> & maze, priority_queue<Node, vector<Node>, compareNodeScores>& queue, Node& curNode)
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
		long long tenative_score = curNode.getScore() + 1001;

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
		long long tenative_score = curNode.getScore() + 1001;

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
		long long tenative_score = curNode.getScore() + 2001;

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

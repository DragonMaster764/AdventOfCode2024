#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class fileNode
{
private:
	long long fileID; //negative one = free space
public:
	fileNode()
	{
		fileID = -1;
	}
	fileNode(long long id)
	{
		fileID = id;
	}
	long long getFileID()
	{
		return fileID;
	}
	void setFileID(long long id)
	{
		fileID = id;
	}
};

// Methods
bool isGap(vector<fileNode>& disk);
void moveFile(vector<fileNode>& disk);
long long calcCheckSum(vector<fileNode>& disk);

int main()
{
	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Get the data
	string input;
	getline(fileRead, input);
	/*cout << input << endl;*/

	//Disk imagery
	vector<fileNode> diskImage;
	long long fileID = 0;
	
	for (int i = 0; i < input.size(); i += 2)
	{
		int fileSize = input[i] - '0';
		int freeSpace = 0;

		if (i + 1 != input.size())
		{
			freeSpace = input[i + 1] - '0';
		}
		else
		{
			freeSpace = 0;
		}

		//For the file size place the number of the size
		for (int j = 0; j < fileSize; j++)
		{
			diskImage.push_back(fileNode(fileID));
		}

		//For the free space place '.'
		for (int j = 0; j < freeSpace; j++)
		{
			diskImage.push_back(fileNode());
		}

		fileID++;
	}

	//Testing
	/*for (int i = 0; i < diskImage.size(); i++)
	{
		int id = diskImage[i].getFileID();
		if (id == -1)
		{
			cout << '.';
		}
		else
		{
			cout << "'" << id << "'";
		}
	}*/
	cout << endl;

	//While there is a gap, move files into free space gaps
	while (isGap(diskImage))
	{
		moveFile(diskImage);
	}

	//Testing
	for (long long i = 0; i < diskImage.size(); i++)
	{
		long long id = diskImage[i].getFileID();
		if (id == -1)
		{
			cout << '.';
		}
		else
		{
			cout << "'" << id << "'";
		}
	}
	cout << endl;

	long long checkSum = calcCheckSum(diskImage);
	cout << "New checksum: " << checkSum << endl;

	return 0;
}

long long calcCheckSum(vector<fileNode>& disk)
{
	long long checksum = 0;
	for (long long i = 0; i < disk.size(); i++)
	{
		if (disk[i].getFileID() != -1)
		{
			checksum += disk[i].getFileID() * i;
		}
	}

	return checksum;
}

void moveFile(vector<fileNode>& disk)
{
	fileNode fileToMove = fileNode();

	//From the end of disk, find the first non freespace file
	for (long long i = disk.size() - 1; i >= 0; i--)
	{
		if (disk[i].getFileID() != -1)
		{
			fileToMove.setFileID(disk[i].getFileID());

			//Remove that file, then replace it with a free space
			disk.erase(disk.begin() + i);
			disk.insert(disk.begin() + i, fileNode());
			break;
		}
	}

	//Replace the first free space with the file to move
	for (long long i = 0; i < disk.size(); i++)
	{
		if (disk[i].getFileID() == -1)
		{
			disk.erase(disk.begin() + i);
			disk.insert(disk.begin() + i, fileToMove);
			break;
		}
	}

}

bool isGap(vector<fileNode>& disk)
{
	//Check for free space. Once there is a free space, if there is a nonfree space item later,
	//there is a gap between numbers
	bool prevIsFreeSpace = false;
	for (long long i = 0; i < disk.size(); i++)
	{
		if (disk[i].getFileID() == -1)
		{
			prevIsFreeSpace = true;
		}

		if (disk[i].getFileID() != -1 && prevIsFreeSpace)
		{
			return true;
		}
	}

	return false;
}




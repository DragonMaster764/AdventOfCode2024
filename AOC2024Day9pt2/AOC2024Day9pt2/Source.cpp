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
void isGap(vector<fileNode>& disk, vector<vector<int>>& gapMap);
void moveFile(vector<fileNode>& disk, vector<vector<int>> & gap);
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

	////Testing
	//for (int i = 0; i < diskImage.size(); i++)
	//{
	//	int id = diskImage[i].getFileID();
	//	if (id == -1)
	//	{
	//		cout << '.';
	//	}
	//	else
	//	{
	//		cout << "'" << id << "'";
	//	}
	//}
	//cout << endl;

	//Get a list of all gaps
	vector<vector<int>> gaps;
	isGap(diskImage, gaps);

	////Testing
	//for (int i = 0; i < gaps.size(); i++)
	//{
	//	cout << gaps[i][0] << ", " << gaps[i][1] << " | ";
	//}
	//cout << endl;

	//Move all the files into the gaps according to part 2
	moveFile(diskImage, gaps);

	////Testing
	//for (long long i = 0; i < diskImage.size(); i++)
	//{
	//	long long id = diskImage[i].getFileID();
	//	if (id == -1)
	//	{
	//		cout << '.';
	//	}
	//	else
	//	{
	//		cout << "'" << id << "'";
	//	}
	//}
	//cout << endl;

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

void moveFile(vector<fileNode>& disk, vector<vector<int>> & gaps)
{
	int fileID = disk[disk.size() - 1].getFileID(); //START
	
	while (fileID >= 0)
	{
		int fileSize = 0;
		int startFileIndex = 0;
		bool foundFile = false;

		for (int i = disk.size() - 1; i >= 0; i--)
		{
			if (disk[i].getFileID() == fileID)
			{
				foundFile = true;
				fileSize++;
			}
			else if (disk[i].getFileID() != fileID && foundFile)
			{
				foundFile = false;
				startFileIndex = i + 1; //Since i is at the index of the file we aren't looking for
				break;
			}
		}

		// Now see if there is a gap available to fit that size
		for (int i = 0; i < gaps.size(); i++)
		{
			if (gaps[i][0] >= fileSize && gaps[i][1] < startFileIndex)
			{
				//Move the file
				for (int j = 0; j < fileSize; j++)
				{
					disk.erase(disk.begin() + startFileIndex);
				}
				for (int j = 0; j < fileSize; j++)
				{
					disk.insert(disk.begin() + startFileIndex, fileNode());
				}
				
				//maybe a gaps.push_back needed?

				for (int j = 0; j < fileSize; j++)
				{
					disk.erase(disk.begin() + gaps[i][1]);
				}
				for (int j = 0; j < fileSize; j++)
				{
					disk.insert(disk.begin() + gaps[i][1], fileNode(fileID));
				}

				vector<int> oldGap = gaps[i];
				int newGapSize = oldGap[0] - fileSize;
				int newGapIndex = oldGap[1] + fileSize;
				gaps[i] = { newGapSize, newGapIndex }; //Replaces the old gap with the new info

				break;
			}
		}


		fileID--;
	}

}

void isGap(vector<fileNode> & disk, vector<vector<int>>& gapMap)
{
	//Check for free space. Once there is a free space, if there is a nonfree space item later,
	//there is a gap between numbers
	bool prevIsFreeSpace = false;
	int gapSize = 0;
	int firstSpaceIndex = 0;

	for (long long i = 0; i < disk.size(); i++)
	{
		if (disk[i].getFileID() == -1 && !prevIsFreeSpace)
		{
			prevIsFreeSpace = true;
			gapSize++;
			firstSpaceIndex = i;
		}
		else if (disk[i].getFileID() == -1)
		{
			gapSize++;
		}

		if (disk[i].getFileID() != -1 && prevIsFreeSpace)
		{
			gapMap.push_back({ gapSize, firstSpaceIndex });
			gapSize = 0;
			prevIsFreeSpace = false;
			firstSpaceIndex = 0;
		}
	}

}




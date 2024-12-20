#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

bool sortTowels(string& one, string& other)
{
	return one.size() > other.size();
}

// Methods
void readTowels(vector<string>& list, string towels);
bool findCombos(string pattern, const vector<string>& availableTowels, unordered_map<string, unsigned long long>& cache, 
	string earlierPatternPart);

int main()
{

	// Import the data
	ifstream fileRead("Input.txt", ifstream::in);

	// Parse the data
	string input;

	getline(fileRead, input);

	vector<string> availableTowels;
	readTowels(availableTowels, input);

	//Sort available towels by size so largest is done first
	sort(availableTowels.begin(), availableTowels.end(), sortTowels);

	vector<string> desiredPatterns;

	while (getline(fileRead, input))
	{
		if (!input.size())
		{
			continue;
		}

		desiredPatterns.push_back(input);

	}

	ofstream outFile("output.txt");

	unsigned long long workingTowelPatterns = 0; // Keeps track of how many patterns are possible
	unsigned long long totalCombinations = 0;
	for (string pattern : desiredPatterns)
	{ 
		unordered_map<string, unsigned long long> cache; // Cache with already made patterns

		bool patternWorked = findCombos(pattern, availableTowels, cache, "");
		if (patternWorked)
		{
			outFile << "Pattern Worked: " << pattern << endl;

			totalCombinations += cache[pattern];

			cout << "Pattern Completed: " << pattern << endl;

			workingTowelPatterns++;
		}
	}

	cout << "Number of Patterns that work: " << workingTowelPatterns << endl;
	cout << "Total Combinations: " << totalCombinations << endl;

	return 0;
}

bool findCombos(string pattern, const vector<string>& availableTowels, unordered_map<string, unsigned long long>& cache, 
	string earlierPatternPart)
{
	//End case
	if (pattern == "")
	{
		return true;
	}

	//First try cache
	if (cache.find(pattern) != cache.end())
	{
		//If they match, we know it works
		return true;
	}

	//If not in cache we will go through the available towels instead and add to the cache
	bool patternWorked = false;
	for (string towel : availableTowels)
	{
		//Make sure sizes are equal or less than the pattern size
		if (towel.size() <= pattern.size())
		{
			string matchingSizePatternSub = pattern.substr(0, towel.size());

			//If they match, recursive call for the rest of the pattern
			//also, add possible pattern to the cache
			if (towel == matchingSizePatternSub)
			{

				bool isPossible = false;
				string previousPart = earlierPatternPart + matchingSizePatternSub;
				isPossible = findCombos(pattern.substr(towel.size()), availableTowels, cache,
					previousPart);

				if (isPossible)
				{
					patternWorked = true;

					if (pattern != "" && !(cache.find(pattern) != cache.end()))
					{
						//towelCombo newCacheItem(pattern);

						unsigned long long combosToAdd = 1;

						if (cache.find(pattern.substr(towel.size())) != cache.end())
						{
							combosToAdd = cache[pattern.substr(towel.size())];
						}

						cache.insert({pattern, combosToAdd});
					}
					else
					{
						unsigned long long combosToAdd = 1;

						if (cache.find(pattern.substr(towel.size())) != cache.end())
						{
							combosToAdd = cache[pattern.substr(towel.size())];
						}

						cache[pattern] = cache[pattern] + combosToAdd;

					}
				}
			}
		}
	}

	return patternWorked;

}

void readTowels(vector<string>& list, string towels)
{
	string towelString = "";
	for (int i = 0; i < towels.size(); i++)
	{
		char c = towels[i];
		if (c != ',' && c != ' ')
		{
			towelString += towels[i];
		}
		else if (towelString != "" && c == ' ' || c == ',')
		{
			list.push_back(towelString);
			towelString = "";
		}
	}

	if (towelString != "")
	{
		list.push_back(towelString);
	}
}

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct towelCombo
{
	string towelSubstring;
	long long combosToMake;

	towelCombo(string curSubstring)
	{
		towelSubstring = curSubstring;
		combosToMake = 0;
	}

	bool operator>(const towelCombo& other) const
	{
		return towelSubstring.size() > other.towelSubstring.size();
	}
};

bool sortTowels(string& one, string& other)
{
	return one.size() > other.size();
}

// Methods
void readTowels(vector<string>& list, string towels);
bool findCombos(string pattern, const vector<string>& availableTowels, vector<towelCombo>& cache, string earlierPatternPart,
	vector<string>& failedPatterns);
bool inFailedPatterns(string pattern, const vector<string>& failures);

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

	vector<towelCombo> cache; // Cache with already made patterns 
	vector<string> failedPatterns; //Failed patterns cache
	long long workingTowelPatterns = 0; // Keeps track of how many patterns are possible
	for (string pattern : desiredPatterns)
	{
		bool patternWorked = findCombos(pattern, availableTowels, cache, "", failedPatterns);
		if (patternWorked)
		{
			outFile << "Pattern Worked: " << pattern << endl;
			workingTowelPatterns++;
		}
	}

	cout << "Number of Patterns that work: " << workingTowelPatterns << endl;

	return 0;
}

bool findCombos(string pattern, const vector<string>& availableTowels, vector<towelCombo>& cache, string earlierPatternPart,
	vector<string>& failedPatterns)
{
	//End case
	if (pattern == "")
	{
		//If the earlierPattern is not in cache, add it
		bool earlierPatternInCache = false;

		for (towelCombo towelPattern : cache)
		{
			if (earlierPatternPart == towelPattern.towelSubstring)
			{
				earlierPatternInCache = true;
			}
		}

		if (!earlierPatternInCache)
		{
			towelCombo newCacheItem(earlierPatternPart);
			cache.push_back(newCacheItem);
		}

		return true;
	}

	//First try cache
	//Make sure cache is ordered largest to smallest to do largest first (saves time on doing tiny combos)
	sort(cache.begin(), cache.end(), greater<towelCombo>());
	bool inCache = false;

	for (towelCombo towelPattern : cache)
	{
		if (towelPattern.towelSubstring.size() == pattern.size()) //Make sure the pattern isn't bigger than the pattern we are finding
		{
			string cacheSubstring = towelPattern.towelSubstring;
			string matchingSizePatternSub = pattern.substr(0, towelPattern.towelSubstring.size());

			//If they match, we know it works
			if (cacheSubstring == matchingSizePatternSub && !inFailedPatterns(matchingSizePatternSub, failedPatterns))
			{
				/*cout << "Pattern: " << earlierPatternPart + pattern << " found in cache!" << endl;*/
				return true;
			}

		}
	}

	//If not in cache we will go through the available towels instead and add to the cache
	for (string towel : availableTowels)
	{
		//Make sure sizes are equal or less than the pattern size
		if (towel.size() <= pattern.size())
		{
			string matchingSizePatternSub = pattern.substr(0, towel.size());

			//If they match, recursive call for the rest of the pattern
			//also, add possible pattern to the cache
			if (towel == matchingSizePatternSub && !inFailedPatterns(matchingSizePatternSub, failedPatterns))
			{

				bool isPossible = false;
				string previousPart = earlierPatternPart + matchingSizePatternSub;
				isPossible = findCombos(pattern.substr(towel.size()), availableTowels, cache,
					previousPart, failedPatterns);

				if (isPossible)
				{
					towelCombo newCacheItem(earlierPatternPart + matchingSizePatternSub);
					cache.push_back(newCacheItem);

					return true;
				}
				else
				{
					failedPatterns.push_back(pattern);
				}
			}
		}
	}

	/*failedPatterns.push_back(earlierPatternPart + pattern);*/

	return false;

}

bool inFailedPatterns(string pattern, const vector<string>& failures)
{
	for (string failure : failures)
	{
		if (pattern == failure)
		{
			return true;
		}
	}

	return false;
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

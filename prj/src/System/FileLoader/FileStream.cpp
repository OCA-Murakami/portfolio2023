#include <fstream>
#include <string>
#include <vector>
#include "FileStream.h"

std::vector<std::string> SetTutorialChara()
{
	std::string filePath = "data/text/TutorialString.txt";
	std::ifstream fileControler(filePath);
	std::vector<std::string> tmpStr;

	if (!fileControler)
	{
		// ‰½‚©‚µ‚ç‚µ‚½‚¢
	}

	std::string lineStr;
	while (!fileControler.eof())
	{
		std::getline(fileControler, lineStr);
		tmpStr.emplace_back(lineStr);
	}

	fileControler.close();

	return tmpStr;
}

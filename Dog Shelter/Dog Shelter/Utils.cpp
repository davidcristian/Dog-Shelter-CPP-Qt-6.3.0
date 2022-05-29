#include <sstream>
#include "Utils.h"

/// <summary>
/// Tokenizes a string
/// </summary>
/// <param name="data">the string to be tokenized</param>
/// <param name="delimiter">the delimiter used for tokenization</param>
/// <returns>a vector of strings containing the tokens</returns>
std::vector<std::string> tokenize(std::string data, char delimiter)
{
	std::vector<std::string> result;
	std::stringstream stream(data);

	std::string token;
	while (std::getline(stream, token, delimiter))
		result.push_back(token);

	return result;
}

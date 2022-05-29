#include <iostream>
#include <vector>
#include "Dog.h"
#include "Utils.h"

/// <summary>
/// Constructs the Dog
/// </summary>
/// <param name="name">The name of the dog</param>
/// <param name="breed">The breed of the dog</param>
/// <param name="age">The age of the dog</param>
/// <param name="photograph">The photograph of the dog</param>
Dog::Dog(const std::string& name, const std::string& breed, const int& age, const std::string& photograph)
	: name{ name }, breed{ breed }, age{ age }, photograph{ photograph }{}

/// <summary>
/// Lists the information of the dog
/// </summary>
/// <returns>the string representation of the dog</returns>
std::string Dog::toString() const
{
	std::string text = this->name + " is a " + this->breed + " of age " + std::to_string(this->age);
	return text;
}

/// <summary>
/// Overrides the >> operator
/// </summary>
/// <param name="stream">the stream providing data</param>
/// <param name="dog">the dog to create</param>
/// <returns>a reference to the stream</returns>
std::istream& operator>>(std::istream& stream, Dog& dog)
{
	std::string line;
	std::getline(stream, line);

	std::vector<std::string> tokens = tokenize(line, ',');
	if (tokens.size() != 4)
	{
		dog.name = dog.breed = dog.photograph = "null";
		dog.age = -32768;
		return stream;
	}

	dog.name = tokens[0];
	dog.breed = tokens[1];
	dog.age = std::stoi(tokens[2]);
	dog.photograph = tokens[3];

	return stream;
}

/// <summary>
/// Overrides the << operator
/// </summary>
/// <param name="stream">the stream receiving data</param>
/// <param name="dog">the dog providing the data</param>
/// <returns>a reference to the stream</returns>
std::ostream& operator<<(std::ostream& stream, const Dog& dog)
{
	stream << dog.name << "," << dog.breed << "," << dog.age << "," << dog.photograph << std::endl;
	return stream;
}

#include <algorithm>
#include <fstream>
#include <sstream>
#include "Repository.h"
#include "Validator.h"

/// <summary>
/// Constructor for the class, automatically reads
/// from the .txt file when created if init is true
/// </summary>
Repository::Repository(const bool& init, const std::string& fileName) : fileName{ fileName }
{
	if (init)
		this->read();
}

/// <summary>
/// Reads all the dogs from the TXT file into the repository
/// </summary>
void Repository::read()
{
	if (this->fileName.empty()) return;

	std::ifstream f(this->fileName);
	if (!f.is_open())
		throw FileException("The file could not be opened!");

	Dog dog{};
	while (f >> dog)
		this->add(dog);

	f.close();
}

/// <summary>
/// Writes a TXT file of the adoption list
/// </summary>
void Repository::write()
{
	if (this->fileName.empty()) return;

	std::ofstream f(this->fileName);
	if (!f.is_open())
		throw FileException("The file could not be opened!");

	for (const Dog& dog : this->dogs)
	{
		f << dog;
	}

	f.close();
}

/// <summary>
/// Adds a dog to the vector of dogs
/// </summary>
/// <param name="dog">the dog to add</param>
void Repository::add(const Dog& dog, int index)
{
	Dog d{};
	try
	{
		d = this->findByNameAndBreed(dog.getName(), dog.getBreed());
		throw DuplicateDogException();
	}
	catch (InexistenDogException&) {}

	if (index < 0 || index > this->size()) index = this->size();
	this->dogs.insert(this->dogs.begin() + index, dog);
	this->write();
}

/// <summary>
/// Removes a dog from the vector of dogs
/// </summary>
/// <param name="dog">the dog to remove</param>
void Repository::remove(const Dog& dog)
{
	auto it = std::find(this->dogs.begin(), this->dogs.end(), dog);
	if (it == this->dogs.end())
		throw InexistenDogException{};

	this->dogs.erase(it);
	this->write();
}

/// <summary>
/// Updates a dog in the vector of dogs
/// </summary>
/// <param name="oldDog">the old dog</param>
/// <param name="newDog">the new dog</param>
void Repository::update(const Dog& oldDog, const Dog& newDog)
{
	for (auto& dog : this->dogs)
	{
		if (dog == oldDog)
		{
			dog = newDog;
			this->write();
			return;
		}
	}

	throw InexistenDogException{};
}

/// <summary>
/// returns the index of a dog
/// </summary>
/// <param name="dog">the dog to look for</param>
/// <returns>the index of the found dog,
///			 -1 if the dog is not found</returns>
int Repository::indexOf(const Dog& dog) const
{
	auto it = std::find(this->dogs.begin(), this->dogs.end(), dog);

	return it == this->dogs.end() ? -1 : static_cast<int>(it - this->dogs.begin());
}

/// <summary>
/// Searches for a dog in the vector of dogs, throws an error if it doesn't exist
/// </summary>
/// <param name="name">the name of the dog</param>
/// <param name="breed">the breed of the dog</param>
/// <returns>the found dog</returns>
Dog& Repository::findByNameAndBreed(const std::string& name, const std::string& breed)
{
	for (auto& dog : this->dogs)
	{
		if (dog.getName() == name && dog.getBreed() == breed)
			return dog;
	}

	throw InexistenDogException{};
}

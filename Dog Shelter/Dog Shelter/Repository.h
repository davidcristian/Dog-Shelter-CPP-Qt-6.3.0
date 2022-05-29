#pragma once

#include <vector>
#include <string>
#include "Dog.h"

class Repository
{
private:
	std::vector<Dog> dogs;
	std::string fileName;

	void read();
	void write();

public:
	Repository(const bool& init = false, const std::string& fileName = "");

	void add(const Dog& dog, int index = -1);
	void remove(const Dog& dog);
	void update(const Dog& oldDog, const Dog& newDog);

	int indexOf(const Dog& dog) const;
	Dog& findByNameAndBreed(const std::string& name, const std::string& breed);
	
	std::vector<Dog>& getDogs() { return this->dogs; };
	Dog& operator[](const int& index) { return this->dogs[index]; };

	int size() const { return static_cast<int>(this->dogs.size()); };
	void setFileName(const std::string& fileName) { this->fileName = fileName; }
};

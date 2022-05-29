#pragma once

#include <vector>
#include <string>
#include "Dog.h"

class AdoptionList
{
protected:
	std::vector<Dog> dogs;
	std::string fileName = "Dogs";

public:
	AdoptionList() = default;
	virtual ~AdoptionList() = default;

	void add(const Dog& dog, int index = -1);
	void remove(const Dog& dog);
	void update(const Dog& oldDog, const Dog& newDog);

	virtual void write() = 0;
	virtual void open() = 0;

	std::vector<Dog>& getDogs() { return this->dogs; };
	int size() const { return static_cast<int>(this->dogs.size()); };
};

class CSVAdoptionList : public AdoptionList
{
private:
	std::string extension = ".csv";

public:
	CSVAdoptionList() = default;

	void write() override;
	void open() override;
};

class HTMLAdoptionList : public AdoptionList
{
private:
	std::string extension = ".html";

public:
	HTMLAdoptionList() = default;

	void write() override;
	void open() override;
};

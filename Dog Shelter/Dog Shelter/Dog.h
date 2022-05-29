#pragma once

#include <string>
#include <iostream>

class Dog
{
private:
	std::string name;
	std::string breed;
	int age;
	std::string photograph;

public:
	Dog() : name{ "" }, breed{ "" }, age{ -1 }, photograph{ "" }{}
	Dog(const std::string& name, const std::string& breed, const int& age, const std::string& photograph);

	std::string getName() const { return this->name; }
	std::string getBreed() const { return this->breed; }
	int getAge() const { return this->age; }
	std::string getPhotohraph() const { return this->photograph; }

	void setName(const std::string& _name) { this->name = _name; }
	void setBreed(const std::string& _breed) { this->breed = _breed; }
	void setAge(const int& _age) { this->age = _age; }
	void setPhotograph(const std::string& _photograph) { this->photograph = _photograph; }

	std::string toString() const;

	bool operator==(const Dog& dog) const { return this->name == dog.name && this->breed == dog.breed; };
	friend std::istream& operator>>(std::istream& stream, Dog& dog);
	friend std::ostream& operator<<(std::ostream& stream, const Dog& dog);
};

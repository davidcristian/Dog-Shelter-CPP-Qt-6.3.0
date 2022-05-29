#pragma once

#include <vector>
#include <algorithm>
#include "Dog.h"

template <class T>
class Comparator
{
public:
	virtual ~Comparator() = default;
	virtual bool compare(const T& e1, const T& e2) = 0;
};

class ComparatorAscendingByName: public Comparator<Dog>
{
public:
	bool compare(const Dog& e1, const Dog& e2) override;
};

class ComparatorDescendingByAge : public Comparator<Dog>
{
public:
	bool compare(const Dog& e1, const Dog& e2) override;
};

template <class T>
void genericSort(std::vector<T>& v, Comparator<T>* c)
{
	std::sort(v.begin(), v.end(),
		[&c](const T& elem1, const T& elem2)
		{
			return c->compare(elem1, elem2);
		}
	);
};

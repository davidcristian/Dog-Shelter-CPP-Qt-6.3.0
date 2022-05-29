#include "Comparator.h"

/// <summary>
/// Comapres 2 dogs by name
/// </summary>
/// <param name="e1">first dog</param>
/// <param name="e2">second dog</param>
/// <returns>true if the dogs are in the correct order,
///			 false, otherwise</returns>
bool ComparatorAscendingByName::compare(const Dog& e1, const Dog& e2)
{
	return e1.getName().compare(e2.getName()) < 0;
}

/// <summary>
/// Comapres 2 dogs by age
/// </summary>
/// <param name="e1">first dog</param>
/// <param name="e2">second dog</param>
/// <returns>true if the dogs are in the correct order,
///			 false, otherwise</returns>
bool ComparatorDescendingByAge::compare(const Dog& e1, const Dog& e2)
{
	return e1.getAge() > e2.getAge();
}

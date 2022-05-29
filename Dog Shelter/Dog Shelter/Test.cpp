#include <assert.h>
#include "Test.h"
#include "Repository.h"
#include "AdoptionList.h"
#include "Service.h"
#include "Comparator.h"
#include "Validator.h"

/// <summary>
/// Tests the domain
/// </summary>
void Test::testDomain()
{
	Dog dog{ "def", "abc", 3, "url1" };
	assert(dog.getBreed() == "abc");
	assert(dog.getName() == "def");
	assert(dog.getAge() == 3);
	assert(dog.getPhotohraph() == "url1");

	dog.setBreed("ghi");
	dog.setName("jkl");
	dog.setAge(4);
	dog.setPhotograph("url2");

	assert(dog.getBreed() == "ghi");
	assert(dog.getName() == "jkl");
	assert(dog.getAge() == 4);
	assert(dog.getPhotohraph() == "url2");

	Dog dog2;
	dog2.setName("jkl");
	dog2.setBreed("ghi");
	assert(dog == dog2);

	dog2.setAge(4);
	assert(dog.toString() == "jkl is a ghi of age 4");
}

/// <summary>
/// Tests the repository
/// </summary>
void Test::testRepo()
{
	Repository repo{};
	Dog dog1{ "def", "abc", 3, "url1" };
	Dog dog2{ "jkl", "ghi", 4, "url2" };
	Dog dog3{ "def", "ghi", 3, "url3" };
	Dog dog4{ "gsd", "fad", 3, "url4" };
	Dog dog5{ "gfs", "dag", 3, "url5" };

	try
	{
		repo.add(dog1);
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}
	assert(repo[0] == dog1);

	assert(repo.findByNameAndBreed(dog1.getName(), dog1.getBreed()) == dog1);

	try
	{
		assert(repo.findByNameAndBreed(dog2.getName(), dog2.getBreed()) == dog2);
		assert(false);
	}
	catch (std::exception&)
	{
		assert(true);
	}

	try
	{
		repo.update(dog3, dog2);
		assert(false);
	}
	catch (std::exception&)
	{
		assert(true);
	}

	try
	{
		repo.update(dog1, dog3);
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}

	try
	{
		repo.add(dog4);
		repo.add(dog5);
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}

	try
	{
		repo.remove(dog3);
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}

	try
	{
		repo.remove(dog2);
		assert(false);
	}
	catch (std::exception&)
	{
		assert(true);
	}
}

/// <summary>
/// Tests the service
/// </summary>
void Test::testServ()
{
	Repository repo{};
	AdoptionList* adoptionList = new CSVAdoptionList;
	DogValidator validator{};
	Service serv{ repo, adoptionList, validator };

	Dog dog1{ "def", "abc", 3, "http" };
	Dog dog2{ "jkl", "ghi", 4, "http" };
	Dog dog3{ "def", "ghi", 3, "http" };
	Dog dog4{ "gsd", "fad", 3, "http" };
	Dog dog5{ "gfs", "dag", 3, "http" };

	try
	{
		serv.add(dog1.getName(), dog1.getBreed(), dog1.getAge(), dog1.getPhotohraph());
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}
	assert(serv.getRepo().size() == 1);

	assert(serv.filterByBreedAndAge("abc", 4).size() == 1);

	try
	{
		serv.add(dog1.getName(), dog1.getBreed(), dog1.getAge(), dog1.getPhotohraph());
		assert(false);
	}
	catch (std::exception&)
	{
		assert(true);
	}
	assert(serv.getRepo().size() == 1);

	try
	{
		serv.update(dog2.getName(), dog2.getBreed(), dog3.getName(), dog3.getBreed(), dog3.getAge(), dog3.getPhotohraph());
		assert(false);
	}
	catch (std::exception&)
	{
		assert(true);
	}

	try
	{
		serv.update(dog1.getName(), dog1.getBreed(), dog3.getName(), dog3.getBreed(), dog3.getAge(), dog3.getPhotohraph());
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}

	try
	{
		serv.add(dog4.getName(), dog4.getBreed(), dog4.getAge(), dog4.getPhotohraph());
		serv.add(dog5.getName(), dog5.getBreed(), dog5.getAge(), dog5.getPhotohraph());
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}

	try
	{
		serv.remove(dog3.getName(), dog3.getBreed());
		assert(true);
	}
	catch (std::exception&)
	{
		assert(false);
	}

	try
	{
		serv.remove(dog2.getName(), dog2.getBreed());
		assert(false);
	}
	catch (std::exception&)
	{
		assert(true);
	}

	assert(serv.getRepo().size() == 2);

	Repository repo2{};
	Service serv2{ repo2, adoptionList, validator, true };
	assert(serv2.getRepo().size() == 10);

	delete adoptionList;
}

/// <summary>
/// Tests the comparator class
/// </summary>
void Test::testComparator()
{
	Repository repo{};
	Dog dog1{ "e", "abc", 1, "url1" };
	Dog dog2{ "d", "ghi", 2, "url2" };
	Dog dog3{ "c", "ghi", 3, "url3" };
	Dog dog4{ "b", "fad", 4, "url4" };
	Dog dog5{ "a", "dag", 5, "url5" };

	repo.add(dog1);
	repo.add(dog2);
	repo.add(dog3);
	repo.add(dog4);
	repo.add(dog5);

	Comparator<Dog>* comp1 = new ComparatorAscendingByName;
	genericSort<Dog>(repo.getDogs(), comp1);

	assert(repo[0].getName() == "a");
	assert(repo[1].getName() == "b");
	assert(repo[2].getName() == "c");
	assert(repo[3].getName() == "d");
	assert(repo[4].getName() == "e");
	delete comp1;

	Comparator<Dog>* comp2 = new ComparatorDescendingByAge;
	genericSort<Dog>(repo.getDogs(), comp2);

	assert(repo[0].getAge() == 5);
	assert(repo[1].getAge() == 4);
	assert(repo[2].getAge() == 3);
	assert(repo[3].getAge() == 2);
	assert(repo[4].getAge() == 1);
	delete comp2;
}

/// <summary>
/// Runs all the tests
/// </summary>
void Test::runAllTests()
{
	testDomain();
	testRepo();
	testServ();

	testComparator();
}

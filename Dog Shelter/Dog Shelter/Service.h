#pragma once

#include <vector>
#include <string>
#include "Repository.h"
#include "AdoptionList.h"
#include "Validator.h"
#include "Action.h"

class Service
{
private:
	Repository& repo;
	AdoptionList* adoptionList;
	DogValidator& validator;

	std::vector<std::unique_ptr<Action>> undoStack;
	std::vector<std::unique_ptr<Action>> redoStack;

public:
	Service(Repository& repo, AdoptionList* adoptionList, DogValidator& validator, bool generate = false);
	Repository& getRepo() { return this->repo; };

	void add(const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	void remove(const std::string& name, const std::string& breed);
	void update(const std::string& oldName, const std::string& oldBreed, const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	
	void undo();
	void redo();
	void clearUndoRedo();

	Repository filterByBreedAndAge(const std::string& breed, const int& age);
	Repository filterByString(const std::string& text);

	void adopt(const Dog& dog);
	AdoptionList* getAdoptionList() { return this->adoptionList; };
};

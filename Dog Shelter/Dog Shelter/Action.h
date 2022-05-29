#pragma once

#include "Dog.h"
#include "Repository.h"
#include "AdoptionList.h"

class Action
{
public:
	virtual void executeUndo() = 0;
	virtual void executeRedo() = 0;

	virtual ~Action() = default;
};

class ActionAdd : public Action
{
private:
	Dog addedDog;
	Repository& repo;
	int index;

public:
	ActionAdd(const Dog& dog, Repository& repo, const int& index);

	void executeUndo() override;
	void executeRedo() override;
};

class ActionRemove : public Action
{
private:
	Dog deletedDog;
	Repository& repo;
	int index;

public:
	ActionRemove(const Dog& dog, Repository& repo, const int& index);

	void executeUndo() override;
	void executeRedo() override;
};

class ActionUpdate : public Action
{
private:
	Dog oldDog;
	Dog newDog;
	Repository& repo;

public:
	ActionUpdate(const Dog& oldDog, const Dog& newDog, Repository& repo);

	void executeUndo() override;
	void executeRedo() override;
};

class ActionAdopt : public Action
{
private:
	Dog adoptedDog;
	Repository& repo;
	int repoIndex;

	Repository& dogsToShow;
	int dogsToShowIndex;
	AdoptionList* adoptionList;
	int adoptionListIndex;

public:
	ActionAdopt(const Dog& dog, Repository& repo, const int& repoIndex,
		Repository& dogsToShow, const int& dogsToShowIndex,
		AdoptionList* adoptionList, const int& adoptionListIndex);

	void executeUndo() override;
	void executeRedo() override;

	Dog getDog() const { return this->adoptedDog; }
	int getDogsToShowIndex() const { return this->dogsToShowIndex; }
};

#include "Action.h"

ActionAdd::ActionAdd(const Dog& dog, Repository& repo, const int& index) : addedDog{ dog }, repo{ repo }, index{ index } { }

void ActionAdd::executeUndo()
{
	repo.remove(addedDog);
}

void ActionAdd::executeRedo()
{
	repo.add(addedDog, index);
}

ActionRemove::ActionRemove(const Dog& dog, Repository& repo, const int& index) : deletedDog{ dog }, repo{ repo }, index{ index } { }

void ActionRemove::executeUndo()
{
	repo.add(deletedDog, index);
}

void ActionRemove::executeRedo()
{
	repo.remove(deletedDog);
}

ActionUpdate::ActionUpdate(const Dog& oldDog, const Dog& newDog, Repository& repo) : oldDog{ oldDog }, newDog{ newDog }, repo{ repo } { }

void ActionUpdate::executeUndo()
{
	repo.update(newDog, oldDog);
}

void ActionUpdate::executeRedo()
{
	repo.update(oldDog, newDog);
}

ActionAdopt::ActionAdopt(const Dog& dog, Repository& repo, const int& repoIndex,
	Repository& dogsToShow, const int& dogsToShowIndex,
	AdoptionList* adoptionList, const int& adoptionListIndex) : adoptedDog{ dog }, repo{ repo }, repoIndex{ repoIndex },
																dogsToShow{ dogsToShow }, dogsToShowIndex{ dogsToShowIndex },
																adoptionList{ adoptionList }, adoptionListIndex{ adoptionListIndex } { }

void ActionAdopt::executeUndo()
{
	adoptionList->remove(adoptedDog);
	repo.add(adoptedDog, repoIndex);

	dogsToShow.add(adoptedDog, dogsToShowIndex);
}

void ActionAdopt::executeRedo()
{
	dogsToShow.remove(adoptedDog);

	repo.remove(adoptedDog);
	adoptionList->add(adoptedDog, adoptionListIndex);
}

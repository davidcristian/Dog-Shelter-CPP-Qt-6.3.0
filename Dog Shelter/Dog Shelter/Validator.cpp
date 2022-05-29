#include "Validator.h"

GUIException::GUIException(const std::string& msg) : message(msg) { }

const char* GUIException::what()
{
	return message.c_str();
}

FileException::FileException(const std::string& msg) : message(msg) { }

const char* FileException::what()
{
	return message.c_str();
}

UndoException::UndoException(const std::string& msg) : message(msg) { }

const char* UndoException::what()
{
	return message.c_str();
}

RedoException::RedoException(const std::string& msg) : message(msg) { }

const char* RedoException::what()
{
	return message.c_str();
}

RepositoryException::RepositoryException() : exception{}, message{ "" } { }
RepositoryException::RepositoryException(const std::string& msg) : message{ msg } { }

const char* RepositoryException::what()
{
	return this->message.c_str();
}

const char* DuplicateDogException::what()
{
	return "The dog is already in the shelter!";
}

const char* InexistenDogException::what()
{
	return "The dog is not in the shelter!";
}

DogException::DogException(const std::string& _errors) : errors{ _errors } { }

std::string DogException::getErrors() const
{
	return this->errors;
}

/// <summary>
/// Validate a dog and throw an error if the data is incorrect
/// </summary>
/// <param name="dog">The dog to validate</param>
void DogValidator::validate(const Dog& dog)
{
	std::string errors;

	if (dog.getName() == "null" && dog.getBreed() == "null" && dog.getAge() == -32768 && dog.getPhotohraph() == "null")
		errors.append("Invalid number of data fields!\n");
	else
	{
		if (dog.getName().length() < 3)
			errors.append("The dog's name cannot be less than 3 characters!\n");
		if (dog.getBreed().length() < 3)
			errors.append("The dog's breed cannot be less than 3 characters!\n");
		if (dog.getAge() < 0 || dog.getAge() > 30)
			errors.append("The dog's age must be greater than -1 and less than 31!\n");
		if (dog.getPhotohraph().length() < 4 || dog.getPhotohraph().rfind("http", 0) != 0)
			errors.append("The dog's photograph must start with http!\n");
	}

	if (errors.size() > 0)
		throw DogException(errors.erase(errors.length() - 1));
}

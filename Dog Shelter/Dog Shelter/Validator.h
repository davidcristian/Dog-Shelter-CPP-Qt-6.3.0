#pragma once

#include <string>
#include "Dog.h"

class GUIException : public std::exception
{
protected:
	std::string message;

public:
	GUIException(const std::string& msg);
	virtual const char* what();
};

class FileException : public std::exception
{
protected:
	std::string message;

public:
	FileException(const std::string& msg);
	virtual const char* what();
};

class UndoException : public std::exception
{
protected:
	std::string message;

public:
	UndoException(const std::string& msg);
	virtual const char* what();
};

class RedoException : public std::exception
{
protected:
	std::string message;

public:
	RedoException(const std::string& msg);
	virtual const char* what();
};

class RepositoryException : public std::exception
{
protected:
	std::string message;

public:
	RepositoryException();
	RepositoryException(const std::string& msg);
	virtual ~RepositoryException() { }
	virtual const char* what();
};

class DuplicateDogException : public RepositoryException
{
public:
	const char* what();
};

class InexistenDogException : public RepositoryException
{
public:
	const char* what();
};

class DogException
{
private:
	std::string errors;

public:
	DogException(const std::string& _errors);
	std::string getErrors() const;
};

class DogValidator
{
public:
	DogValidator() = default;
	static void validate(const Dog& s);
};

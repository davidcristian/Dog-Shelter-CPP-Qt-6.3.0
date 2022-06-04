#include <QtWidgets/QApplication>
#include <QScreen>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QFormLayout>
#include "AdminGUI.h"

AdminGUI::AdminGUI(Service& serv, QWidget* modeSelector, QWidget* parent) : QWidget{ parent }, modeSelector{ modeSelector }, serv{ serv }
{
	this->initGUI();
	this->center();
	this->connectSignalsAndSlots();
}

void AdminGUI::showEvent(QShowEvent* e)
{
	QWidget::showEvent(e);
	this->populateDogsList();
}

void AdminGUI::initGUI()
{
	QFont font{ "Arial", 14 };
	
	// General layout of the window
	QHBoxLayout* mainHLayout = new QHBoxLayout{ this };

	// left side - just the list
	this->dogsList = new QListWidget{};
	// set the selection model
	this->dogsList->setSelectionMode(QAbstractItemView::SingleSelection);
	
	// add gradient to list
	QLinearGradient listGradient(0, 0, 0, this->dogsList->height());
	listGradient.setColorAt(0, Qt::white);
	listGradient.setColorAt(1, Qt::gray);

	QPalette palette = this->dogsList->palette();
	palette.setBrush(QPalette::ColorRole::Base, QBrush(listGradient));
	this->dogsList->setPalette(palette);
	
	// add the widget to the layout
	mainHLayout->addWidget(this->dogsList);

	// right side - dog information + buttons
	QWidget* rightSide = new QWidget{};
	QVBoxLayout* vLayout = new QVBoxLayout{ rightSide };

	// dog information
	QWidget* dogDataWidget = new QWidget{};
	QFormLayout* formLayout = new QFormLayout{ dogDataWidget };
	
	this->filterEdit = new QLineEdit{};
	this->dogNameEdit = new QLineEdit{};
	this->dogBreedEdit = new QLineEdit{};
	this->dogAgeEdit = new QLineEdit{};
	this->dogPhotographEdit = new QTextEdit{};

	QLabel* filterLabel = new QLabel{ "&Filter:" };
	filterLabel->setBuddy(this->filterEdit);
	QLabel* nameLabel = new QLabel{ "&Dog name:" };
	nameLabel->setBuddy(this->dogNameEdit);
	QLabel* breedLabel = new QLabel{ "&Dog breed: " };
	breedLabel->setBuddy(this->dogBreedEdit);
	QLabel* ageLabel = new QLabel{ "&Dog age: " };
	ageLabel->setBuddy(this->dogAgeEdit);
	QLabel* photographLabel = new QLabel{ "&Photograph:" };
	photographLabel->setBuddy(this->dogPhotographEdit);
	
	filterLabel->setFont(font);
	nameLabel->setFont(font);
	breedLabel->setFont(font);
	ageLabel->setFont(font);
	photographLabel->setFont(font);
	this->filterEdit->setFont(font);
	this->dogNameEdit->setFont(font);
	this->dogBreedEdit->setFont(font);
	this->dogAgeEdit->setFont(font);
	this->dogPhotographEdit->setFont(font);
	
	formLayout->addRow(filterLabel, this->filterEdit);
	formLayout->addRow(nameLabel, this->dogNameEdit);
	formLayout->addRow(breedLabel, this->dogBreedEdit);
	formLayout->addRow(ageLabel, this->dogAgeEdit);
	formLayout->addRow(photographLabel, this->dogPhotographEdit);

	vLayout->addWidget(dogDataWidget);

	// buttons
	QWidget* buttonsWidget = new QWidget{};
	QHBoxLayout* hLayout = new QHBoxLayout{ buttonsWidget };
	
	this->undoButton = new QPushButton("Undo");
	this->undoButton->setFont(font);

	this->redoButton = new QPushButton("Redo");
	this->redoButton->setFont(font);

	QWidget* buttonsWidget2 = new QWidget{};
	QHBoxLayout* hLayout2 = new QHBoxLayout{ buttonsWidget2 };

	this->addDogButton = new QPushButton("Add Dog");
	this->addDogButton->setFont(font);
	
	this->deleteDogButton = new QPushButton("Delete Dog");
	this->deleteDogButton->setFont(font);

	this->updateDogButton = new QPushButton("Update Dog");
	this->updateDogButton->setFont(font);

	this->changeModeButton = new QPushButton("Change Mode");
	this->changeModeButton->setFont(font);
	
	hLayout->addWidget(this->undoButton);
	hLayout->addWidget(this->redoButton);
	
	hLayout2->addWidget(this->addDogButton);
	hLayout2->addWidget(this->deleteDogButton);
	hLayout2->addWidget(this->updateDogButton);
	hLayout2->addWidget(this->changeModeButton);

	vLayout->addWidget(buttonsWidget);
	vLayout->addWidget(buttonsWidget2);

	// add everything to the layout
	mainHLayout->addWidget(this->dogsList);
	mainHLayout->addWidget(rightSide);

	// disable buttons on initial empty list
	this->deleteDogButton->setEnabled(false);
	this->updateDogButton->setEnabled(false);

	this->undoButton->setEnabled(false);
	this->redoButton->setEnabled(false);

	// set focus on add button
	this->addDogButton->setFocus();
}

// Fix windows high DPI scaling
void AdminGUI::center()
{
	qApp->processEvents();

	int screenWidth = qApp->primaryScreen()->availableGeometry().width();
	int screenHeight = qApp->primaryScreen()->availableGeometry().height();

	this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);
}

void AdminGUI::connectSignalsAndSlots()
{
	// when the list of dogs is updated - re-populate the list
	QObject::connect(this->filterEdit, &QLineEdit::textChanged, this, &AdminGUI::filter);
	QObject::connect(this, &AdminGUI::dogsUpdatedSignal, this, &AdminGUI::populateDogsList);
	QObject::connect(this, &AdminGUI::loadDogsSignal, this, &AdminGUI::loadDogs);

	// add a connection: function listItemChanged() will be called when an item in the list is selected
	QObject::connect(this->dogsList, &QListWidget::itemSelectionChanged, this, [this]() { this->listItemChanged(); });

	// add button connections
	QObject::connect(this->addDogButton, &QPushButton::clicked, this, &AdminGUI::addDogButtonHandler);
	QObject::connect(this->deleteDogButton, &QPushButton::clicked, this, &AdminGUI::deleteDogButtonHandler);
	QObject::connect(this->updateDogButton, &QPushButton::clicked, this, &AdminGUI::updateDogButtonHandler);
	QObject::connect(this->changeModeButton, &QPushButton::clicked, this, &AdminGUI::changeModeButtonHandler);

	// connect the signals
	QObject::connect(this, SIGNAL(addDogSignal(const std::string&, const std::string&, const int&, const std::string&)),
		this, SLOT(addDog(const std::string&, const std::string&, const int&, const std::string&)));

	QObject::connect(this, SIGNAL(removeDogSignal(const std::string&, const std::string&)),
		this, SLOT(removeDog(const std::string&, const std::string&)));

	QObject::connect(this, SIGNAL(updateDogSignal(const std::string&, const std::string&, const std::string&, const std::string&, const int&, const std::string&)),
		this, SLOT(updateDog(const std::string&, const std::string&, const std::string&, const std::string&, const int&, const std::string&)));

	// create shortcuts
	this->undoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
	this->redoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
	QObject::connect(this->undoShortcut, &QShortcut::activated, this, &AdminGUI::undo);
	QObject::connect(this->redoShortcut, &QShortcut::activated, this, &AdminGUI::redo);

	// add undo and redo connections
	QObject::connect(this->undoButton, &QPushButton::clicked, this, &AdminGUI::undo);
	QObject::connect(this->redoButton, &QPushButton::clicked, this, &AdminGUI::redo);
}

void AdminGUI::showInformation(const std::string& info)
{
	QMessageBox::information(this, "Information", QString::fromStdString(info));
}

void AdminGUI::showError(const std::string& err)
{
	QMessageBox::critical(this, "Error", QString::fromStdString(err));
}

void AdminGUI::populateDogsList()
{
	if (this->filterEdit->text().size() > 0)
	{
		this->filter(this->filterEdit->text());
		return;
	}

	int oldIndex = this->getSelectedIndex();
	this->dogsList->clear();
	
	this->dogsToShow = this->serv.getRepo().getDogs();
	emit loadDogsSignal(oldIndex);
}

void AdminGUI::filter(const QString& qstr)
{
	std::string text = qstr.toStdString();
	if (text.size() == 0)
	{
		emit dogsUpdatedSignal();
		return;
	}

	int oldIndex = this->getSelectedIndex();
	if (text.size() == 1) oldIndex = 0;
	this->dogsList->clear();

	this->dogsToShow = this->serv.filterByString(text).getDogs();
	emit loadDogsSignal(oldIndex);
}

void AdminGUI::loadDogs(int oldIndex)
{
	for (auto& dog : this->dogsToShow)
	{
		QString itemInList = QString::fromStdString(dog.getName() + " - " + dog.getBreed());
		QListWidgetItem* item = new QListWidgetItem{ itemInList };

		QFont f{ "Arial", 14 };
		item->setFont(f);

		this->dogsList->addItem(item);
	}

	// set the selection to the previous one
	// (if possible) so the cursor doesn't jump
	if (oldIndex == -1 || this->dogsList->count() == 0)
	{
		this->dogsList->setCurrentRow(0);

		if (this->dogsList->count() > 0)
			this->selectedDog = this->dogsToShow[0];
		else
			this->selectedDog = Dog{};
	}
	else
	{
		while (oldIndex >= this->dogsList->count())
			oldIndex--;

		this->dogsList->setCurrentRow(oldIndex);
		this->selectedDog = this->dogsToShow[oldIndex];
	}

	this->deleteDogButton->setEnabled(this->dogsList->count() > 0);
	this->updateDogButton->setEnabled(this->dogsList->count() > 0);

	this->undoButton->setEnabled(true);
	this->redoButton->setEnabled(true);
}

void AdminGUI::listItemChanged()
{
	int index = this->getSelectedIndex();
	if (index == -1 || index >= this->dogsToShow.size())
		return;

	Dog dog = this->dogsToShow[index];
	this->selectedDog = dog;

	this->dogNameEdit->setText(QString::fromStdString(dog.getName()));
	this->dogBreedEdit->setText(QString::fromStdString(dog.getBreed()));
	this->dogAgeEdit->setText(QString::fromStdString(std::to_string(dog.getAge())));
	this->dogPhotographEdit->setText(QString::fromStdString(dog.getPhotohraph()));
}

int AdminGUI::getSelectedIndex()
{
	if (this->dogsList->count() == 0)
		return -1;

	// get selected index
	QModelIndexList listIndex = this->dogsList->selectionModel()->selectedIndexes();
	if (listIndex.size() == 0)
	{
		this->dogNameEdit->clear();
		this->dogBreedEdit->clear();
		this->dogAgeEdit->clear();
		this->dogPhotographEdit->clear();

		return -1;
	}

	return listIndex.at(0).row();
}

void AdminGUI::addDogButtonHandler()
{
	// read data from the textboxes and add the new dog
	std::string name = this->dogNameEdit->text().toStdString();
	std::string breed = this->dogBreedEdit->text().toStdString();
	std::string ageStr = this->dogAgeEdit->text().toStdString();

	int age = ageStr.size() == 0 || ageStr.find_first_not_of("0123456789") != std::string::npos ? -1 : std::stoi(ageStr);
	std::string photograph = this->dogPhotographEdit->toPlainText().toStdString();

	emit addDogSignal(name, breed, age, photograph);
}

void AdminGUI::deleteDogButtonHandler()
{
	// read data from the textboxes and delete the dog

	std::string name = this->dogNameEdit->text().toStdString();
	std::string breed = this->dogBreedEdit->text().toStdString();

	emit removeDogSignal(name, breed);
}

void AdminGUI::updateDogButtonHandler()
{
	// read data from the textboxes and add the new dog
	std::string name = this->dogNameEdit->text().toStdString();
	std::string breed = this->dogBreedEdit->text().toStdString();
	std::string ageStr = this->dogAgeEdit->text().toStdString();

	int age = ageStr.size() == 0 || ageStr.find_first_not_of("0123456789") != std::string::npos ? -1 : std::stoi(ageStr);
	std::string photograph = this->dogPhotographEdit->toPlainText().toStdString();

	emit updateDogSignal(this->selectedDog.getName(), this->selectedDog.getBreed(), name, breed, age, photograph);
}

void AdminGUI::changeModeButtonHandler()
{
	this->hide();
	this->modeSelector->show();
}

void AdminGUI::undo()
{
	try
	{
		this->serv.undo();
		emit dogsUpdatedSignal();
	
		this->showInformation("The last operation was undone successfully.");
	}
	catch (UndoException& e)
	{
		this->showError(e.what());
	}
}

void AdminGUI::redo()
{
	try
	{
		this->serv.redo();
		emit dogsUpdatedSignal();

		this->showInformation("The last operation was redone successfully.");
	}
	catch (RedoException& e)
	{
		this->showError(e.what());
	}
}

void AdminGUI::clearUndoRedo()
{
	this->serv.clearUndoRedo();
}

void AdminGUI::addDog(const std::string& name, const std::string& breed, const int& age, const std::string& photograph)
{
	try
	{
		this->serv.add(name, breed, age, photograph);
		emit dogsUpdatedSignal();

		emit clearUserUndoRedoSignal();
		this->showInformation("The dog has been added to the shelter.");
	}
	catch (DogException& e)
	{
		this->showError(e.getErrors());
	}
	catch (RepositoryException& e)
	{
		this->showError(e.what());
	}
	catch (FileException& e)
	{
		this->showError(e.what());
	}
}

void AdminGUI::removeDog(const std::string& name, const std::string& breed)
{
	try
	{
		this->serv.remove(name, breed);
		emit dogsUpdatedSignal();

		emit clearUserUndoRedoSignal();
		this->showInformation("The dog has been removed from the shelter.");
	}
	catch (DogException& e)
	{
		this->showError(e.getErrors());
	}
	catch (RepositoryException& e)
	{
		this->showError(e.what());
	}
	catch (FileException& e)
	{
		this->showError(e.what());
	}
}

void AdminGUI::updateDog(const std::string& oldName, const std::string& oldBreed, const std::string& name, const std::string& breed, const int& age, const std::string& photograph)
{
	try
	{
		this->serv.update(oldName, oldBreed, name, breed, age, photograph);
		emit dogsUpdatedSignal();

		emit clearUserUndoRedoSignal();
		this->showInformation("The dog was updated with the new information.");
	}
	catch (DogException& e)
	{
		this->showError(e.getErrors());
	}
	catch (RepositoryException& e)
	{
		this->showError(e.what());
	}
	catch (FileException& e)
	{
		this->showError(e.what());
	}
}

#pragma once

#include <qwidget.h>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QShortcut>
#include "Service.h"
#include "Dog.h"

class AdminGUI : public QWidget
{
	Q_OBJECT

public:
	AdminGUI(Service& serv, QWidget* modeSelector, QWidget* parent = Q_NULLPTR);
	~AdminGUI() = default;

private:
	QWidget* modeSelector;
	Service& serv;
	std::vector<Dog> dogsToShow;
	Dog selectedDog;
	
	QListWidget* dogsList;

	QLineEdit* filterEdit;
	QLineEdit* dogNameEdit;
	QLineEdit* dogBreedEdit;
	QLineEdit* dogAgeEdit;
	QTextEdit* dogPhotographEdit;

	QPushButton* undoButton;
	QPushButton* redoButton;
	QShortcut* undoShortcut;
	QShortcut* redoShortcut;

	QPushButton* addDogButton;
	QPushButton* deleteDogButton;
	QPushButton* updateDogButton;
	QPushButton* changeModeButton;

	void initGUI();
	void center();
	void connectSignalsAndSlots();
	void showEvent(QShowEvent* e) override;
	void showInformation(const std::string& info);
	void showError(const std::string& err);

	void populateDogsList();
	void listItemChanged();
	int getSelectedIndex();

	void addDogButtonHandler();
	void deleteDogButtonHandler();
	void updateDogButtonHandler();
	void changeModeButtonHandler();

signals:
	void dogsUpdatedSignal();
	void loadDogsSignal(int oldIndex = 0);

	void addDogSignal(const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	void removeDogSignal(const std::string& name, const std::string& breed);
	void updateDogSignal(const std::string& oldName, const std::string& oldBreed, const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	
	void clearUserUndoRedoSignal();

public slots:
	void loadDogs(int oldIndex = 0);
	void filter(const QString& qstr);

	void undo();
	void redo();

	void addDog(const std::string& name, const std::string& breed, const int& age, const std::string& photograph);
	void removeDog(const std::string& name, const std::string& breed);
	void updateDog(const std::string& oldName, const std::string& oldBreed, const std::string& name, const std::string& breed, const int& age, const std::string& photograph);

	void clearUndoRedo();
};

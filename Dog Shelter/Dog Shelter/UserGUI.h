#pragma once

#include <qwidget.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>
#include <QShortcut>
#include <unordered_map>
#include "AdoptionTableModel.h"
#include "Service.h"
#include "AdoptionList.h"
#include "Action.h"
#include "Dog.h"

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240

class UserGUI : public QWidget
{
	Q_OBJECT

public:
	UserGUI(Service& serv, QWidget* modeSelector, QWidget* parent = Q_NULLPTR);
	~UserGUI();

private:
	Service& serv;
	QWidget* modeSelector;
	QNetworkAccessManager* networkManager;
	std::unordered_map<QString, QPixmap>* images;

	std::vector<std::unique_ptr<Action>> undoStack;
	std::vector<std::unique_ptr<Action>> redoStack;

	Repository dogsToShow;
	int currentIndex = -1;

	AdoptionList* adopted;
	
	QTabWidget* tabWidget;
	AdoptionTableModel* tableModel;
	QTableView* picturesTableView;

	QLabel* dogImage;
	QLineEdit* dogNameEdit;
	QLineEdit* dogBreedEdit;
	QLineEdit* dogAgeEdit;

	QPushButton* viewAllButton;
	QPushButton* viewFilteredButton;
	QPushButton* showAdoptedButton;
	QPushButton* changeModeButton;

	QPushButton* undoButton;
	QPushButton* redoButton;
	QShortcut* undoShortcut;
	QShortcut* redoShortcut;

	QPushButton* adoptButton;
	QPushButton* nextButton;
	QPushButton* stopButton;

	void initGUI();
	void center();
	void connectSignalsAndSlots();
	void showInformation(const std::string& info);
	void showError(const std::string& err);

	void adoptButtonHandler();

	void loadCurrentDog();
	void loadImage(const QString& imageURL);

signals:
	void prepareAdoptionSignal();
	void adoptSignal(const Dog& dog);

	void nextDogSignal();
	void stopShowingDogsSignal();

	void updateTableSignal();
	void clearAdminUndoRedoSignal();

public slots:
	void receivedReply(QNetworkReply* reply);
	
	void viewAllDogs();
	void viewFilteredDogs();
	void showAdoptionList();
	void changeModeButtonHandler();

	void undo();
	void redo();

	void prepareAdoption();
	void adopt(const Dog& dog);
	void loadNextDog();
	void stopShowingDogs();
	
	void updateTable();
	void clearUndoRedo();
};

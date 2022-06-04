#include <QtWidgets/QApplication>
#include <QScreen>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include "UserGUI.h"

UserGUI::UserGUI(Service& serv, QWidget* modeSelector, QWidget* parent) : QWidget{ parent }, modeSelector{ modeSelector }, serv{ serv }
{
	this->networkManager = new QNetworkAccessManager{ this };
	this->images = new std::unordered_map<QString, QPixmap>();

	this->adopted = this->serv.getAdoptionList();
	this->tableModel = new AdoptionTableModel{ this->adopted };
	this->pictureDelegate = new PictureDelegate{ this->tableModel, this->images };

	this->initGUI();
	this->center();
	this->connectSignalsAndSlots();
}

UserGUI::~UserGUI()
{
	delete this->images;
}

void UserGUI::initGUI()
{	
	QFont font{ "Arial", 14 };
	QFont tabFont{ "Arial", 12 };

	QHBoxLayout* centralLayout = new QHBoxLayout{ this };
	this->tabWidget = new QTabWidget{ this };
	this->tabWidget->setFont(tabFont);

	// ====== FIRST TAB ======
	QWidget* firstTab = new QWidget{};
	QHBoxLayout* firstTabHLayout = new QHBoxLayout{ firstTab };

	// left side - just the image
	this->dogImage = new QLabel{};
	QPixmap pixmap{};
	pixmap.fill(Qt::white);
	this->dogImage->setPixmap(pixmap);
	// set the selection model
	firstTabHLayout->addWidget(this->dogImage);

	// right side - dog information + buttons
	QWidget* firsTabRightSide = new QWidget{};
	QVBoxLayout* vLayout = new QVBoxLayout{ firsTabRightSide };

	// dog information
	QWidget* dogDataWidget = new QWidget{};
	QFormLayout* formLayout = new QFormLayout{ dogDataWidget };

	this->dogNameEdit = new QLineEdit{};
	this->dogBreedEdit = new QLineEdit{};
	this->dogAgeEdit = new QLineEdit{};

	QLabel* nameLabel = new QLabel{ "&Dog name: " };
	nameLabel->setBuddy(this->dogNameEdit);
	QLabel* breedLabel = new QLabel{ "&Dog breed: " };
	breedLabel->setBuddy(this->dogBreedEdit);
	QLabel* ageLabel = new QLabel{ "&Dog age: " };
	ageLabel->setBuddy(this->dogAgeEdit);

	nameLabel->setFont(font);
	breedLabel->setFont(font);
	ageLabel->setFont(font);
	this->dogNameEdit->setFont(font);
	this->dogBreedEdit->setFont(font);
	this->dogAgeEdit->setFont(font);

	formLayout->addRow(nameLabel, this->dogNameEdit);
	formLayout->addRow(breedLabel, this->dogBreedEdit);
	formLayout->addRow(ageLabel, this->dogAgeEdit);

	vLayout->addWidget(dogDataWidget);

	// buttons
	QWidget* buttonsWidget1 = new QWidget{};
	QHBoxLayout* hLayout1 = new QHBoxLayout{ buttonsWidget1 };

	this->viewAllButton = new QPushButton("View All Dogs");
	this->viewAllButton->setFont(font);

	this->viewFilteredButton = new QPushButton("View Matching Dogs");
	this->viewFilteredButton->setFont(font);

	// buttons
	QWidget* buttonsWidget2 = new QWidget{};
	QHBoxLayout* hLayout2 = new QHBoxLayout{ buttonsWidget2 };

	this->openAdoptedButton = new QPushButton("Open Adoption List");
	this->openAdoptedButton->setFont(font);

	this->changeModeButton = new QPushButton("Change Mode");
	this->changeModeButton->setFont(font);

	// buttons
	QWidget* buttonsWidget3 = new QWidget{};
	QHBoxLayout* hLayout3 = new QHBoxLayout{ buttonsWidget3 };

	this->undoButton = new QPushButton("Undo");
	this->undoButton->setFont(font);

	this->redoButton = new QPushButton("Redo");
	this->redoButton->setFont(font);

	// buttons
	QWidget* buttonsWidget4 = new QWidget{};
	QHBoxLayout* hLayout4 = new QHBoxLayout{ buttonsWidget4 };

	this->adoptButton = new QPushButton("Adopt");
	this->adoptButton->setFont(font);

	this->nextButton = new QPushButton("Next");
	this->nextButton->setFont(font);

	this->stopButton = new QPushButton("Stop");
	this->stopButton->setFont(font);

	hLayout1->addWidget(this->viewAllButton);
	hLayout1->addWidget(this->viewFilteredButton);

	hLayout2->addWidget(this->openAdoptedButton);
	hLayout2->addWidget(this->changeModeButton);

	hLayout3->addWidget(this->undoButton);
	hLayout3->addWidget(this->redoButton);

	hLayout4->addWidget(this->adoptButton);
	hLayout4->addWidget(this->nextButton);
	hLayout4->addWidget(this->stopButton);

	vLayout->addWidget(buttonsWidget1);
	vLayout->addWidget(buttonsWidget2);
	vLayout->addWidget(buttonsWidget3);
	vLayout->addWidget(buttonsWidget4);

	this->dogNameEdit->setEnabled(false);
	this->openAdoptedButton->setEnabled(false);

	this->undoButton->setEnabled(false);
	this->redoButton->setEnabled(false);

	this->adoptButton->setEnabled(false);
	this->nextButton->setEnabled(false);
	this->stopButton->setEnabled(false);

	// add everything to the firstTabHLayout
	firstTabHLayout->addWidget(firsTabRightSide);

	// ====== SECOND TAB ======
	QWidget* secondTab = new QWidget{};
	QHBoxLayout* secondTabHLayout = new QHBoxLayout{ secondTab };

	this->picturesTableView = new QTableView(secondTab);

	// set the model
	this->picturesTableView->setModel(this->tableModel);

	// set the custom delegate
	this->picturesTableView->setItemDelegate(this->pictureDelegate);

	// hide the vertical header
	this->picturesTableView->verticalHeader()->hide();
	// force the columns to resize, according to the size of their contents
	this->picturesTableView->resizeColumnsToContents();
	this->picturesTableView->resizeRowsToContents();

	//this->tabWidget->tabBar()->setEnabled(false);
	secondTabHLayout->addWidget(this->picturesTableView);

	// add the tabs
	this->tabWidget->addTab(firstTab, "Adopt");
	this->tabWidget->addTab(secondTab, "Adoption List");

	centralLayout->addWidget(this->tabWidget);
	this->tabWidget->setCurrentIndex(0);
}

// Fix windows high DPI scaling
void UserGUI::center()
{
	qApp->processEvents();

	int screenWidth = qApp->primaryScreen()->availableGeometry().width();
	int screenHeight = qApp->primaryScreen()->availableGeometry().height();

	this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);
}

void UserGUI::connectSignalsAndSlots()
{
	QObject::connect(this->networkManager, &QNetworkAccessManager::finished, this, &UserGUI::receivedReply);

	// add button connections
	QObject::connect(this->viewAllButton, &QPushButton::clicked, this, &UserGUI::viewAllDogs);
	QObject::connect(this->viewFilteredButton, &QPushButton::clicked, this, &UserGUI::viewFilteredDogs);
	QObject::connect(this->openAdoptedButton, &QPushButton::clicked, this, &UserGUI::openAdoptionList);
	QObject::connect(this->changeModeButton, &QPushButton::clicked, this, &UserGUI::changeMode);

	QObject::connect(this->adoptButton, &QPushButton::clicked, this, &UserGUI::adoptButtonHandler);
	QObject::connect(this->nextButton, &QPushButton::clicked, this, &UserGUI::loadNextDog);
	QObject::connect(this->stopButton, &QPushButton::clicked, this, &UserGUI::stopShowingDogs);

	// connect the signals
	QObject::connect(this, SIGNAL(prepareAdoptionSignal()), this, SLOT(prepareAdoption()));
	QObject::connect(this, SIGNAL(adoptSignal(const Dog&)), this, SLOT(adopt(const Dog&)));
	QObject::connect(this, SIGNAL(nextDogSignal()), this, SLOT(loadNextDog()));
	QObject::connect(this, SIGNAL(stopShowingDogsSignal()), this, SLOT(stopShowingDogs()));

	// create shortcuts
	this->undoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
	this->redoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
	QObject::connect(this->undoShortcut, &QShortcut::activated, this, &UserGUI::undo);
	QObject::connect(this->redoShortcut, &QShortcut::activated, this, &UserGUI::redo);

	// add undo and redo connections
	QObject::connect(this->undoButton, &QPushButton::clicked, this, &UserGUI::undo);
	QObject::connect(this->redoButton, &QPushButton::clicked, this, &UserGUI::redo);

	QObject::connect(this, &UserGUI::updateTableSignal, this, &UserGUI::updateTable);
}

void UserGUI::showInformation(const std::string& info)
{
	QMessageBox::information(this, "Information", QString::fromStdString(info));
}

void UserGUI::showError(const std::string& err)
{
	QMessageBox::critical(this, "Error", QString::fromStdString(err));
}

void UserGUI::loadCurrentDog()
{
	Dog dog = this->dogsToShow[this->currentIndex];

	this->dogNameEdit->setText(QString::fromStdString(dog.getName()));
	this->dogBreedEdit->setText(QString::fromStdString(dog.getBreed()));
	this->dogAgeEdit->setText(QString::fromStdString(std::to_string(dog.getAge())));

	QString photograph = QString::fromStdString(dog.getPhotohraph());

	if (this->images->find(photograph) != this->images->end())
	{
		QPixmap pixmap = this->images->at(photograph);

		QImage image{ pixmap.toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied) };
		image = image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT, Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::TransformationMode::SmoothTransformation);
		image = image.copy((image.width() - IMAGE_WIDTH) / 2, (image.height() - IMAGE_HEIGHT) / 2, IMAGE_WIDTH, IMAGE_HEIGHT);

		this->dogImage->setPixmap(QPixmap::fromImage(image));
	}
	else
	{
		this->loadImage(photograph);
	}

	//std::string command = "start ";
	//system(command.append(dog.getPhotohraph()).c_str());
}

void UserGUI::loadImage(const QString& imageURL)
{
	QUrl url{ imageURL };
	this->networkManager->get(QNetworkRequest(url));
}

void UserGUI::viewAllDogs()
{
	this->dogsToShow = this->serv.getRepo();
	emit prepareAdoptionSignal();
}

void UserGUI::viewFilteredDogs()
{
	std::string breed = this->dogBreedEdit->text().toStdString();
	std::string ageStr = this->dogAgeEdit->text().toStdString();
	int age = ageStr.size() == 0 || ageStr.find_first_not_of("0123456789") != std::string::npos ? -1 : std::stoi(ageStr);

	this->dogsToShow = this->serv.filterByBreedAndAge(breed, age);
	emit prepareAdoptionSignal();
}

void UserGUI::openAdoptionList()
{
	try
	{
		this->serv.getAdoptionList()->open();
	}
	catch (RepositoryException& e)
	{
		this->showError(e.what());
	}
}

void UserGUI::changeMode()
{
	this->hide();
	this->modeSelector->show();
}

void UserGUI::adoptButtonHandler()
{
	Dog dog = dogsToShow[this->currentIndex];

	std::unique_ptr<Action> p = std::make_unique<ActionAdopt>(
		dog, this->serv.getRepo(), this->serv.getRepo().indexOf(dog),
		this->dogsToShow, this->dogsToShow.indexOf(dog),
		this->serv.getAdoptionList(), this->serv.getAdoptionList()->size() + 1);
	undoStack.push_back(std::move(p));
	redoStack.clear();

	this->dogsToShow.remove(dog);

	emit adoptSignal(dog);
	this->currentIndex--;

	if (dogsToShow.size() == 0)
	{
		this->showInformation("There are no more dogs available for adoption.");
		emit stopShowingDogsSignal();
	}
	else
	{
		emit nextDogSignal();
	}
}

void UserGUI::loadNextDog()
{
	this->currentIndex++;
	if (this->currentIndex >= this->dogsToShow.size())
		this->currentIndex = 0;

	this->loadCurrentDog();
}

void UserGUI::stopShowingDogs()
{
	this->currentIndex = -1;
	this->dogsToShow.getDogs().clear();

	QPixmap pixmap{};
	pixmap.fill(Qt::white);

	this->dogImage->setPixmap(pixmap);

	this->dogNameEdit->clear();
	this->dogBreedEdit->clear();
	this->dogAgeEdit->clear();

	this->undoButton->setEnabled(this->adopted->size() > 0);
	this->redoButton->setEnabled(this->adopted->size() > 0);

	this->adoptButton->setEnabled(false);
	this->nextButton->setEnabled(false);
	this->stopButton->setEnabled(false);

	this->dogBreedEdit->setEnabled(true);
	this->dogAgeEdit->setEnabled(true);

	this->viewAllButton->setEnabled(true);
	this->viewFilteredButton->setEnabled(true);
	this->openAdoptedButton->setEnabled(this->adopted->size() > 0);
	this->changeModeButton->setEnabled(true);

	//this->tabWidget->tabBar()->setEnabled(this->hasAdopted);
}

void UserGUI::undo()
{
	try
	{
		if (undoStack.size() == 0)
			throw UndoException("There is nothing to undo!");

		std::unique_ptr<Action> action = std::move(undoStack.back());
		undoStack.pop_back();

		{
			Action* tempBase = action.get();
			ActionAdopt* tempDerived = static_cast<ActionAdopt*>(tempBase);

			if (this->currentIndex >= tempDerived->getDogsToShowIndex())
				this->currentIndex++;
		}

		action.get()->executeUndo();
		redoStack.push_back(std::move(action));

		emit updateTableSignal();
		this->showInformation("The last operation was undone successfully.");
	}
	catch (UndoException& e)
	{
		this->showError(e.what());
	}
}

void UserGUI::redo()
{
	try
	{
		if (redoStack.size() == 0)
			throw RedoException("There is nothing to redo!");

		std::unique_ptr<Action> action = std::move(redoStack.back());
		redoStack.pop_back();

		{
			Action* tempBase = action.get();
			ActionAdopt* tempDerived = static_cast<ActionAdopt*>(tempBase);

			if (this->currentIndex != -1 && tempDerived->getDog() == dogsToShow[this->currentIndex])
				emit nextDogSignal();
			
			if (this->currentIndex >= tempDerived->getDogsToShowIndex())
				this->currentIndex--;
		}

		action.get()->executeRedo();
		undoStack.push_back(std::move(action));

		emit updateTableSignal();
		this->showInformation("The last operation was redone successfully.");
	}
	catch (RedoException& e)
	{
		this->showError(e.what());
	}
}

void UserGUI::clearUndoRedo()
{
	undoStack.clear();
	redoStack.clear();
}

void UserGUI::prepareAdoption()
{
	if (dogsToShow.size() == 0)
	{
		this->showInformation("There are no dogs available for adoption.");
		return;
	}

	this->dogsToShow.setFileName("");

	this->dogBreedEdit->setEnabled(false);
	this->dogAgeEdit->setEnabled(false);

	this->viewAllButton->setEnabled(false);
	this->viewFilteredButton->setEnabled(false);
	this->openAdoptedButton->setEnabled(false);
	this->changeModeButton->setEnabled(false);

	this->undoButton->setEnabled(true);
	this->redoButton->setEnabled(true);

	this->adoptButton->setEnabled(true);
	this->nextButton->setEnabled(true);
	this->stopButton->setEnabled(true);

	//this->tabWidget->tabBar()->setEnabled(false);

	this->currentIndex = 0;
	this->loadCurrentDog();
}

void UserGUI::adopt(const Dog& dog)
{
	try
	{
		this->serv.adopt(dog);
		emit updateTableSignal();

		emit clearAdminUndoRedoSignal();
		this->showInformation("The dog has been added to the adoption list.");
	}
	catch (FileException& e)
	{
		this->showError(e.what());
	}
}

void UserGUI::updateTable()
{
	emit this->tableModel->layoutChanged();
	// force the columns to resize, according to the size of their contents
	this->picturesTableView->resizeColumnsToContents();
	this->picturesTableView->resizeRowsToContents();
}

void UserGUI::receivedReply(QNetworkReply* reply)
{
	QPixmap pixmap{};
	pixmap.fill(Qt::black);

	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray replyData = reply->readAll();
		pixmap.loadFromData(replyData);

		if (pixmap.isNull())
		{
			pixmap = QPixmap{};
			pixmap.fill(Qt::black);
		}
	}

	this->images->operator[](reply->url().toString()) = pixmap;
	
	QImage image{ pixmap.toImage().convertToFormat(QImage::Format_ARGB32_Premultiplied) };
	image = image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT, Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::TransformationMode::SmoothTransformation);
	image = image.copy((image.width() - IMAGE_WIDTH) / 2, (image.height() - IMAGE_HEIGHT) / 2, IMAGE_WIDTH, IMAGE_HEIGHT);

	this->dogImage->setPixmap(QPixmap::fromImage(image));
	reply->deleteLater();
}

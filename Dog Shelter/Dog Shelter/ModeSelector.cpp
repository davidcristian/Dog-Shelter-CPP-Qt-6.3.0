#include <QtWidgets/QApplication>
#include <QScreen>
#include <QGridLayout>
#include <QLabel>
#include "ModeSelector.h"
#include "AdminGUI.h"
#include "UserGUI.h"

ModeSelector::ModeSelector(const int& repoType, QWidget* parent) : QWidget{ parent }
{
	switch (repoType)
	{
	case 0:
		adoptionList = std::make_unique<CSVAdoptionList>();
		break;
	case 1:
		adoptionList = std::make_unique<HTMLAdoptionList>();
		break;
	default:
		throw RepositoryException("Unable to create Adoption List!");
	}

	this->repo = std::make_unique<Repository>(true, "Dogs.txt");
	this->validator = std::make_unique<DogValidator>();
	this->serv = std::make_unique<Service>(*repo.get(), adoptionList.get(), *validator.get(), repo.get()->size() == 0);
	
	this->adminGUI = std::make_unique<AdminGUI>(*this->serv.get(), this);
	this->userGUI = std::make_unique<UserGUI>(*this->serv.get(), this);

	this->initGUI();
	this->center();
	this->connectSignalsAndSlots();
}

void ModeSelector::initGUI()
{
	QFont font{ "Arial", 14 };
	QVBoxLayout* mainVLayout = new QVBoxLayout{ this };

	// label
	QWidget* labelWidget = new QWidget{};
	QHBoxLayout* labelLayout = new QHBoxLayout{ labelWidget };

	QLabel* label = new QLabel{ "Access Type" };
	label->setAlignment(Qt::AlignCenter);
	label->setFont(font);

	labelLayout->addWidget(label, Qt::AlignCenter);
	labelLayout->setContentsMargins(0, 16, 0, 16);

	// buttons
	QWidget* buttonWidget = new QWidget{};
	QHBoxLayout* buttonLayout = new QHBoxLayout{ buttonWidget };

	this->userButton = new QPushButton("User");
	this->userButton->setFont(font);

	this->adminButton = new QPushButton("Admin");
	this->adminButton->setFont(font);

	buttonLayout->addWidget(this->userButton);
	buttonLayout->addWidget(this->adminButton);
	buttonLayout->setContentsMargins(16, 16, 16, 16);
	buttonLayout->setSpacing(128);

	mainVLayout->addWidget(labelWidget);
	mainVLayout->addWidget(buttonWidget);
}

// Fix windows high DPI scaling
void ModeSelector::center()
{
	int screenWidth = qApp->primaryScreen()->availableGeometry().width();
	int screenHeight = qApp->primaryScreen()->availableGeometry().height();

	this->move((screenWidth - this->width() / 2) / 2, (screenHeight - this->height() / 2) / 2);
}

void ModeSelector::connectSignalsAndSlots()
{
	QObject::connect(this, SIGNAL(modeSelectedSignal(const int&)), this, SLOT(modeSelected(const int&)));

	QObject::connect(this->userButton, &QPushButton::clicked, this, &ModeSelector::userButtonHandler);
	QObject::connect(this->adminButton, &QPushButton::clicked, this, &ModeSelector::adminButtonHandler);

	QObject::connect(this->adminGUI.get(), SIGNAL(clearUserUndoRedoSignal()), this->userGUI.get(), SLOT(clearUndoRedo()));
	QObject::connect(this->userGUI.get(), SIGNAL(clearAdminUndoRedoSignal()), this->adminGUI.get(), SLOT(clearUndoRedo()));
}

void ModeSelector::adminButtonHandler()
{
	emit modeSelectedSignal(0);
}

void ModeSelector::userButtonHandler()
{
	emit modeSelectedSignal(1);
}

void ModeSelector::modeSelected(const int& accessMode)
{
	this->hide();

	switch (accessMode)
	{
	case 0:
		this->adminGUI.get()->show();
		break;
	case 1:
		this->userGUI.get()->show();
		break;
	default:
		throw GUIException("Unable to open selected GUI!");
	}
}

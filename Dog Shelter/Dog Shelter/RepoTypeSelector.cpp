#include <QtWidgets/QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "RepoTypeSelector.h"
#include "ModeSelector.h"

RepoTypeSelector::RepoTypeSelector(QWidget* parent) : QWidget{ parent }
{
	this->initGUI();
	this->center();
	this->connectSignalsAndSlots();
}

void RepoTypeSelector::initGUI()
{
	QFont font{ "Arial", 14 };
	QVBoxLayout* mainVLayout = new QVBoxLayout{ this };

	// label
	QWidget* labelWidget = new QWidget{};
	QHBoxLayout* labelLayout = new QHBoxLayout{ labelWidget };

	QLabel* label = new QLabel{ "Repository Type" };
	label->setAlignment(Qt::AlignCenter);
	label->setFont(font);

	labelLayout->addWidget(label, Qt::AlignCenter);
	labelLayout->setContentsMargins(0, 16, 0, 16);

	// buttons
	QWidget* buttonWidget = new QWidget{};
	QHBoxLayout* buttonLayout = new QHBoxLayout{ buttonWidget };

	this->CSVButton = new QPushButton("CSV");
	this->CSVButton->setFont(font);

	this->HTMLButton = new QPushButton("HTML");
	this->HTMLButton->setFont(font);

	buttonLayout->addWidget(this->CSVButton);
	buttonLayout->addWidget(this->HTMLButton);
	buttonLayout->setContentsMargins(16, 16, 16, 16);
	buttonLayout->setSpacing(128);

	mainVLayout->addWidget(labelWidget);
	mainVLayout->addWidget(buttonWidget);
}

// Fix windows high DPI scaling
void RepoTypeSelector::center()
{
	int screenWidth = qApp->primaryScreen()->availableGeometry().width();
	int screenHeight = qApp->primaryScreen()->availableGeometry().height();

	this->move((screenWidth - this->width() / 2) / 2, (screenHeight - this->height() / 2) / 2);
}

void RepoTypeSelector::connectSignalsAndSlots()
{
	QObject::connect(this, SIGNAL(repoSelectedSignal(const int&)), this, SLOT(repoSelected(const int&)));

	QObject::connect(this->CSVButton, &QPushButton::clicked, this, &RepoTypeSelector::CSVButtonHandler);
	QObject::connect(this->HTMLButton, &QPushButton::clicked, this, &RepoTypeSelector::HTMLBUttonButtonHandler);
}

void RepoTypeSelector::CSVButtonHandler()
{
	emit repoSelectedSignal(0);
}

void RepoTypeSelector::HTMLBUttonButtonHandler()
{
	emit repoSelectedSignal(1);
}

void RepoTypeSelector::repoSelected(const int& repoType)
{
	this->hide();

	this->modeSelector = std::make_unique<ModeSelector>(repoType);
	modeSelector.get()->show();

	this->close();
}

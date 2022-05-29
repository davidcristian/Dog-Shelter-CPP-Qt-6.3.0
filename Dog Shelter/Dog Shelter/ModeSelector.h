#pragma once

#include <qwidget.h>
#include <QPushButton>

#include "AdoptionList.h"
#include "Repository.h"
#include "Validator.h"
#include "Service.h"

class ModeSelector : public QWidget
{
	Q_OBJECT

public:
	ModeSelector(const int& repoType, QWidget* parent = Q_NULLPTR);
	~ModeSelector() = default;

private:
	std::unique_ptr<AdoptionList> adoptionList;
	std::unique_ptr<Repository> repo;
	std::unique_ptr<DogValidator> validator;
	std::unique_ptr<Service> serv;

	std::unique_ptr<QWidget> adminGUI;
	std::unique_ptr<QWidget> userGUI;
	QPushButton* userButton;
	QPushButton* adminButton;

	void initGUI();
	void center();
	void connectSignalsAndSlots();

	void adminButtonHandler();
	void userButtonHandler();

signals:
	void modeSelectedSignal(const int& accessMode);

public slots:
	void modeSelected(const int& accessMode);
};

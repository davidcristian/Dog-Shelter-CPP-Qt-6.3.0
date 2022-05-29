#pragma once

#include <qwidget.h>
#include <QPushButton>

class RepoTypeSelector : public QWidget
{
	Q_OBJECT

public:
	RepoTypeSelector(QWidget* parent = Q_NULLPTR);
	~RepoTypeSelector() = default;

private:
	std::unique_ptr<QWidget> modeSelector;
	QPushButton* CSVButton;
	QPushButton* HTMLButton;

	void initGUI();
	void center();
	void connectSignalsAndSlots();

	void CSVButtonHandler();
	void HTMLBUttonButtonHandler();

signals:
	void repoSelectedSignal(const int& repoType);

public slots:
	void repoSelected(const int& repoType);
};

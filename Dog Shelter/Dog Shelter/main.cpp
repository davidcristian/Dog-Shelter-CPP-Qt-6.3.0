#include <QtWidgets/QApplication>
#include "RepoTypeSelector.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QGuiApplication::setApplicationDisplayName("Dog Shelter");

	std::unique_ptr<RepoTypeSelector> repoTypeSelector = std::make_unique<RepoTypeSelector>();
	repoTypeSelector.get()->show();
	
	return a.exec();
}

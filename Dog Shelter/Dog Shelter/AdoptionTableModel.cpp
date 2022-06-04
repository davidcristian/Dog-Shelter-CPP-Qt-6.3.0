#include <QFont>
#include <QBrush>
#include "AdoptionTableModel.h"

AdoptionTableModel::AdoptionTableModel(AdoptionList* adoptionList, QObject* parent) : QAbstractTableModel{ parent }, adoptionList{ adoptionList } { }

int AdoptionTableModel::rowCount(const QModelIndex& parent) const
{
	// add 1 to show empty row for Qt::EditRole
	return this->adoptionList->size();
}

int AdoptionTableModel::columnCount(const QModelIndex& parent) const
{
	return 4;
}

QVariant AdoptionTableModel::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	int column = index.column();

	// get the dogs
	std::vector<Dog> dogs = this->adoptionList->getDogs();

	// Allow adding in the table
	// this is to show an empty row at the end of the table - to allow adding new dogs
	if (row == dogs.size())
		return QVariant{};

	// get the dog from the current row
	Dog dog = dogs[row];
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch (column)
		{
		case 0:
			return QString::fromStdString(dog.getName());
		case 1:
			return QString::fromStdString(dog.getBreed());
		case 2:
			return QString::fromStdString(std::to_string(dog.getAge()));
		case 3:
			return QString::fromStdString(dog.getPhotohraph());
		default:
			break;
		}
	}
	if (role == Qt::FontRole)
	{
		QFont font{ "Arial", 14 };
		return font;
	}
	if (role == Qt::BackgroundRole)
	{
		if (row % 2 == 1)
			return QBrush{ Qt::white }; // QBrush{ Qt::lightGray };
	}

	return QVariant{};
}

QVariant AdoptionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case 0:
				return QString{ "Name" };
			case 1:
				return QString{ "Breed" };
			case 2:
				return QString{ "Age" };
			case 3:
				return QString{ "Photograph" };
			default:
				break;
			}
		}
	}
	if (role == Qt::FontRole)
	{
		QFont font{ "Arial", 15 };
		font.setBold(true);
		return font;
	}

	return QVariant{};
}

// TODO:
// - validate single cell edit and insert
// - check for duplicate dog on edit finish
bool AdoptionTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid() || role != Qt::EditRole)
		return false;

	// set the new data to the dog
	int dogIndex = index.row();

	// get the dogs
	std::vector<Dog> dogs = this->adoptionList->getDogs();

	std::string valueStr = value.toString().toStdString();
	int age = valueStr.size() == 0 || valueStr.find_first_not_of("0123456789") != std::string::npos ? -1 : std::stoi(valueStr);

	// Allow adding in the table
	//if the index is >= number of dogs => a new dog is added
	if (dogIndex == dogs.size())
	{
		this->beginInsertRows(QModelIndex{}, dogIndex, dogIndex);

		switch (index.column())
		{
		case 0:
			this->adoptionList->add(Dog{ valueStr, "", -1, "" });
			break;
		case 1:
			this->adoptionList->add(Dog{ "", valueStr, -1, "" });
			break;
		case 2:
			this->adoptionList->add(Dog{ "", "", age, ""});
			break;
		case 3:
			this->adoptionList->add(Dog{ "", "", -1, valueStr });
			break;
		}

		this->endInsertRows();
		return true;
	}

	Dog& currentDog = dogs[dogIndex];
	Dog oldDog = currentDog;
	switch (index.column())
	{
	case 0:
		currentDog.setName(valueStr);
		break;
	case 1:
		currentDog.setBreed(valueStr);
		break;
	case 2:
		currentDog.setAge(age);
		break;
	case 3:
		currentDog.setPhotograph(valueStr);
		break;
	}
	this->adoptionList->update(oldDog, currentDog);

	// emit the dataChanged signal
	emit dataChanged(index, index);
	return true;
}

Qt::ItemFlags AdoptionTableModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEnabled; // | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

#pragma once

#include <QAbstractTableModel>
#include "AdoptionList.h"

class AdoptionTableModel : public QAbstractTableModel
{
private:
	AdoptionList* adoptionList;

public:
	AdoptionTableModel(AdoptionList* adoptionList, QObject* parent = Q_NULLPTR);
	~AdoptionTableModel() = default;

	// number of rows
	int rowCount(const QModelIndex& parent = QModelIndex{}) const override;

	// number of columns
	int columnCount(const QModelIndex& parent = QModelIndex{}) const override;

	// Value at a given position
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	// add header data
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// will be called when a cell is edited
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	// used to set certain properties of a cell
	Qt::ItemFlags flags(const QModelIndex& index) const override;
};

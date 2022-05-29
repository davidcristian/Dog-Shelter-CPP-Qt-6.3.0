#pragma once

#include <unordered_map>
#include <QStyledItemDelegate>
#include <QPixmap>
#include <QtNetwork/QNetworkAccessManager>
#include "AdoptionTableModel.h"

#define TABLE_IMAGE_WIDTH 32
#define TABLE_IMAGE_HEIGHT 96

class PictureDelegate : public QStyledItemDelegate
{
private:
	AdoptionTableModel* model;

	QNetworkAccessManager* networkManager;
	std::unordered_map<QString, QPixmap>* images;

public:
	PictureDelegate(AdoptionTableModel* model, std::unordered_map<QString, QPixmap>* images, QWidget* parent = Q_NULLPTR);
	~PictureDelegate() = default;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

public slots:
	void receivedReply(QNetworkReply* reply);
};

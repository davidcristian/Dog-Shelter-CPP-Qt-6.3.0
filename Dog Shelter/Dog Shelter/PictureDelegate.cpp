#include <QPainter>
#include <QtNetwork/QNetworkReply>
#include "PictureDelegate.h"
#include "Dog.h"

PictureDelegate::PictureDelegate(AdoptionTableModel* model, QWidget* parent) : QStyledItemDelegate{ parent }, model{ model }
{
	this->networkManager = new QNetworkAccessManager{ this };
	QObject::connect(this->networkManager, &QNetworkAccessManager::finished, this, &PictureDelegate::receivedReply);
}

void PictureDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// show a picture only in the fourth column; the other columns remain unchanged
	if (index.column() != 3)
	{
		if (index.column() == 2 && index.model()->data(index, Qt::EditRole).toString() == "-1")
			return;

		QStyledItemDelegate::paint(painter, option, index);
		return;
	}
	
	QString photograph = index.model()->data(index, Qt::EditRole).toString();

	if (this->images.find(photograph) != this->images.end())
	{
		QPixmap pixmap = this->images.at(photograph);
		painter->drawPixmap(option.rect, pixmap);
	}
	else
	{
		QUrl url{ photograph };
		this->networkManager->get(QNetworkRequest(url));

		QPixmap pixmap{};
		pixmap.fill(Qt::white);

		painter->drawPixmap(option.rect, pixmap);
	}

	//QStyledItemDelegate::paint(painter, option, index);
}

QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// show a picture only in the fourth column; the other columns remain unchanged
	if (index.column() == 3)
		return QSize(TABLE_IMAGE_WIDTH, TABLE_IMAGE_HEIGHT);

	return QStyledItemDelegate::sizeHint(option, index);
}

PictureDelegate::~PictureDelegate()
{
	delete this->networkManager;
}

void PictureDelegate::receivedReply(QNetworkReply* reply)
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

	this->images[reply->url().toString()] = pixmap;
	emit this->model->layoutChanged();

	reply->deleteLater();
}

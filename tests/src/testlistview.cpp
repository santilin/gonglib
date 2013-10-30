
#include <iostream>
#include <gongguiapplication.h>
#include <QListView>
#include <QStringListModel>
#include <QAbstractListModel>
#include "testlistview.h"


using namespace gong;

TestListView::TestListView()
{
}


TestListView::~TestListView()
{
}


class DataModel: public QAbstractListModel
{
public:
	DataModel( QObject *parent) : QAbstractListModel( parent ) {}
	int rowCount( const QModelIndex & parent = QModelIndex() ) const { return 2; }
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const
	{
		if ( role == Qt::DisplayRole ) {
			return "a";
		}
		return QVariant();
	}
};

void TestListView::run()
{
	int argc = 1;
	char *argv[] = { "gongcommontests", 0 };
	QApplication app(argc, argv, true);
	QListView *lv = new QListView();
	DataModel d(lv);
	lv->setModel( &d );
	lv->show();
	app.setMainWidget(lv);
	app.exec();
}

class ListView : public QListView
{
public:
  ListView()
  {
    QStringListModel *myModel = new QStringListModel(this);
    QStringList list;
    list << "a" << "b" <<"c" <<"d" <<"e" <<"f" <<"g" <<"h" <<"i" <<"j" <<"k";
    myModel->setStringList(list);
    setModel(myModel);
  }
};


void TestListView::runStringList()
{
	int argc = 1;
	char *argv[] = { "gongcommontests", 0 };
	QApplication app(argc, argv, true);
	ListView window;
	window.resize(100, 50);
	window.show();
//	app.setMainWidget(window);
	app.exec();
}

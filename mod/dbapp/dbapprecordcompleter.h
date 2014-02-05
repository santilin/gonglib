#ifndef GONG_DBAPPCOMPLETER_H
#define GONG_DBAPPCOMPLETER_H

#include <QCompleter>
#include <gongdbdefinition.h>

namespace gong {

class RecordCompleterModel: public QAbstractListModel
{
public:
	RecordCompleterModel(dbRecord *record, const Xtring &fldname, QObject *parent = 0 );
	virtual ~RecordCompleterModel();

	int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
private:
	dbResultSet *pResultSet;
};

class RecordCompleter: public QCompleter
{
public:
	RecordCompleter( dbRecord *record, const Xtring &fldname, QObject *parent = 0);
	RecordCompleter( dbDefinition *db, dbConnection *conn,
			   const Xtring &tablename, const Xtring &fldname, QObject *parent = 0 );
	virtual ~RecordCompleter();
private:
	RecordCompleterModel *pRecCompModel;
};

} // namespace gong
#endif

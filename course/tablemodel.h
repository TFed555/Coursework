#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <QVariant>
#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QDebug>
#include "db.h"

class tablemodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    tablemodel( QObject* parent = 0);

private:
    int rowCount( const QModelIndex& parent ) const;
     int columnCount( const QModelIndex& parent ) const;
     QVariant data( const QModelIndex& index, int role ) const;
       QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

      void appendPerson(  const QString& id, const QString& title, const QString& desc,
                          const QString& deadline, const QString& pay, const QString& status, const QString& responsible );
private:
    enum Column {
        ID = 0,
        TITLE,
        DESC,
        DEADLINE,
        PAY,
        STATUS,
        RESPONSIBLE,
        LAST
    };

    typedef QHash< Column, QVariant > PersonData;
    typedef QList< PersonData > Persons;
    Persons m_persons;
private:
     DataBase *db;
};

#endif // TABLEMODEL_H

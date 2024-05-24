#ifndef WORKSMODEL_H
#define WORKSMODEL_H

#include <QVariant>
#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QDebug>
#include "db.h"

class WorksModel : public QAbstractTableModel
{
    Q_OBJECT
public:
   static WorksModel* instance();

public:
    void updateWorkStatus(int id, int status);
    void updateAddWork();
    void removeWorks(const QModelIndexList &indexes);
    QList<QList<QVariant>> getList();
private:
    void setupModel();
    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    void appendWork( const int& id, const QString& title,
                          const QString& status );

private:
    enum Column {
        ID = 0,
        TITLE,
        STATUS,
        LAST
    };

    typedef QHash <Column, QVariant> ListData;
    typedef QList <ListData> Works;
    Works works;
private:
    WorksModel(QObject *parent = nullptr);
    WorksModel(const WorksModel&) = delete;
    WorksModel& operator = (const WorksModel&) = delete;
    static WorksModel* m_instance;
    DataBase *db;
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
protected:
    ~WorksModel();
};

#endif // WORKSMODEL_H

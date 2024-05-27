#ifndef USERSMODEL_H
#define USERSMODEL_H

#include <QVariant>
#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QDebug>
#include "db.h"

class UsersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
   static UsersModel* instance();

public:
   void removeUsers(const QModelIndexList &indexes);
   void updateModel();
   void updateUserRole(int id, int role);
   void updateUserUnit(int id, QString unit);
   void updateUserPost(int id, QString post);
    QList<QList<QVariant>> getList();
private:
    void setupModel();
    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    void appendUser( const int& id, const QString& role, const QString& surname, const QString& name, const QString& patronymic, const QString& phone,
                          const QString& unit, const int& role_id, const QString& degree, const QString& rank, const QString& post );

private:
    enum Column {
        ID = 0,
        ROLE,
        SURNAME,
        NAME,
        PATRONYMIC,
        PHONE,
        PASSWORD,
        UNIT,
        ROLE_ID,
        DEGREE,
        RANK,
        POST,
        LAST
    };

    typedef QHash <Column, QVariant> ListData;
    typedef QList <ListData> Users;
    Users users;
private:
    UsersModel(QObject *parent = nullptr);
    UsersModel(const UsersModel&) = delete;
    UsersModel& operator = (const UsersModel&) = delete;
    static UsersModel* m_instance;
    DataBase *db;
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
protected:
    ~UsersModel();
};
#endif // USERSMODEL_H

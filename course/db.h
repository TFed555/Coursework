#ifndef DB_H
#define DB_H

#include <QSqlRecord>

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>


class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();

    void connectToDataBase();
    bool insertIntoUsersTable(const QVariantList &data);
    bool insertIntoWorksTable(const QVariantList &data);
    bool loginExists(QString login);
    bool pswdCompare(QString login, QString pswd);
    void deleteTable(const QString &tableName);
    bool tableExists(const QString &tableName);
    int getRole(QString login);
    void closeDataBase();

public:
    QSqlDatabase db;

private:
    const QString dbName = "./DB.db";


private:
    void insertIntoRolesTable(QString data);
    void insertIntoStatusTable(QString data);
    bool openDataBase();
    bool restoreDataBase();
    bool createUsersTable();
    bool createRolesTable();
    bool createWorksTable();
    bool createStatusTable();
};

#endif // DB_H

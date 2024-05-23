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
    void closeDataBase();
    bool insertIntoUsersTable(const QVariantList &data);
    bool insertIntoWorksTable(const QVariantList &data);
    bool insertIntoTasksTable(const int userID, const int workID);
    bool loginExists(QString login);
    bool pswdCompare(QString login, QString pswd);
    void deleteTable(const QString &tableName);
    bool tableExists(const QString &tableName);
    int getRole(QString login);
    int getStatus(int workID);
    QString getStatusName(int ID);
    int getLastWorkID();
    bool updateWorkStatus(int workID, int statusID);
    int getTaskID(int workID, QString order);
    QString getRoleName(int role);
    bool updateTaskResponsibles(int workID, int respID, int taskID);

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
    bool createTasksTable();
};

#endif // DB_H

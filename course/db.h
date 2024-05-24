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
//методы для бд
    void connectToDataBase();
    void closeDataBase();
    bool insertIntoUsersTable(const QVariantList &data);
    bool insertIntoWorksTable(const QVariantList &data);
    bool insertIntoTasksTable(const int userID, const int workID);
    void deleteTable(const QString &tableName);
    bool tableExists(const QString &tableName);
//методы для окна авторизации
    bool loginExists(QString login);
    bool pswdCompare(QString login, QString pswd);
//геттеры
    int getRole(QString login);
    int getStatus(int workID);
    QString getStatusName(int ID);
    int getLastWorkID();
    int getResponsible(int workID);
    int getTaskID(int workID, QString order);
    QString getRoleName(int role);
    QList<QList<QVariant>> selectAllStatus();
    int getCountTasks(int workID);
//методы для обновления
    bool updateWorkStatus(int workID, int statusID);
    bool updateUserRole(int userID, int role);
    bool updateUserPost(int userID, QString post);
    bool updateTaskResponsibles(int workID, int respID, int taskID);

    int checkUserID(QString login);

public:
    QSqlDatabase db;


private:
    const QString dbName = "./DB.db";
    QSqlQuery query;


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

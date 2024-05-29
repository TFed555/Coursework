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
    QList<QList<QVariant>> selectTasks(int workID);
    QList<QList<QVariant>> selectUsers();
    QList<QList<QVariant>> selectWorks();
    QSet<int> selectWorksForUser(const int &userID);
//методы для обновления
    bool updateWorkStatus(int workID, int statusID);
    bool updateUserRole(int userID, int role);
    bool updateUserUnit(int userID, QString unit);
    bool updateUserPost(int userID, QString label, QString post);
    bool updateUserSalary(const int userID, const int pay);
    bool updateTaskResponsibles(int workID, int respID, int taskID);
    bool deleteResponsible(int taskID);
    bool deleteUsers(QVariantList idList);
    bool deleteWorks(QVariantList idList);
    bool finishTasks(QVariantList idList);
    int checkUserID(QString login);

public:
    QSqlDatabase db;


private:
    const QString dbName = "./DB.db";
    QSqlQuery query;


private:
    void insertIntoRolesTable(QString data);
    void insertIntoStatusTable(QString data);
    void insertIntoSalaryTable(QVariantList &data);
    bool openDataBase();
    bool restoreDataBase();
    bool createUsersTable();
    bool createRolesTable();
    bool createWorksTable();
    bool createStatusTable();
    bool createTasksTable();
    bool createSalaryTable();
};

#endif // DB_H

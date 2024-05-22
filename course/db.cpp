#include "db.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}



void DataBase::connectToDataBase()
{

    if(!QFile(dbName).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }


}

bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createUsersTable()){
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}


bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(db.open()){
        QSqlQuery query(db);
        query.exec("PRAGMA foreign_keys = ON;");
        if (!tableExists("Roles")){
            createRolesTable();
        }
        if (!tableExists("Users")){
            createUsersTable();
        }
        if (!tableExists("Status")){
            createStatusTable();
        }
        if (!tableExists("Works")){
            createWorksTable();
        }
        if (!tableExists("Tasks")){
            createTasksTable();
        }
        return true;
    } else {
        return false;
    }

}


void DataBase::closeDataBase()
{
    db.close();
}


bool DataBase::createRolesTable(){
     QSqlQuery query;
     if(!query.exec( "CREATE TABLE  IF NOT EXISTS Roles ("
                             "Role_ID  INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "Name     NVARCHAR(20)    NOT NULL"
                         ")"
                     )){
         qDebug() << "DataBase: error of create " << "Roles";
         qDebug() << query.lastError().text();
         return false;
     }
     else {
            insertIntoRolesTable("сотрудник");
            insertIntoRolesTable("организатор");
            insertIntoRolesTable("администратор");
         return true;
     }
     return false;
}

void DataBase::insertIntoRolesTable(QString data){
    QSqlQuery query;
    query.prepare("INSERT INTO Roles( Name ) "
                  "   VALUES( :name )"
    );
    query.bindValue(":name", data);
    if( !query.exec() ) {
            qDebug() << db.lastError().text();
        }
}

bool DataBase::createUsersTable()
{

    QSqlQuery query;
    if(!query.exec( "CREATE TABLE Users ("
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "Role                    INTEGER                  NOT NULL,"
                            "Surname                 NVARCHAR(255)            NOT NULL,"
                            "Name                    NVARCHAR(255)            NOT NULL,"
                            "Patronymic              NVARCHAR(255)            NULL,"
                            "PhoneNumber             NVARCHAR(12)                  NOT NULL,"
                            "Password                VARCHAR(60)              NOT NULL,"
                            "Post                    NVARCHAR(255)            NOT NULL,"
                            "FOREIGN KEY  (Role)     REFERENCES  Roles(Role_ID)"

                        ")"
                    )){
        qDebug() << "DataBase: error of create " << "Users";
        qDebug() << query.lastError().text();
        return false;
    }
    else{
        return true;
    }
//    else {
//    if (!query.exec("INSERT INTO Users( Role, Surname, Name, Patronymic, phoneNumber, Password, Post )"
//                    "VALUES ( 3, 'Бердин', 'Игорь', 'Олегович', '+71231231231', 'admin', 'Преподаватель' )"
//                )){
//        qDebug() << "error insert admin";
//        return false;
//    }
//    else{
//        return true;
//    }
//    }
    return false;
}

bool DataBase::insertIntoUsersTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Users( Role, Surname, Name, Patronymic, phoneNumber, Password, Post ) "
                  "   VALUES( :role, :surname, :name, :patronymic, :phoneNumber, :password, :post )"
    );
    query.bindValue(":role",  data[0].toInt());
    query.bindValue(":surname", data[1].toString());
    query.bindValue(":name", data[2].toString());
    query.bindValue(":patronymic", data[3].toString());
    query.bindValue(":phoneNumber", data[4].toString());
    query.bindValue(":password", data[5].toString());
    query.bindValue(":post", data[6].toString());

    if(!query.exec()){
            qDebug() << "error insert into " << "Users";
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
        return false;
}

bool DataBase::tableExists(const QString &tableName)
{
    if (db.tables().contains(tableName)) return true;
    return false;
}

bool DataBase::loginExists(QString login){
    QSqlQuery query;
    query.prepare("Select * From Users Where phoneNumber = :phoneNumber");
    query.bindValue(":phoneNumber", login);

    if(!query.exec())
       {
        qDebug() << "Error select from table Users";
        qDebug() << query.lastError().text();
        return false;
    }
    else {
        //qDebug() << "Selected from table Users";
        return query.next();
    }

    return false;
}

bool DataBase::pswdCompare(QString login, QString pswd){
    QSqlQuery query;
    auto ok = query.prepare("Select Password From Users "
                            "Where PhoneNumber = :login;");
    if (!ok) {
        qDebug() << query.lastError();
        return false;
    }
    query.bindValue(":login", login);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }

    query.next();
    QVariant res = query.value(0).toString();
    if (pswd.toLatin1().toHex()!=res){
        return false;
    }
    return true;
}

int DataBase::getRole(QString login){
    QSqlQuery query;
    query.prepare("Select Role From Users "
                  "Where phoneNumber = :login;");
    query.bindValue(":login", login);
    if (!query.exec()){
        qDebug()<<query.lastError().text();
        return 0;
    }

    query.next();
    return query.value(0).toInt();
}

void DataBase::deleteTable(const QString &tableName)
{
    QSqlDatabase db = QSqlDatabase::database();

     if(db.isValid() && db.isOpen()){

         QSqlQuery q(db);
         QString query= QString("DROP TABLE IF EXISTS %1;").arg(tableName);

         if(q.exec(query))
             qDebug() << "delete table";
     }
}

bool DataBase::createStatusTable(){
     QSqlQuery query;
     if(!query.exec( "CREATE TABLE IF NOT EXISTS Status ("
                             "ID  INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "Name     NVARCHAR(20)    NOT NULL"
                         ")"
                     )){
         qDebug() << "DataBase: error of create " << "Status";
         qDebug() << query.lastError().text();
         return false;
     }
     else {
            insertIntoStatusTable("В ожидании");
            insertIntoStatusTable("Выполняется");
            insertIntoStatusTable("Выполнено");
         return true;
     }
     return false;
}

void DataBase::insertIntoStatusTable(QString data){
    QSqlQuery query;
    query.prepare("INSERT INTO Status( Name ) "
                  "   VALUES( :name )"
    );
    query.bindValue(":name", data);
    if( !query.exec() ) {
            qDebug() << db.lastError().text();
        }
}

bool DataBase::createWorksTable()
{

    QSqlQuery query;
    if(!query.exec( "CREATE TABLE Works ("
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "Title                     NVARCHAR(255)            NOT NULL,"
                            "Deadline                  DATE                     NOT NULL,"
                            "Pay                       INTEGER                    NOT NULL,"
                            "Description              NVARCHAR(255)             NULL,"
                            "Status                    INTEGER                  NOT NULL,"
                            "FOREIGN KEY  (Status)     REFERENCES  Status(ID) "
                        ")"
                    )){
        qDebug() << "DataBase: error of create " << "Works";
        qDebug() << query.lastError().text();
        return false;
    }
    else{
        if (!query.exec("INSERT INTO Works( Title, Deadline, Pay, Description, Status )"
                            "VALUES ( 'Тест', '2024-04-04', 5500, '00', 1 )"
                        )){
                qDebug() << "error insert work";
                return false;
            }
            else{
                return true;
            }
    }
    return false;
}

bool DataBase::insertIntoWorksTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Works( Title, Deadline, Pay, Description, Status ) "
                  "   VALUES( :title, :deadline, :pay, :description, :status )"
    );
    query.bindValue(":title",  data[0].toString());
    query.bindValue(":deadline", data[1].toDate());
    query.bindValue(":pay", data[2].toInt());
    query.bindValue(":description", data[3].toString());
    query.bindValue(":status", data[4].toInt());

    if(!query.exec()){
            qDebug() << "Error insert into " << "Works";
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
        return false;
}

QString DataBase::getStatusName(int ID){
    QSqlQuery query;
    query.prepare("Select Name "
                  "From Status Where ID = :id");
    query.bindValue(":id", ID);
    if (!query.exec()){
        qDebug()<<query.lastError().text();
    }
    query.next();
    return query.value(0).toString();
}

bool DataBase::createTasksTable(){
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE Tasks ("
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "Responsible       INTEGER            NOT NULL,"
                            "Work              INTEGER            NOT NULL,"
                            "FOREIGN KEY  (Responsible)     REFERENCES  Users(ID),"
                            "FOREIGN KEY (Work) REFERENCES Works(ID) "
                        ")"
                    )){
        qDebug() << "DataBase: error of create " << "Tasks";
        qDebug() << query.lastError().text();
        return false;
    }
    else{
        return true;
    }
    return false;
}

bool DataBase::insertIntoTasksTable(const int userID, const int workID)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Tasks( Responsible, Work ) "
                  "   VALUES( :userID, :workID )"
    );
    query.bindValue(":userID",  userID);
    query.bindValue(":workID", workID);

    if(!query.exec()){
            qDebug() << "Error insert into " << "Tasks";
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
        return false;
}

int DataBase::getLastWorkID()
{
    QSqlQuery query;
    query.prepare("Select ID From Works");
    if (!query.exec()){
        qDebug()<<query.lastError().text();
    }
    query.last();
    return query.value(0).toInt();
}

bool DataBase::updateWorkStatus(int workID, int statusID){
    QSqlQuery query;
    query.prepare("Update Works "
                              "Set Status = :statusID "
                                 "Where Works.ID = :workID");
    query.bindValue(":statusID", statusID);
    query.bindValue(":workID", workID);
    if (!query.exec()){
        return false;
    }
    return true;
}

int DataBase::getTaskID(int workID, QString order){
    QSqlQuery query;
    query.prepare("Select ID From Tasks Where Work = :workID Order By ROWID " + order+ " LIMIT 1  ");
    query.bindValue(":workID", workID);
    query.exec();
    query.next();
    qDebug()<<query.value(0).toInt();
    return query.value(0).toInt();
}

bool DataBase::updateTaskResponsibles(int workID, int respID, int taskID){
    QSqlQuery query;
    query.prepare("Update Tasks Set Responsible = :respID "
                  " Where Work = :workID And ID = :taskID"
                  );
    query.bindValue(":respID", respID);
    query.bindValue(":workID", workID);
    query.bindValue(":taskID", taskID);
    if (!query.exec()){
        return false;
    }
    return true;
}

int DataBase::getStatus(int workID){
    QSqlQuery query;
    query.prepare("Select Status From Works Where Works.ID = :workID");
    query.bindValue(":workID", workID);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

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
        qDebug() << "Selected from table Users";
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
                            "Status                    INTEGER                  NOT NULL,"
                            "Deadline                  DATE                     NOT NULL,"
                            "Pay                       INTEGER                    NOT NULL,"
                            "Description              NVARCHAR(255)             NULL,"
                            "Responsible              INTEGER                   NOT NULL,"
                            "FOREIGN KEY  (Status)     REFERENCES  Status(ID),"
                            "FOREIGN KEY (Responsible) REFERENCES  Users(ID)"
                        ")"
                    )){
        qDebug() << "DataBase: error of create " << "Works";
        qDebug() << query.lastError().text();
        return false;
    }
    else{
        if (!query.exec("INSERT INTO Works( Title, Status, Deadline, Pay, Description, Responsible )"
                            "VALUES ( 'Тест', 1, '2024-04-04', 5500, '00', 2 )"
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
    query.prepare("INSERT INTO Works( Title, Status, Deadline, Pay, Description, Responsible ) "
                  "   VALUES( :title, :status, :deadline, :pay, :description, :responsible )"
    );
    query.bindValue(":title",  data[0].toString());
    query.bindValue(":status", data[1].toInt());
    query.bindValue(":deadline", data[2].toDate());
    query.bindValue(":pay", data[3].toInt());
    query.bindValue(":description", data[4].toString());
    query.bindValue(":responsible", data[5].toInt());

    if(!query.exec()){
            qDebug() << "Error insert into " << "Works";
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
        return false;
}



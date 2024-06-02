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
        createSalaryTable();
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
            insertIntoRolesTable("заведующий");
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

bool DataBase::createSalaryTable(){
     QSqlQuery query;
     if(!query.exec( "CREATE TABLE  IF NOT EXISTS EmployeeSalary ("
                             "ID  INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "FixedPay INTEGER  NOT NULL, "
                             "Pay      INTEGER DEFAULT 0 NOT NULL, "
                             "user_ID INTEGER NOT NULL, "
                          "CONSTRAINT S_Key FOREIGN KEY (user_ID) REFERENCES Users(ID) "
                         ")"
                     )){
         qDebug() << "DataBase: error of create " << "Salary";
         qDebug() << query.lastError().text();
         return false;
     }
     else {
         return true;
     }
     return false;
}

void DataBase::insertIntoSalaryTable(QVariantList &data){
    QSqlQuery query;
    query.prepare("INSERT INTO EmployeeSalary( FixedPay, user_ID ) "
                  "   VALUES( :pay, :userID )"
    );
    query.bindValue(":pay", data[0].toInt());
    query.bindValue(":userID", data[1].toInt());
    if( !query.exec() ) {
            qDebug() << db.lastError().text();
            qDebug()<<"??????";
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
                            "Unit                    NVARCHAR(255)            NOT NULL,"
                            "Degree                  NVARCHAR(255)            NULL,"
                            "Rank                    NVARCHAR(255)            NULL,"
                            "Post                    NVARCHAR(255)            NULL,"
                            "FOREIGN KEY  (Role)     REFERENCES  Roles(Role_ID)  "

                        ")"
                    )){
        qDebug() << "DataBase: error of create " << "Users";
        qDebug() << query.lastError().text();
        return false;
    }
    else{
        return true;
    }
    return false;
}

bool DataBase::insertIntoUsersTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Users( Role, Surname, Name, Patronymic, phoneNumber, Password, Unit ) "
                  "   VALUES( :role, :surname, :name, :patronymic, :phoneNumber, :password, :unit )"
    );
    query.bindValue(":role", 1);
    query.bindValue(":surname", data[0].toString());
    query.bindValue(":name", data[1].toString());
    query.bindValue(":patronymic", data[2].toString());
    query.bindValue(":phoneNumber", data[3].toString());
    query.bindValue(":password", data[4].toString());
    query.bindValue(":unit", data[5].toString());
//    query.bindValue(":salary", data[5].toString() == "Преподаватель" ? 50000 : 30000 );
    if(!query.exec()){
            qDebug() << "error insert into " << "Users";
            qDebug() << query.lastError().text();
            return false;
        } else {
        QVariantList salary = {data[5].toString() == "Преподаватель" ? 50000 : 30000, checkUserID(data[3].toString())};
        insertIntoSalaryTable(salary);
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

QString DataBase::getRoleName(int role){
    QSqlQuery query;
    query.prepare("Select Name "
                  "From Roles Where Role_ID = :id");
    query.bindValue(":id", role);
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


bool DataBase::finishTasks(QVariantList idList){
    QSqlQuery query;
//    QList<QVariantList> list;
    for (QVariant i : idList){
    QVariantList resps;
    int pay;
    query.prepare("Select Responsible From Tasks Where Work = :workIDs");
    query.bindValue(":workIDs", i);
    query.exec();
    while (query.next()){
        resps.append(query.value(0));
    }
    query.prepare("Select Pay From Works Where ID = :workIDs");
    query.bindValue(":workIDs", i);
    query.exec();
    query.next();
    pay = resps.count() > 1 ? query.value(0).toInt() / 2 : query.value(0).toInt();
    for ( QVariant j : resps ){
        updateUserSalary(j.toInt(), pay);
    }
}
    deleteWorks(idList);
    return true;
}


bool DataBase::updateUserSalary(const int userID, const int pay){
    QSqlQuery query;
    query.prepare("Update EmployeeSalary "
                   "Set Pay = Pay + :pay "
                    "Where user_ID = :userID ");
    query.bindValue(":userID", userID);
    query.bindValue(":pay", pay);
    if (!query.exec()){
        return false;
    }
    return true;
}


bool DataBase::deleteWorks(QVariantList idList){
    QSqlQuery query;
    query.prepare("Delete from Tasks "
                   "Where Tasks.Work = :workIDs");
    query.bindValue(":workIDs", idList);
    query.execBatch();
    query.prepare("Delete from Works "
                   "Where Works.ID = :workIDs");
    query.bindValue(":workIDs", idList);
    if (!query.execBatch()){
       return false;
    }
    return true;
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

int DataBase::checkUserID(QString login){
    QSqlQuery query;
    query.prepare("Select Users.ID "
                    "From Users Where Users.phoneNumber = :login");
    query.bindValue(":login", login);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int DataBase::getResponsible(int workID){
    QSqlQuery query;
    query.prepare("Select Tasks.Responsible "
                   "From Tasks LEFT JOIN Users On Users.ID = Tasks.Responsible "
                                "Where Tasks.Work = :workID");
    query.bindValue(":workID", workID);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

bool DataBase::updateUserRole(int userID, int role){
    QSqlQuery query;
    query.prepare("Update Users "
                   "Set Role = :roleID "
                   "Where ID = :userID");
    query.bindValue(":userID", userID);
    query.bindValue(":roleID", role);
    if (!query.exec()){
        return false;
    }
    return true;
}

bool DataBase::updateUserUnit(int userID, QString unit){
    QSqlQuery query;
    int fixedPay = unit == "Преподаватель" ? 50000 : 30000;
    query.prepare("Update Users "
                       "Set Unit = :unit "
                       "Where ID = :userID ");
        query.bindValue(":userID", userID);
        query.bindValue(":unit", unit);
    query.exec();
    query.prepare("Update EmployeeSalary "
                    "Set FixedPay = :fixedPay "
                        "Where user_ID = :userID ");
    query.bindValue(":userID", userID);
    query.bindValue(":fixedPay", fixedPay);
    query.exec();
    return true;
}

bool DataBase::updateUserPost(int userID, QString label, QString post){
    QSqlQuery query;
    query.prepare("Update Users "
                   "Set "+label+" = :post "
                   "Where ID = :userID ");
    query.bindValue(":userID", userID);
    query.bindValue(":post", post);
    if(!query.exec()){
        return false;
    }
    return true;
}

QList<QList<QVariant>> DataBase::selectAllStatus(){
    QSqlQuery query;
     QList<QList<QVariant>> list;
     query.exec("Select Status.Name, Status.ID "
                     "From Status");
     while(query.next()){
         QList<QVariant> status;
         status.append(query.value(1).toInt());
         status.append(query.value(0).toString());
         list.append(status);
     }
     return list;
}

int DataBase::getCountTasks(int workID){
    QSqlQuery query;
    query.prepare("Select Count(ID) From Tasks where Work = :workID");
    query.bindValue(":workID", workID);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

bool DataBase::deleteResponsible(int taskID){
    QSqlQuery query;
    query.prepare("Delete From Tasks "
                  "Where ID = :taskID");
    query.bindValue(":taskID", taskID);
    if(query.exec()){
        return true;
    }
    return false;
}

QList<QList<QVariant>> DataBase::selectTasks(int workID){
    QList<QList<QVariant>> list;
    QSqlQuery query;
    query.prepare("Select Title, Description, Deadline, Pay, Status, Users.Name, Users.Surname, Users.Patronymic, Tasks.Responsible "
                   " From Works "
                       "LEFT JOIN Tasks On Tasks.Work = Works.ID "
                           "LEFT JOIN Users On Tasks.Responsible = Users.ID Where Works.ID = :workID" );
    query.bindValue(":workID", workID);
    if(!query.exec()){
        qDebug()<<query.lastError().text();
    }
    while (query.next()){
        QList<QVariant> task;
        for (int i = 0; i < 9; i++){
            task.append(query.value(i));
       }
      list.append(task);
    }
   return list;
}

QList<QList<QVariant>> DataBase::selectUsers(){
    QList<QList<QVariant>> list;
    QSqlQuery query;
     query.prepare("Select Users.ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Unit, Roles.Role_ID, Degree, Rank, Post, FixedPay+Pay "
                   " From Users JOIN Roles ON Roles.Role_ID = Users.Role "
                     "JOIN EmployeeSalary ON EmployeeSalary.user_ID = Users.ID " );
     if(!query.exec()){
     }
     else {
         while (query.next()){
             QList<QVariant> user;
             for (int i = 0; i < 12; i++){
                 user.append(query.value(i));
            }
           list.append(user);
         }
     }
   return list;
}

bool DataBase::deleteUsers(QVariantList idList){
    QSqlQuery query;
    query.prepare("Delete from Tasks "
                     "Where Tasks.Responsible = :userID");
    query.bindValue(":userID", idList);
    query.execBatch();
    query.prepare("Delete from EmployeeSalary "
                     "Where user_ID = :userID");
    query.bindValue(":userID", idList);
    query.bindValue(":userID", idList);
    query.execBatch();
    query.prepare("Delete from Users "
                     "Where Users.ID = :userID");
    query.bindValue(":userID", idList);
    if(!query.execBatch()){
        return false;
    }
   return true;
}


QList<QList<QVariant>> DataBase::selectWorks(){
    QList<QList<QVariant>> list;
    QSqlQuery query;
    query.prepare("Select Works.ID, Title, Status.Name, Works.Deadline "
                   " From Works JOIN Status ON Status.ID = Works.Status " );
     if(!query.exec()){
     }
     else {
         while (query.next()){
          QList<QVariant> work;
             for (int i = 0; i < 4; i++){
                 work.append(query.value(i));
            }
           list.append(work);
         }
     }
     return list;
}

QSet<int> DataBase::selectWorksForUser(const int &userID){
    QSet<int> workIDs;
    QSqlQuery query;
    query.prepare("Select Work FROM Tasks Where Tasks.Responsible = :userID");
    query.bindValue(":userID", userID);
    if(query.exec()){
        while (query.next()){
            workIDs.insert(query.value(0).toInt());
        }
    }
    return workIDs;
}

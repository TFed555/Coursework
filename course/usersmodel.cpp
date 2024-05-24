#include "usersmodel.h"

UsersModel* UsersModel::m_instance = nullptr;

UsersModel* UsersModel::instance()
{
    if (!m_instance) {
        m_instance = new UsersModel();
    }
    return m_instance;
}

UsersModel::UsersModel(QObject* parent) : QAbstractTableModel(parent)
{
    setParent(parent);
    this->setupModel();

}

UsersModel::~UsersModel(){
    delete db;
    delete query;
}


void UsersModel::setupModel(){
    query->prepare("Select Users.ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Post, Roles.Role_ID "
                   " From Users JOIN Roles ON Roles.Role_ID = Users.Role " );
     if(!query->exec()){
         qDebug()<<query->lastError().text();
     }
     else {
         while (query->next()){
         appendUser(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString(), query->value(3).toString(),
                    query->value(4).toString(), query->value(5).toString(), query->value(6).toString(), query->value(7).toInt());
         }
     }
}

int UsersModel::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return users.count();
}

int UsersModel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return LAST;
}

QVariant UsersModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case ID:
        return trUtf8( "ID" );
    case ROLE:
        return trUtf8( "Роль" );
    case SURNAME:
        return trUtf8("Фамилия");
    case NAME:
        return trUtf8("Имя");
    case PATRONYMIC:
        return trUtf8("Отчество");
    case POST:
        return trUtf8("Должность");

}
    return QVariant();
}


QVariant UsersModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid() ||
        users.count() <= index.row() ||
        ( role != Qt::DisplayRole && role != Qt::EditRole )) {
        return QVariant();
    }

    return users[ index.row() ][ Column( index.column() ) ];
}

void UsersModel::appendUser(const int& id, const QString& role, const QString& surname, const QString& name, const QString& patronymic, const QString& phone,
                                const QString& post, const int& role_id) {
    ListData user;
    user[ ID ] = id;
    user[ ROLE ] = role;
    user[ SURNAME ] = surname;
    user[ NAME ] = name;
    user[ PATRONYMIC ] = patronymic;
    user[ PHONE ] = phone;
    user[ POST ] = post;
    user[ ROLE_ID ] = role_id;

    int row = users.count();
    beginInsertRows( QModelIndex(), row, row );
    users.append(user);
    endInsertRows();
}

void UsersModel::removeUsers(const QModelIndexList &indexes){
      QVector<int> rows;
      QVariantList idList;
      for (const QModelIndex &index : indexes) {
             int id = users[index.row()][ID].toInt();
             rows.append(id);
      }
      for (int i : rows){
          idList.append(i);
      }
      //метод в бд
        query->prepare("Delete from Tasks "
                         "Where Tasks.Responsible = :userID");
        query->bindValue(":userID", idList);
        query->execBatch();
        query->prepare("Delete from Users "
                         "Where Users.ID = :userID");
        query->bindValue(":userID", idList);
        if(!query->execBatch()){
            qDebug()<<query->lastError().text();
        }
        else {
            for (const QModelIndex &index : indexes) {
                   users.removeAt(index.row());
            }
            emit layoutChanged();
        }
}


void UsersModel::updateModel(){
    query->prepare("Select Users.ID, Roles.Name, Surname, Users.Name, Patronymic, phoneNumber, Post, Roles.Role_ID "
                   " From Users JOIN Roles ON Roles.Role_ID = Users.Role " );
     if(!query->exec()){
         qDebug()<<query->lastError().text();
     }
     else {
         query->last();
         appendUser(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString(), query->value(3).toString(),
                    query->value(4).toString(), query->value(5).toString(), query->value(6).toString(), query->value(7).toInt());
     }
}

QList<QList<QVariant>> UsersModel::getList()
{
    QList<QList<QVariant>> list;
    for (int i = 0; i < users.count(); i++){
        QList<QVariant> user;
        user.append(users[i][ID]);
        user.append(users[i][ROLE]);
        user.append(users[i][SURNAME]);
        user.append(users[i][NAME]);
        user.append(users[i][PATRONYMIC]);
        user.append(users[i][PHONE]);
        user.append(users[i][ROLE_ID]);
        user.append(users[i][POST]);
        list.append(user);
    }
    return list;
}

void UsersModel::updateUserRole(int id, int role)
{
    QString res;
    for (int i = 0;i < users.count();i++){
        if (users[i][ID] == id){
            res = db->getRoleName(role);
            users[i][ROLE] = res;
            users[i][ROLE_ID] = role;
            emit layoutChanged();
        }
}
}


void UsersModel::updateUserPost(int id, QString post)
{
    for (int i = 0;i < users.count();i++){
        if (users[i][ID] == id){
            users[i][POST] = post;
            emit layoutChanged();
        }
}
}







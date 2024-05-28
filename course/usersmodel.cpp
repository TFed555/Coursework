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
    QList<QList<QVariant>> list = db->selectUsers();
    for (int i = 0; i < list.count(); i++){
        appendUser(list[i][0].toInt(), list[i][1].toString(), list[i][2].toString(), list[i][3].toString(), list[i][4].toString(),
                list[i][5].toString(), list[i][6].toString(), list[i][7].toInt(), list[i][8].toString(), list[i][9].toString(), list[i][10].toString());
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
    case UNIT:
        return trUtf8("Должность");

}
    return QVariant();
}


//поменять вывод столбцов
QVariant UsersModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid() ||
        users.count() <= index.row() ||
        ( role != Qt::DisplayRole && role != Qt::EditRole )) {
        return QVariant();
    }

    return users[ index.row() ][ Column( index.column() ) ];
}

void UsersModel::appendUser(const int& id, const QString& role, const QString& surname, const QString& name, const QString& patronymic, const QString& phone,
                                const QString& unit, const int& role_id, const QString& degree, const QString& rank, const QString& post) {
    ListData user;
    user[ ID ] = id;
    user[ ROLE ] = role;
    user[ SURNAME ] = surname;
    user[ NAME ] = name;
    user[ PATRONYMIC ] = patronymic;
    user[ PHONE ] = phone;
    user[ UNIT ] = unit;
    user[ ROLE_ID ] = role_id;
    user[ DEGREE ] = degree;
    user[ RANK ] = rank;
    user[ POST ] = post;

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
      if(db->deleteUsers(idList)){
          for (const QModelIndex &index : indexes) {
                     users.removeAt(index.row());
                }
         emit layoutChanged();
      }
}


void UsersModel::updateModel(){
    users.clear();
    emit layoutChanged();
    QList<QList<QVariant>> list = db->selectUsers();
    for (int i = 0; i < list.count(); i++){
        appendUser(list[i][0].toInt(), list[i][1].toString(), list[i][2].toString(), list[i][3].toString(), list[i][4].toString(),
                list[i][5].toString(), list[i][6].toString(), list[i][7].toInt(), list[i][8].toString(), list[i][9].toString(), list[i][10].toString());
    }
}


//мб вложенный
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
        user.append(users[i][UNIT]);
        user.append(users[i][DEGREE]);
        user.append(users[i][RANK]);
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


void UsersModel::updateUserUnit(int id, QString unit)
{
    for (int i = 0;i < users.count();i++){
        if (users[i][ID] == id){
            users[i][UNIT] = unit;
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






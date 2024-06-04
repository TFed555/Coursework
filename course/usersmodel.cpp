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
    setupModel();

}

UsersModel::~UsersModel(){
}


void UsersModel::setupModel(){
    QList<QList<QVariant>> list = db->selectUsers();
    insertUsers(list);
}

void UsersModel::insertUsers(const QList<QList<QVariant>>& list){
    beginResetModel();
    users.clear();
    users.reserve(list.size());
    for (const auto& user : list){
        appendUser(user[0].toInt(), user[1].toString(), user[2].toString(), user[3].toString(), user[4].toString(),
                user[5].toString(), user[6].toString(), user[7].toInt(), user[8].toString(), user[9].toString(), user[10].toString(),
                    user[11].toInt());
    }
    endResetModel();
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
        return trUtf8( "Фамилия");
    case NAME:
        return trUtf8( "Имя");
    case PATRONYMIC:
        return trUtf8("Отчество");
    case UNIT:
        return trUtf8( "Отдел");
    case SALARY:
        return trUtf8( "Текущая зарплата" );

}
    return QVariant();
}


QVariant UsersModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid() ||
        users.count() <= index.row() ||
        ( role != Qt::DisplayRole && role != Qt::EditRole )) {
        return QVariant();
    }

    int column = index.column();
        switch (column) {
        case ID:
        case ROLE:
        case SURNAME:
        case NAME:
        case PATRONYMIC:
        case UNIT:
        case SALARY:
            return users[index.row()][Column(column)];
        default:
            return QVariant();
        }

//    return users[ index.row() ][ Column( index.column() ) ];
}

void UsersModel::appendUser(const int& id, const QString& role, const QString& surname, const QString& name, const QString& patronymic, const QString& phone,
                                const QString& unit, const int& role_id, const QString& degree, const QString& rank, const QString& post, const int& salary) {
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
    user[ SALARY ] = salary;

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
    setupModel();
}



QList<QList<QVariant>> UsersModel::getList()
{
    QList<QList<QVariant>> list;
    for (const auto& user : users){
        QList<QVariant> userData;
        for (int i = 0; i < LAST; ++i){
            userData.append(user[Column(i)]);
        }
        list.append(userData);
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
    for (auto& user : users){
        if (user[ID] == id){
            user[UNIT] = unit;
            emit layoutChanged();
        }
}
}

void UsersModel::updateUserPost(int id, QString post)
{
    for (auto& user : users){
        if (user[ID] == id){
            user[POST] = post;
            emit layoutChanged();
        }
}
}







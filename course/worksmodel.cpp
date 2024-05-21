#include "worksmodel.h"

WorksModel::WorksModel(QObject* parent) : QAbstractTableModel(parent)
{
    setParent(parent);
    this->setupModel();

}

//WorksModel::~WorksModel(){

//}

void WorksModel::updateAddWork(){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
//     query->prepare("Select Works.ID, Title, Description, Deadline, Pay, Status.Name, "
//                    "Users.Name, Users.Surname, Users.Patronymic "
//                    " From Works JOIN Status ON Status.ID = Works.Status "
//                            " LEFT  JOIN Tasks On Tasks.Work = Works.ID "
//                                  "LEFT JOIN Users On Users.ID = Tasks.Responsible" );
    query->prepare("Select Works.ID, Title, Status.Name "
                   " From Works JOIN Status ON Status.ID = Works.Status " );
     if(!query->exec()){
         qDebug()<<query->lastError().text();
     }
     else {
         query->last();
         //QString responsible = query->value(6).toString()+" "+query->value(7).toString()+" "+query->value(8).toString();
         appendWork(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString());
     }
}

void WorksModel::setupModel(){
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
    query->prepare("Select Works.ID, Title, Status.Name "
                   " From Works JOIN Status ON Status.ID = Works.Status " );
     if(!query->exec()){
         qDebug()<<query->lastError().text();
     }
     else {
         while (query->next()){
         //QString responsible = query->value(6).toString()+" "+query->value(7).toString()+" "+query->value(8).toString();
         appendWork(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString());
         }
     }
}

int WorksModel::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return works.count();
}

int WorksModel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return LAST;
}

QVariant WorksModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case ID:
        return trUtf8( "ID" );
    case TITLE:
        return trUtf8( "Дополнительная работа" );
//    case DESC:
//        return trUtf8( "Описание" );
//    case DEADLINE:
//        return trUtf8( "Срок" );
//    case PAY:
//        return trUtf8( "Оплата" );
    case STATUS:
        return trUtf8("Статус");
//    case RESPONSIBLE:
//        return trUtf8("Ответственный");
}
    return QVariant();
}


QVariant WorksModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid() ||
        works.count() <= index.row() ||
        ( role != Qt::DisplayRole && role != Qt::EditRole )) {
        return QVariant();
    }

    return works[ index.row() ][ Column( index.column() ) ];
}


void WorksModel::appendWork(const int& id, const QString& title,
                            const QString& status ) {
    ListData work;
    work[ ID ] = id;
    work[ TITLE ] = title;
//    work[ DESC ] = desc;
//    work[ DEADLINE ] = deadline;
//    work[ PAY ] = pay;
    work[ STATUS ] = status;
//    work[ RESPONSIBLE ] = responsible;

    int row = works.count();
    beginInsertRows( QModelIndex(), row, row );
    works.append(work);
    endInsertRows();
}

void  WorksModel::updateWorkStatus(int id, int status)
{
    QString res;
    for (int i = 0;i < works.count();i++){
        if (works[i][ID] == id){
            res = db->getStatusName(status);
            works[i][STATUS] = res;
            emit layoutChanged();
        }
    }
    //emit dataChanged(createIndex(5, 5, this));

}

void WorksModel::removeWorks(const QModelIndexList &indexes){
      QVector<int> rows;
      QVariantList idList;
      for (const QModelIndex &index : indexes) {
             int id = works[index.row()][ID].toInt();
             rows.append(id);
      }
      for (int i : rows){
          idList.append(i);
      }
        DataBase conn;
        QSqlQuery* query = new QSqlQuery(conn.db);
        query->prepare("Delete from Tasks "
                       "Where Tasks.Work = :workID");
        query->bindValue(":workID", idList);
        if(!query->execBatch()){
            qDebug()<<query->lastError().text();
        }
        query->prepare("Delete from Works "
                       "Where Works.ID = :workID");
        query->bindValue(":workID", idList);
        if (!query->execBatch()){
            qDebug()<<query->lastError().text();
        }
        else {
            for (const QModelIndex &index : indexes) {
                //beginRemoveRows(QModelIndex(), index, index);
                   works.removeAt(index.row());
            }
            emit layoutChanged();
        }
}




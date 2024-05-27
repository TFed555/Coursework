#include "worksmodel.h"

WorksModel* WorksModel::m_instance = nullptr;

WorksModel* WorksModel::instance()
{
    if (!m_instance) {
        m_instance = new WorksModel();
    }
    return m_instance;
}

WorksModel::WorksModel(QObject* parent) : QAbstractTableModel(parent)
{
    setParent(parent);
    this->setupModel();

}

WorksModel::~WorksModel(){
    delete db;
    delete query;
}

void WorksModel::updateAddWork(){
    works.clear();
    emit layoutChanged();
    QList<QList<QVariant>> list = db->selectWorks();
    for (int i = 0; i < list.count(); i++){
        appendWork(list[i][0].toInt(), list[i][1].toString(), list[i][2].toString(), list[i][3].toDate());
    }
//    query->prepare("Select Works.ID, Title, Status.Name, Works.Deadline "
//                   " From Works JOIN Status ON Status.ID = Works.Status " );
//     if(!query->exec()){
//         qDebug()<<query->lastError().text();
//     }
//     else {
//         query->last();
//         appendWork(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString(), query->value(3).toDate());
//     }
}

void WorksModel::setupModel(){
    QList<QList<QVariant>> list = db->selectWorks();
    for (int i = 0; i < list.count(); i++){
        appendWork(list[i][0].toInt(), list[i][1].toString(), list[i][2].toString(), list[i][3].toDate());
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
    case STATUS:
        return trUtf8("Статус");
    case DEADLINE:
         return trUtf8("Срок");

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
                            const QString& status, const QDate& deadline ) {
    ListData work;
    work[ ID ] = id;
    work[ TITLE ] = title;
    work[ STATUS ] = status;
    work[ DEADLINE ] = deadline;

    int row = works.count();
    beginInsertRows( QModelIndex(), row, row );
    works.append(work);
    endInsertRows();
}

void WorksModel::updateWorkStatus(int id, int status)
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
      if(db->deleteWorks(idList)){
            for (const QModelIndex &index : indexes) {
                //beginRemoveRows(QModelIndex(), index, index);
                   works.removeAt(index.row());
            }
            emit layoutChanged();
    }
}

QList<QList<QVariant>> WorksModel::getList()
{
    QList<QList<QVariant>> list;
    for (int i = 0; i < works.count(); i++){
        QList<QVariant> work;
        work.append(works[i][ID]);
        work.append(works[i][TITLE]);
        work.append(works[i][STATUS]);
        list.append(work);
    }
    return list;
}



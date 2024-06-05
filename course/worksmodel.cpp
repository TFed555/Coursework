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

}

void WorksModel::updateAddWork(){
    setupModel();
}

void WorksModel::setupModel(){
    QList<QList<QVariant>> list = db->selectWorks();
    insertWorks(list);
}

void WorksModel::insertWorks(const QList<QList<QVariant>> &list){
    beginResetModel();
    works.clear();
    works.reserve(list.size());
    for (const auto& work : list){
       appendWork(work[0].toInt(), work[1].toString(), work[2].toString(), work[3].toDate());
    }
    endResetModel();
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
        return trUtf8( "Статус");
    case DEADLINE:
         return trUtf8( "Срок");
    default:
        return QVariant();
    }
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
            emit dataChanged(index(i, STATUS), index(i, STATUS));
        }
    }

}


QVariantList WorksModel::getIdList(const QModelIndexList &indexes){
    QVector<int> rows;
    QVariantList idList;
    for (const QModelIndex &index : indexes) {
           int id = works[index.row()][ID].toInt();
           rows.append(id);
    }
    for (int i : rows){
        idList.append(i);
    }
    return idList;
}

void WorksModel::removeWorks(QList<int> sourceRows){
    std::sort(sourceRows.rbegin(), sourceRows.rend());

    QVariantList idList;
    for (int row : sourceRows) {
        idList.append(works[row][ID]);
    }

    if (db->deleteWorks(idList)) {
        beginResetModel();
        for (int row : sourceRows) {
            works.removeAt(row);
        }
        endResetModel();
        emit layoutChanged();
    }
}

void WorksModel::finishWorks(QList<int> sourceRows){
    std::sort(sourceRows.rbegin(), sourceRows.rend());

    QVariantList idList;
    for (int row : sourceRows) {
        idList.append(works[row][ID]);
    }

    if (db->finishTasks(idList)) {
        beginResetModel();
        for (int row : sourceRows) {
            works.removeAt(row);
        }
        endResetModel();
        emit layoutChanged();
    }
}


QList<QList<QVariant>> WorksModel::getList()
{
    QList<QList<QVariant>> list;
    for (const auto& work : works){
        QList<QVariant> workData;
        for (int i = 0; i < LAST; ++i){
            workData.append(work[Column(i)]);
        }
        list.append(workData);
    }
    return list;
}



#include "tablemodel.h"

tablemodel::tablemodel(QObject* parent)
{
    setParent(parent);

    //db = new DataBase;
    //db->connectToDataBase();
    DataBase conn;
    QSqlQuery* query = new QSqlQuery(conn.db);
     query->prepare("Select Works.ID, Title, Description, Deadline, Pay, Status.Name, Users.Name, Users.Surname, Users.Patronymic "
                    " From Works JOIN Status ON Status.ID = Works.Status "
                    " JOIN Users ON Users.ID = Works.Responsible" );
     if(!query->exec()){
         qDebug()<<"err";
     }
     else {
         while (query->next()){
         //query->next();
         QString responsible = query->value(6).toString()+" "+query->value(7).toString()+" "+query->value(8).toString();
//         qDebug()<<responsible<<" ";
         appendPerson(query->value(0).toString(), query->value(1).toString(), query->value(2).toString(),
                      query->value(3).toString(), query->value(4).toString(), query->value(5).toString(),
                      responsible);
             qDebug()<<" 0";
         }
     }
}

int tablemodel::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return m_persons.count();
}

int tablemodel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return LAST;
}

QVariant tablemodel::headerData( int section, Qt::Orientation orientation, int role ) const {
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
        return trUtf8( "Title" );
    case DESC:
        return trUtf8( "Description" );
    case DEADLINE:
        return trUtf8( "Deadline" );
    case PAY:
        return trUtf8( "Pay" );
    case STATUS:
        return trUtf8("Status");
    case RESPONSIBLE:
        return trUtf8("Responsible");
}
    return QVariant();
}


QVariant tablemodel::data( const QModelIndex& index, int role ) const {
    if(
        !index.isValid() ||
        m_persons.count() <= index.row() ||
        ( role != Qt::DisplayRole && role != Qt::EditRole )
    ) {
        return QVariant();
    }

    return m_persons[ index.row() ][ Column( index.column() ) ];
}


void tablemodel::appendPerson( const QString& id, const QString& title, const QString& desc,
                               const QString& deadline, const QString& pay, const QString& status, const QString& responsible ) {
    PersonData person;
    person[ ID ] = id;
    person[ TITLE ] = title;
    person[ DESC ] = desc;
    person[ DEADLINE ] = deadline;
    person[ PAY ] = pay;
    person[ STATUS ] = status;
    person[ RESPONSIBLE ] = responsible;

    int row = m_persons.count();
    beginInsertRows( QModelIndex(), row, row );
    m_persons.append( person );
    endInsertRows();
}

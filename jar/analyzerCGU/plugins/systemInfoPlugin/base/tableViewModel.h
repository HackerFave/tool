#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H
#include <QAbstractItemModel>

class tableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit tableViewModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setHorizontalHeader(const QStringList& headers);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(const QVector<QStringList>& data);
    QVector<QStringList>& DataVector() {return m_data;}
    ~tableViewModel(void);

signals:

public slots:

private:
    QStringList m_HorizontalHeader;
    QVector<QStringList> m_data;

};

#endif // TABLEVIEWMODEL_H

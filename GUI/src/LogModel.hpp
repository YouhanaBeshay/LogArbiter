#pragma once
#include <QAbstractListModel>
#include <QList>
#include <QSet>
#include <QDebug>

struct LogEntry {
    QString timestamp;
    QString level;
    QString source;
    QString message;
    
    QString uniqueKey() const {
        return timestamp + "|" + level + "|" + source + "|" + message;
    }
};

class LogModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int infoCount READ infoCount NOTIFY countChanged)
    Q_PROPERTY(int warningCount READ warningCount NOTIFY countChanged)
    Q_PROPERTY(int criticalCount READ criticalCount NOTIFY countChanged)

public:
    enum Roles {
        TimestampRole = Qt::UserRole + 1,
        LevelRole,
        SourceRole,
        MessageRole,
    };

    explicit LogModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int infoCount() const;
    int warningCount() const;
    int criticalCount() const;
    int count() const;

public slots:
    void addEntry(const QString &timestamp, const QString &level,
                  const QString &source, const QString &message);
    void clear();

signals:
    void countChanged();

private:
    QList<LogEntry> entries_;
    QSet<QString> seenEntries_;
};
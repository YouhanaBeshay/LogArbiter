#include "LogModel.hpp"

LogModel::LogModel(QObject *parent)
    : QAbstractListModel(parent)
{
    qDebug() << "[LogModel] Created";
}

int LogModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return entries_.count();
}

int LogModel::infoCount() const {
    int count = 0;
    for (const auto &entry : entries_) {
        if (entry.level == "INFO") count++;
    }
    return count;
}

int LogModel::warningCount() const {
    int count = 0;
    for (const auto &entry : entries_) {
        if (entry.level == "WARNING") count++;
    }
    return count;
}

int LogModel::criticalCount() const {
    int count = 0;
    for (const auto &entry : entries_) {
        if (entry.level == "CRITICAL") count++;
    }
    return count;
}

QVariant LogModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= entries_.count())
        return QVariant();

    const LogEntry &entry = entries_.at(index.row());

    switch (role) {
        case TimestampRole: return entry.timestamp;
        case LevelRole: return entry.level;
        case SourceRole: return entry.source;
        case MessageRole: return entry.message;
        default: return QVariant();
    }
}

QHash<int, QByteArray> LogModel::roleNames() const {
    return {
        {TimestampRole, "timestamp"},
        {LevelRole, "level"},
        {SourceRole, "source"},
        {MessageRole, "message"},
    };
}

int LogModel::count() const {
    return entries_.count();
}

void LogModel::addEntry(const QString &timestamp, const QString &level,
                        const QString &source, const QString &message) {
    // Create unique key
    QString key = timestamp + "|" + level + "|" + source + "|" + message;
    
    // Check for duplicate
    if (seenEntries_.contains(key)) {
        qDebug() << "[LogModel] Duplicate skipped:" << timestamp << level;
        return;
    }
    
    qDebug() << "[LogModel] Adding:" << level << source << message.left(30);
    
    seenEntries_.insert(key);
    
    beginInsertRows(QModelIndex(), entries_.count(), entries_.count());
    
    LogEntry entry;
    entry.timestamp = timestamp;
    entry.level = level;
    entry.source = source;
    entry.message = message;
    entries_.append(entry);
    
    endInsertRows();
    
    emit countChanged();
}

void LogModel::clear() {
    qDebug() << "[LogModel] Clearing";
    beginResetModel();
    entries_.clear();
    seenEntries_.clear();
    endResetModel();
    emit countChanged();
}
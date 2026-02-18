#include "LogFilterProxyModel.hpp"
#include "LogModel.hpp"

LogFilterProxyModel::LogFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent), filterLevel_("ALL"), logModel_(nullptr) {
  connect(this, &QSortFilterProxyModel::rowsInserted, this,
          &LogFilterProxyModel::visibleCountChanged);
  connect(this, &QSortFilterProxyModel::rowsRemoved, this,
          &LogFilterProxyModel::visibleCountChanged);
  connect(this, &QSortFilterProxyModel::modelReset, this,
          &LogFilterProxyModel::visibleCountChanged);
}

void LogFilterProxyModel::setSourceModel(QAbstractItemModel *sourceModel) {
  if (logModel_) {
    disconnect(logModel_, nullptr, this, nullptr);
  }

  QSortFilterProxyModel::setSourceModel(sourceModel);
  logModel_ = qobject_cast<LogModel *>(sourceModel);

  if (logModel_) {
    connect(logModel_, &LogModel::countChanged, this,
            &LogFilterProxyModel::countChanged);
    connect(logModel_, &LogModel::countChanged, this,
            &LogFilterProxyModel::countsChanged);
    emit countChanged();
    emit countsChanged();
  }
}

int LogFilterProxyModel::count() const {
  return logModel_ ? logModel_->count() : 0;
}

int LogFilterProxyModel::infoCount() const {
  return logModel_ ? logModel_->infoCount() : 0;
}

int LogFilterProxyModel::warningCount() const {
  return logModel_ ? logModel_->warningCount() : 0;
}

int LogFilterProxyModel::criticalCount() const {
  return logModel_ ? logModel_->criticalCount() : 0;
}

void LogFilterProxyModel::clear() {
  if (logModel_) {
    logModel_->clear();
  }
}
void LogFilterProxyModel::setFilterLevel(const QString &level) {
  if (filterLevel_ != level) {
    filterLevel_ = level;
    qDebug() << "[LogFilterProxy] Filter level changed to:" << level;
    emit filterLevelChanged();
    invalidateFilter();
    emit visibleCountChanged();
  }
}

void LogFilterProxyModel::setSearchText(const QString &text) {
  if (searchText_ != text) {
    searchText_ = text;
    qDebug() << "[LogFilterProxy] Search text changed to:" << text;
    emit searchTextChanged();
    invalidateFilter();
    emit visibleCountChanged();
  }
}

bool LogFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (!sourceModel()) {
    return true;
  }

  QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

  QString level = sourceModel()->data(index, LogModel::LevelRole).toString();

  // Filter by level
  if (!filterLevel_.isEmpty() && filterLevel_ != "ALL") {
    if (filterLevel_ == "WARNING+") {
      if (level != "WARNING" && level != "CRITICAL") {
        return false;
      }
    } else if (level != filterLevel_) {
      return false;
    }
  }

  // Filter by search text
  if (!searchText_.isEmpty()) {
    QString timestamp =
        sourceModel()->data(index, LogModel::TimestampRole).toString();
    QString source =
        sourceModel()->data(index, LogModel::SourceRole).toString();
    QString message =
        sourceModel()->data(index, LogModel::MessageRole).toString();

    QString searchLower = searchText_.toLower();
    bool matches = timestamp.toLower().contains(searchLower) ||
                   level.toLower().contains(searchLower) ||
                   source.toLower().contains(searchLower) ||
                   message.toLower().contains(searchLower);

    qDebug() << "[LogFilterProxy] Row" << sourceRow
             << "search match:" << matches;
    return matches;
  }

  return true;
}
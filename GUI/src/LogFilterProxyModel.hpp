#pragma once

#include <QSortFilterProxyModel>

class LogModel; // Forward declaration

class LogFilterProxyModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_PROPERTY(QString filterLevel READ filterLevel WRITE setFilterLevel NOTIFY
                 filterLevelChanged)
  Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY
                 searchTextChanged)
  Q_PROPERTY(int visibleCount READ visibleCount NOTIFY visibleCountChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)
  Q_PROPERTY(int infoCount READ infoCount NOTIFY countsChanged)
  Q_PROPERTY(int warningCount READ warningCount NOTIFY countsChanged)
  Q_PROPERTY(int criticalCount READ criticalCount NOTIFY countsChanged)

public:
  explicit LogFilterProxyModel(QObject *parent = nullptr);

  QString filterLevel() const { return filterLevel_; }
  void setFilterLevel(const QString &level);

  QString searchText() const { return searchText_; }
  void setSearchText(const QString &text);

  int visibleCount() const { return rowCount(); }
  int count() const;
  int infoCount() const;
  int warningCount() const;
  int criticalCount() const;

  void setSourceModel(QAbstractItemModel *sourceModel) override;

  Q_INVOKABLE void clear();

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

signals:
  void filterLevelChanged();
  void searchTextChanged();
  void visibleCountChanged();
  void countChanged();
  void countsChanged();

private:
  QString filterLevel_;
  QString searchText_;
  LogModel *logModel_;
};
#ifndef FUNCTIONSWIDGET_H
#define FUNCTIONSWIDGET_H

#include <memory>

#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QDockWidget>

#include "cutter.h"

class MainWindow;
class QTreeWidgetItem;

namespace Ui
{
    class FunctionsWidget;
}


class FunctionModel : public QAbstractItemModel
{
    Q_OBJECT

private:
    QList<FunctionDescription> *functions;
    QSet<RVA> *import_addresses;


    QFont highlight_font;
    QFont default_font;
    bool nested;

    int current_index;

    bool functionIsImport(ut64 addr) const;

public:
    static const int FunctionDescriptionRole = Qt::UserRole;
    static const int IsImportRole = Qt::UserRole + 1;

    enum Column { NameColumn = 0, SizeColumn, ImportColumn, OffsetColumn, ColumnCount };

    FunctionModel(QList<FunctionDescription> *functions, QSet<RVA> *import_addresses, bool nested, QFont default_font, QFont highlight_font, QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void beginReloadFunctions();
    void endReloadFunctions();

    /*!
     * @return true if the index changed
     */
    bool updateCurrentIndex();

    void setNested(bool nested);
    bool isNested()                 { return nested; }

private slots:
    void seekChanged(RVA addr);
    void functionRenamed(const QString &prev_name, const QString &new_name);
};


class FunctionSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FunctionSortFilterProxyModel(FunctionModel *source_model, QObject *parent = 0);

protected:
    bool filterAcceptsRow(int row, const QModelIndex &parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};



class FunctionsWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit FunctionsWidget(MainWindow *main, QWidget *parent = 0);
    ~FunctionsWidget();

private slots:
    void functionsTreeView_doubleClicked(const QModelIndex &index);
    void showFunctionsContextMenu(const QPoint &pt);

    void on_actionDisasAdd_comment_triggered();
    void on_actionFunctionsRename_triggered();
    void on_action_References_triggered();
    void on_actionFunctionsUndefine_triggered();

    void on_actionHorizontal_triggered();
    void on_actionVertical_triggered();

    void showTitleContextMenu(const QPoint &pt);

    void refreshTree();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<Ui::FunctionsWidget> ui;
    MainWindow      *main;

    QList<FunctionDescription> functions;
    QSet<RVA> import_addresses;

    FunctionModel *function_model;
    FunctionSortFilterProxyModel *function_proxy_model;

    void setScrollMode();
};





#endif // FUNCTIONSWIDGET_H

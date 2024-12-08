#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getData();
    QString aranankelime;

    void setVisibility();
private slots:
    void on_baslabuton_clicked();

    void on_guessbuton_clicked();

    void on_add_clicked();

    void on_update_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_delete_2_clicked();

private:
    Ui::MainWindow *ui;
    QSqlQuery *sorgu, *insertsorgu, *updatesorgu, *deletesorgu;
    QSqlQueryModel *model;
    int hatasayac = 0;
};
#endif // MAINWINDOW_H

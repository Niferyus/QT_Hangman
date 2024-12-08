#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setVisible(false);
    setVisibility();
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/varis/OneDrive/Desktop/dbadamasmaca.db");
    if(!database.open()){
        QMessageBox::critical(this,"Bağlantı Hatası",database.lastError().text());
    }
    sorgu = new QSqlQuery(database);
    insertsorgu = new QSqlQuery(database);
    updatesorgu = new QSqlQuery(database);
    deletesorgu = new QSqlQuery(database);
    model = new QSqlQueryModel();
    getData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getData()
{
    bool kontrol = sorgu->exec("select * from kelimeler");
    if(!kontrol){
        QMessageBox::critical(this,"Bağlanıt Hatası",sorgu->lastError().text());

    }
    model->setQuery(*sorgu);
    ui->tableView->setModel(model);
}

void MainWindow::on_baslabuton_clicked()
{
    hatasayac = 0;
    setVisibility();
    ui->listWidget->clear();
    ui->harftahmin->clear();
    int gecici = 0;
    while (true) {
        QString labelName = "label" + QString::number(gecici);
            QLabel *label = findChild<QLabel *>(labelName);
            if (label) {
                label->deleteLater();
                gecici++;
            }
            else{
                break;
            }
    }
    hatasayac = 0;
    int X = ui->harftahmin->x();
    int Y = ui->harftahmin->y();
    int maxindex = model->rowCount();
    int randomrow = QRandomGenerator::global()->bounded(maxindex);
    aranankelime = model->index(randomrow,1).data().toString();
    for (int i=0;i < aranankelime.length();i++)
    {
        QLabel *label = new QLabel(this);
        label->setObjectName("label" + QString::number(i));
        label->setText("_");
        QFont font = label->font();
        font.setPointSize(20);
        label->setFont(font);
        label->setGeometry(X,Y-100,40,40);
        label->show();
        X += 30;
    }

}

void MainWindow::on_guessbuton_clicked()
{
    if(aranankelime == ui->harftahmin->text()){
        for (int i=0;i<aranankelime.length() ;i++ ) {
            QString labelName = "label" + QString::number(i);
                QLabel *label = findChild<QLabel *>(labelName);
                if (label) {
                    QChar harf = aranankelime.at(i);
                    label->setText(harf);
                }
        }
        QMessageBox::information(this,"Bravo","Kelimeyi bildiniz");
        return;
    }
    QList<int> indexs;
    QString harf = ui->harftahmin->text();
    ui->harftahmin->setText("");
    if(harf.isEmpty() || !harf.at(0).isLetter()){
        QMessageBox::critical(this,"Harf hatası","Harf giriniz");
    }
    else{
        if(aranankelime.contains(harf))
        {
            int index = 0;
            for (int i=0;i<aranankelime.count(harf) ;i++ ) {
                indexs.append(aranankelime.indexOf(harf,index));
                index = aranankelime.indexOf(harf)+1;
            }
            for(int i=0;i<indexs.size();i++){
                QString labelName = "label" + QString::number(indexs[i]);
                    QLabel *label = findChild<QLabel *>(labelName);
                    if (label) {
                        label->setText(harf);
                    }
            }
            bool kontrol = true;
            int bos = 0;
            for (int i=0;i<aranankelime.length() ;i++ ) {
                QString labelName = "label" + QString::number(i);
                    QLabel *label = findChild<QLabel *>(labelName);
                    if(label->text() == "_"){
                        bos++;
                    }

            }
            if(bos == 0){
                QMessageBox::information(this,"Bravo","Kelimeyi bildiniz");
            }

        }
        else {
            hatasayac++;
            ui->listWidget->addItem(harf);
            switch (hatasayac) {
            case 1:
                ui->cizgi_1->setVisible(true);
                break;
            case 2:
                ui->cizgi_2->setVisible(true);
                break;

            case 3:
                ui->cizgi_3->setVisible(true);
                break;

            case 4:
                ui->cizgi_4->setVisible(true);
                break;

            case 5:
                ui->kafa->setVisible(true);
                break;

            case 6:
                ui->govde->setVisible(true);
                break;

            case 7:
                ui->kolsag->setVisible(true);
                break;

            case 8:
                ui->kolsol->setVisible(true);
                break;

            case 9:
                ui->bacaksag->setVisible(true);
                break;

            case 10:
                ui->bacaksol->setVisible(true);
                QMessageBox::critical(this,"Oyun Bitti","Kelimeyi bilemediniz");
                ui->listWidget->clear();
                break;

            }
        }
    }
}

void MainWindow::setVisibility(){
    ui->cizgi_1->setVisible(false);
    ui->cizgi_2->setVisible(false);
    ui->cizgi_3->setVisible(false);
    ui->cizgi_4->setVisible(false);
    ui->bacaksag->setVisible(false);
    ui->bacaksol->setVisible(false);
    ui->kafa->setVisible(false);
    ui->govde->setVisible(false);
    ui->kolsag->setVisible(false);
    ui->kolsol->setVisible(false);
}

void MainWindow::on_add_clicked()
{
    insertsorgu->prepare("insert into kelimeler (kelime) values(?)");
    insertsorgu->addBindValue(ui->newword->text());
    if(!insertsorgu->exec()){
        QMessageBox::critical(this, "Bağlantı Hatası", insertsorgu->lastError().text());
    }
    getData();
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QString text = model->index(index.row(),1).data().toString();

    ui->newword->setText(text);
    ui->lineEdit->setText(model->index(index.row(),0).data().toString());
}


void MainWindow::on_update_clicked()
{
    updatesorgu->prepare("update kelimeler set kelime=? where kelimeNo = ?");
    updatesorgu->addBindValue(ui->newword->text());
    updatesorgu->addBindValue(ui->lineEdit->text().toInt());
    if (!updatesorgu->exec()) {
        QMessageBox::critical(this, "Bağlantı Hatası", updatesorgu->lastError().text());
    }
    getData();
}


void MainWindow::on_delete_2_clicked()
{
    deletesorgu->prepare("delete from kelimeler where kelimeNo = ?");
    deletesorgu->addBindValue(ui->lineEdit->text());
    if (!deletesorgu->exec()) {
        QMessageBox::critical(this, "Bağlantı Hatası", deletesorgu->lastError().text());
    }
    getData();
}


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Window->setReadOnly(true);
    setWindowTitle("Чат Клиент");


    connect(ui->Quit, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->Send, SIGNAL(clicked()), this, SLOT(sendMessage()));

    tcpClient.connectToHost(QHostAddress::LocalHost, 11111);

    connect(&tcpClient, SIGNAL(readyRead()), this, SLOT(getMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
    tcpClient.close();
}

void MainWindow::getMessage() {
    ui->Window->appendPlainText(tcpClient.readAll());
}
void MainWindow::sendMessage() {

    QString nm = ui->Name->text();
    QString txt = ui->Text->text();
    if(nm == "" || txt == "") {
        QMessageBox::critical(this, "!!!!!", "Message Error !!");
        return;
    }
    QByteArray data;
    data.append(nm + ": " + txt);
    tcpClient.write(data);
}

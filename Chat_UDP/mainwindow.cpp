#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->Window->setReadOnly(true);
    setWindowTitle("Чат");
    udpSocket = new QUdpSocket(this);
    if(! udpSocket->bind(12321, QUdpSocket::ShareAddress))
        QMessageBox::critical(this, "!!!!!", "Bind Error !!");

    connect(ui->Send, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(ui->Quit, SIGNAL(clicked()), this, SLOT(close()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(getMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete udpSocket;
}

void MainWindow::sendMessage()
{
    QString nm = ui->Name->text();
    QString txt = ui->Text->text();
    if(nm == "" || txt == "") {
        QMessageBox::critical(this, "!!!!!", "Message Error !!");
        return;
    }
    QByteArray data;
    data.append(nm + ": " + txt);
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, 12321);
}

void MainWindow::getMessage()
{
    QByteArray data;
    while (udpSocket->hasPendingDatagrams()) {
        data.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(data.data(), data.size());
        ui->Window->appendPlainText(data.constData());
    }
}




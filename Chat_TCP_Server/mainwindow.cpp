#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , amount_messages(10)
{
    ui->setupUi(this);
    setWindowTitle("Чат Сервер");
        connect(ui->Quit, SIGNAL(clicked()), this, SLOT(close()));

    if(!tcpServer.isListening() && !tcpServer.listen(QHostAddress::LocalHost, 11111)) {
        QMessageBox::critical(this, "!!!!!", "Listen Error !!");
        return;
    }
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
    tcpServer.close();
}

void MainWindow::acceptConnection()
{
    QTcpSocket * con = tcpServer.nextPendingConnection();
    if (!con) {
        QMessageBox::critical(this, "!!!!!", "Connect Error!!");
        return;
    }
    connect(con, SIGNAL(readyRead()), this, SLOT(getMessage()));
    connect(con, SIGNAL(disconnected()), this, SLOT(disConnection()));
    connection.push_back(con);
    con->write(getAllData());
}

void MainWindow::disConnection()
{
    QTcpSocket * soc = qobject_cast<QTcpSocket *>(sender());
    connection.removeOne(soc);
}

void MainWindow::getMessage()
{
    QTcpSocket * soc = qobject_cast<QTcpSocket *>(sender());
    messages.push_back(soc->readAll());
    if(messages.size() > amount_messages)
        messages.removeFirst();
    sendData();
}

QByteArray MainWindow::getAllData()
{
    QByteArray data;
    data.append(messages.join("\n"));
    return data;
}

void MainWindow::sendData()
{
    QByteArray data;
    data.append(messages.last());
    for(int i=0; i<connection.size(); ++i) {
        if(connection[i]->isValid())
            connection[i]->write(data);
    }
}



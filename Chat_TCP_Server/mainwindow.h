#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpServer tcpServer;
    QList<QTcpSocket *> connection;
    QStringList messages;
    int amount_messages;
private slots:
    void acceptConnection();
    void disConnection();
    void getMessage();
private:
    QByteArray getAllData();
    void sendData();
};
#endif // MAINWINDOW_H

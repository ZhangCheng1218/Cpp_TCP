#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"recvfile.h"
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setText("8999");
     m_s=new QTcpServer(this);

     connect(m_s,&QTcpServer::newConnection,[=](){
         QTcpSocket *tcp =m_s->nextPendingConnection();
         //创建子线程
         RecvFile * subThread=new RecvFile(tcp);
         subThread->start();
         //接收子线程的over信号
         connect(subThread,&RecvFile::over,[=](){

             subThread->exit();
             subThread->wait();
             subThread->deleteLater();
             QMessageBox::information(this,"FileMessage","Receive Over!");

         });
     });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnListen_clicked()
{
    unsigned short port=ui->port->text().toUShort();

    m_s->listen(QHostAddress::Any,port);
}

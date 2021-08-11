#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QThread>
#include"sendfile.h"
#include<QMessageBox>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ip->setText("127.0.0.1");
    ui->port->setText("8999");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);


    //创建子线程
    QThread * t=new QThread;
    //创建工作对象
    SendFile * worker=new SendFile;
    //工作对象放入线程
    worker->moveToThread(t);
    //收到连接信号
    connect(this,&MainWindow::Connect2Server,worker,&SendFile::connectServer);
    //接收发送文件信号
    connect(this,&MainWindow::sendFile,worker,&SendFile::sendFile);    
    //处理子线程发的信号,连接成功
    connect(worker,&SendFile::connectOK,this,[=](){
        QMessageBox::information(this,"ConnectStatus","Connection successed!");
    });
    //处理子线程发的信号，断开连接
    connect(worker,&SendFile::gameover,this,[=](){
        //释放资源
        t->quit();
        t->wait();
        worker->deleteLater();
        t->deleteLater();
    });
    //接收进度条百分比信号
    connect(worker,&SendFile::curpercent,ui->progressBar,&QProgressBar::setValue);
    t->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//点击连接按钮槽函数
void MainWindow::on_btn_connect_clicked()
{
    QString ip=ui->ip->text();
    unsigned short port =ui->port->text().toUShort();
    emit MainWindow::Connect2Server(ip,port);

}
//发送文件
void MainWindow::on_pushButton_clicked()
{
    emit sendFile(ui->filePath->text());
}
//选择文件
void MainWindow::on_btn_selectFile_clicked()
{
    QString path=QFileDialog::getOpenFileName();
    ui->filePath->setText(path);

}

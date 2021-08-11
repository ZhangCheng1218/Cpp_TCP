#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ServerPort->setText("8899");
    ui->ServerIP->setText("127.0.0.1");
    ui->btn_Disconnect->setEnabled(false);
    setWindowTitle("Client");
    //创建client对象
    m_tcp=new QTcpSocket(this);
    //接受数据
    connect(m_tcp,&QTcpSocket::readyRead,[=](){

        QByteArray data= m_tcp->readAll();
        ui->record->append("Server:"+data);
    });
    //断开连接
    connect(m_tcp,&QTcpSocket::disconnected,[=](){
        ui->btn_Connect->setEnabled(true);
        ui->btn_Disconnect->setEnabled(false);
        m_tcp->close();
        m_status->setPixmap(QPixmap(":/4.jpg").scaled(20,20));
        ui->record->append("Disconnected!");
    });
    //连接成功
    connect(m_tcp,&QTcpSocket::connected,[=](){
        ui->record->append("Connection Succeeded!");
        ui->btn_Disconnect->setEnabled(true);
        ui->btn_Connect->setEnabled(false);
        m_status->setPixmap(QPixmap(":/5.jpg").scaled(20,20));
    });
    //状态栏信息
    m_status=new QLabel();
    ui->statusbar->addWidget(new QLabel("Status:"));
    m_status->setPixmap(QPixmap(":/4.jpg").scaled(20,20));
    ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//发送消息
void MainWindow::on_btn_Send_clicked()
{
    QString msg= ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("Client:"+msg);
}
//连接服务器
void MainWindow::on_btn_Connect_clicked()
{
    QString ip=ui->ServerIP->text();
    unsigned short port =ui->ServerPort->text().toUShort();
    m_tcp->connectToHost(QHostAddress(ip),port);

}

void MainWindow::on_btn_Disconnect_clicked()
{
    m_tcp->close();
    ui->btn_Disconnect->setEnabled(false);
     ui->btn_Connect->setEnabled(true);
}

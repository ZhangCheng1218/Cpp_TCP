#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setText("8899");
    setWindowTitle("Server");
    //创建server对象
    m_s=new QTcpServer(this);
    //监听开始后创建连接
    connect(m_s,&QTcpServer::newConnection,[=](){
         ui->record->append("NewConnectiong is create!");
         m_status->setPixmap(QPixmap(":/5.jpg").scaled(20,20));
         m_tcp=m_s->nextPendingConnection();
        //检测是否可以接受数据
        connect(m_tcp,&QTcpSocket::readyRead,[=](){
            QByteArray data= m_tcp->readAll();
            ui->record->append("Client:"+data);
        });
        //断开连接
        connect(m_tcp,&QTcpSocket::disconnected,[=](){
            m_tcp->close();
            m_tcp->deleteLater();
            m_status->setPixmap(QPixmap(":/4.jpg").scaled(20,20));
            ui->record->append("Disconnected!");
        });

    });
    //状态栏信息
    m_status=new QLabel();
    ui->statusbar->addWidget(new QLabel("Status:"));
    //默认断开
    m_status->setPixmap(QPixmap(":/4.jpg").scaled(20,20));
    ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//开始监听
void MainWindow::on_btn_Listen_clicked()
{
    ui->btn_Listen->setEnabled(false);
    unsigned short port=ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any,port);
}
//发送消息
void MainWindow::on_btn_Send_clicked()
{
    QString msg= ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("Server:"+msg);
}

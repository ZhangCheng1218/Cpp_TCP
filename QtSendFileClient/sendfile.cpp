#include "sendfile.h"
#include<QFileInfo>
#include <QHostAddress>
#include<QFile>
#include<QThread>


SendFile::SendFile(QObject *parent) : QObject(parent)
{

}
//发送文件
void SendFile::sendFile(QString path)
{
    QFile file(path);
    QFileInfo info(path);
    int fileSize=info.size();
    file.open(QFile::ReadOnly);
    while (!file.atEnd())
    {
        static int num =0;
        if(num==0)
        {
            m_tcp->write((char*)&fileSize,4);
        }
        QByteArray line=file.readLine();
        num+=line.size();
        int percent=(num*100/fileSize);
        emit curpercent(percent);
        m_tcp->write(line);
    }

}
//连接服务器
void SendFile::connectServer(QString ip,unsigned short port)
{
    m_tcp=new QTcpSocket;
    m_tcp->connectToHost(QHostAddress(ip),port);
    //连接成功
    connect(m_tcp,&QTcpSocket::connected,this,&SendFile::connectOK);

    //断开连接
    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
        //通知主线程，断开连接

        m_tcp->close();
        m_tcp->deleteLater();
        emit gameover();
    });
}

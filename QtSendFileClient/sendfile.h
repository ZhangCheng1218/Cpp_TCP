#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include<QTcpSocket>

class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);
    //发送文件
    void sendFile(QString path);
    //连接服务器
    void connectServer(QString ip,unsigned short port);
signals:
    //追加信号，方便子线程告诉主线程现状
    //连接成功信号
    void connectOK();
    //断开信号
    void gameover();
    //百分比信号
    void curpercent(int num);
private:
    QTcpSocket * m_tcp;
};

#endif // SENDFILE_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void Connect2Server(QString,unsigned short);
    void sendFile(QString path);
private slots:
    void on_btn_connect_clicked();

    void on_pushButton_clicked();

    void on_btn_selectFile_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QPainter>
#include <QMessageBox>
namespace Ui {
class RequestAccess;
}

class LogIn;
class RequestAccess : public QWidget
{
    Q_OBJECT

public:
    RequestAccess(QWidget *parent = nullptr);
    ~RequestAccess();

    virtual void paintEvent(QPaintEvent*);

private slots:
    void on_submitFile_PB_clicked();

    void on_back_PB_clicked();

    void on_request_PB_clicked();

private:
    Ui::RequestAccess *ui;
    std::shared_ptr<LogIn> m_logIn;
};

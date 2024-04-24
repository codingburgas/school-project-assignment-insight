#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "sha256.hpp"
class Register;
class DatabaseManager;
class MainWindow;
class RequestAccess;
namespace Ui {
class LogIn;
}

class LogIn : public QWidget
{
    Q_OBJECT

public:
    LogIn(QWidget *parent = nullptr);
    ~LogIn();

private slots:
    void on_register_PB_clicked();

    void on_requestAcess_PB_clicked();

    void on_logIn_PB_clicked();

private:
    Ui::LogIn *ui;

    virtual void paintEvent(QPaintEvent*) override;

    std::shared_ptr<Register> m_register;

    std::shared_ptr<DatabaseManager> m_database;

    std::shared_ptr<RequestAccess> m_requestAcs;
    std::shared_ptr<SHA256> m_SHA256;

    std::shared_ptr<MainWindow> m_mainWindow;
};

#endif // LOGIN_H

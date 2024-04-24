#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QPainter>
#include <QComboBox>
#include <QSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include "sha256.hpp"


class LogIn;
namespace Ui {
class Register;
}


class Register : public QWidget
{
    Q_OBJECT

public:
    Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_register_PB_clicked();

    void on_goBack_PB_clicked();

private:
    Ui::Register *ui;

    virtual void paintEvent(QPaintEvent*) override;

    std::shared_ptr<QComboBox> m_BirthYear;
    std::shared_ptr<SHA256> m_SHA256;
    std::shared_ptr<LogIn> m_login;

    int monthToNumber(QString& month);
};

#endif // REGISTER_H

#include "login.hpp"
#include "ui_login.h"
#include "mainwindow.hpp"
LogIn::LogIn(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogIn)
{
    ui->setupUi(this);
    m_register = std::make_shared<Register>();

    m_database = std::make_shared<DatabaseManager>();

    m_mainwindow = std::make_shared<MainWindow>();

}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QLinearGradient gradient(0, 0, width(), height()); // Define a linear gradient from top to bottom

    // Define the original colors
    QColor lightBlue(117, 123, 200);
    QColor lightPurple(193, 158, 215);

    // Darken the colors to 60%
    QColor darkBlue = lightBlue.darker(150); // Adjust the parameter to control darkness
    QColor darkPurple = lightPurple.darker(150); // Adjust the parameter to control darkness

    gradient.setColorAt(0, darkBlue); // Darkened Light Blue
    gradient.setColorAt(1, darkPurple); // Darkened Light Purple

    QRect rectLinear(this->rect());
    painter.fillRect(rectLinear, gradient);
}

void LogIn::on_register_PB_clicked()
{
    this->hide();
}


void LogIn::on_requestAcess_PB_clicked()
{
    this->hide();
}


void LogIn::on_logIn_PB_clicked()
{
    QString username = ui->username_LE->text();
    QString password = ui->password_LE->text();
    this->hide();
    m_mainwindow->show();
}


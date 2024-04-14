#include "mainmenu.hpp"
#include "ui_mainmenu.h"
#include "login.hpp"
MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    m_logIn = std::make_shared<LogIn>();
}

MainMenu::~MainMenu()
{
    delete ui;
}


void MainMenu::paintEvent(QPaintEvent *)
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



void MainMenu::on_LogIn_PB_clicked()
{
    this->hide();
    m_logIn->show();
}

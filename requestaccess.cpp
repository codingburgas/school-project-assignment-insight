#include "requestaccess.hpp"
#include "ui_requestaccess.h"
#include "login.hpp"
RequestAccess::RequestAccess(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequestAccess)
{
    ui->setupUi(this);
    m_logIn = std::make_shared<LogIn>();

}

RequestAccess::~RequestAccess()
{
    delete ui;
}

void RequestAccess::on_submitFile_PB_clicked()
{
    QString ImagePath = QFileDialog::getOpenFileName(this, tr("Select Image"), QCoreApplication::applicationDirPath(), tr("Image Files (*.jpg *.png *.pdf)"));

}

void RequestAccess::paintEvent(QPaintEvent *)
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


void RequestAccess::on_back_PB_clicked()
{
    this->hide();
    m_logIn->show();
}


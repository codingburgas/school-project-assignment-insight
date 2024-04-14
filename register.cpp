#include "register.hpp"
#include "ui_register.h"
#include "sha256.hpp"

Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    m_BirthYear = std::make_shared<QComboBox>();
    m_BirthYear.reset(ui->birthYear_CB);

    for(int i = 1924; i <= 2024; i++)
    {
        m_BirthYear->addItem(QString::number(i));
    }

    ui->email_LE->setPlaceholderText("Email address");
    ui->firstName_LE->setPlaceholderText("First Name");
    ui->lastName_LE->setPlaceholderText("Last Name");
    ui->password_LE->setPlaceholderText("Password");

    m_SHA256 = std::make_shared<SHA256>();

}

Register::~Register()
{
    delete ui;
}

void Register::paintEvent(QPaintEvent *)
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


void Register::on_register_PB_clicked()
{
    QString firstName = ui->firstName_LE->text();
    QString lastName = ui->lastName_LE->text();
    QString email = ui->email_LE->text();
    QString password = ui->password_LE->text();
    QString birthDay = ui->birthDay_CB->currentText();
    QString birthMonth = ui->birthMonth_CB->currentText();
    QString birthYear = ui->birthYear_CB->currentText();

    QString salt = m_SHA256->GenerateSalt();
    QString hashedPassword = m_SHA256->Hash(password, salt);
    qDebug() << hashedPassword;
}


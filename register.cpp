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

    for(int i = 0; i <= 31; i++)
    {
        ui->birthDay_CB->addItem(QString::number(i));
    }

    ui->useranme_LE->setPlaceholderText("Username");
    ui->firstName_LE->setPlaceholderText("First Name");
    ui->lastName_LE->setPlaceholderText("Last Name");
    ui->password_LE->setPlaceholderText("Password");
    ui->email_LE->setPlaceholderText("Email address");

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
    QString username = ui->useranme_LE->text();
    QString email = ui->useranme_LE->text();
    QString password = ui->password_LE->text();
    QString birthDay = ui->birthDay_CB->currentText();
    QString birthMonth = ui->birthMonth_CB->currentText();
    QString birthYear = ui->birthYear_CB->currentText();

    QString salt = m_SHA256->GenerateSalt();
    QString hashedPassword = m_SHA256->Hash(password, salt);

    QString birth = birthYear + "-" + birthMonth + "-" + birthDay;
    QSqlQuery qry;

    qry.prepare("INSERT INTO users(`First Name`, `Last Name`, Username, Email, Password, `Date Of Birth`) "
                "VALUES (:First_Name, :Last_Name, :Username, :Email, :Password, :Date_Of_Birth)");
    qry.bindValue(":First_Name", firstName);
    qry.bindValue(":Last_Name", firstName);
    qry.bindValue(":Username", username);
    qry.bindValue(":Email", email);
    qry.bindValue(":Password", hashedPassword);
    qry.bindValue(":Date_Of_Birth", birth);
    if (qry.exec()) {
        QMessageBox::information(this, "Success", "Your registration to Trawma Bank has been successful. \n\nRedirecting to the login page..");
    }
    else
    {
        qDebug() << qry.lastError();
    }

}


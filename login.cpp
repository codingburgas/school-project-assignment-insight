#include "login.hpp"
#include "ui_login.h"
#include "register.hpp"
#include "databasemanager.hpp"
#include "mainwindow.hpp"
#include "requestaccess.hpp"

LogIn::LogIn(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogIn)
{
    ui->setupUi(this);
    m_SHA256 = std::make_shared<SHA256>();
    m_register = std::make_shared<Register>();

    if (!m_database) {
        m_database = std::make_shared<DatabaseManager>();
        m_database->OpenConnection();
    }
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
    m_register->show();
}


void LogIn::on_requestAcess_PB_clicked()
{
    m_requestAcs = std::make_shared<RequestAccess>();
    this->hide();
    m_requestAcs->show();
}


void LogIn::on_logIn_PB_clicked()
{
    QString username = ui->username_LE->text();
    QString password = ui->password_LE->text();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM users WHERE Username = :username");
    qry.bindValue(":username", username);

    // Execute the query and check if the user exists
    if (qry.exec() && qry.next())
    {
        // Retrieve hashed password and salt from the database
        QString hashedPasswordFromDB = qry.value("Password").toString();
        QString saltFromDB = qry.value("Password Salt").toString();
        QString role = qry.value("Role").toString();

        // Hash the entered password for comparison
        QString hashedPasswordToCheck = m_SHA256->Hash(password, saltFromDB);

        // Check if the hashed passwords match
        if (hashedPasswordToCheck == hashedPasswordFromDB)
        {
            qry.bindValue(":username", username);

            if(qry.exec())
            {
                QMessageBox::information(this, "Login Successful", "Welcome to Insight! \n\nYou have successfully logged in.");
                m_mainWindow = std::make_shared<MainWindow>(this, username, role);
                this->hide();
                m_mainWindow->show();
            }
            else
            {
                // Handle the failure of the update query
                QMessageBox::critical(this, "Failure", "Application error, please contact us immediately");
                qDebug() << qry.lastError();
                qDebug() << qry.lastQuery();  // Print the last executed query for further inspection
                qDebug() << qry.boundValues();  // Print the bound values for further inspection
            }
        }
        else
        {
            // Handle the case of incorrect password
            QMessageBox::critical(this, "Failure", "Wrong password or username");
            qDebug() << qry.lastError();
            qDebug() << qry.lastQuery();  // Print the last executed query for further inspection
            qDebug() << qry.boundValues();  // Print the bound values for further inspection
        }
    }
    else
    {
        // Handle the case of incorrect username
        QMessageBox::critical(this, "Failure", "Wrong password or username");
        qDebug() << qry.lastError();
        qDebug() << qry.lastQuery();  // Print the last executed query for further inspection
        qDebug() << qry.boundValues();  // Print the bound values for further inspection
    }
}


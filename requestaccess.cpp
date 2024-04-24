#include "requestaccess.hpp"
#include "ui_requestaccess.h"
#include "login.hpp"
#include <QBuffer>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>


RequestAccess::RequestAccess(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RequestAccess)
{
    ui->setupUi(this);

}

RequestAccess::~RequestAccess()
{
    delete ui;
}

void RequestAccess::on_submitFile_PB_clicked()
{
    QString username = ui->username_LE->text();
    QString ImagePath = QFileDialog::getOpenFileName(this, tr("Select Image"), QCoreApplication::applicationDirPath(), tr("Image Files (*.jpg *.png)"), 0);

    if (!ImagePath.isEmpty())
    {
        QPixmap Image(ImagePath);
        QPixmap scaledImage = Image.scaled(ui->proof_LA->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QBuffer ImageBufferData;

        if (ImageBufferData.open(QIODevice::ReadWrite))
        {
            QString fileExtension = QFileInfo(ImagePath).suffix().toLower();
            if (fileExtension == "jpg" || fileExtension == "jpeg" || fileExtension == "png")
            {
                // Save the image data in the appropriate format
                if (fileExtension == "jpg" || fileExtension == "jpeg")
                    Image.save(&ImageBufferData, "JPG");
                else if (fileExtension == "png")
                    Image.save(&ImageBufferData, "PNG");

                ImageBufferData.close();
                QByteArray FinalDataToSave = ImageBufferData.buffer().toBase64();
                QSqlQuery qry;
                qry.prepare("INSERT INTO accessrequests(Username, `Access Requested`, `Document Proof`)"
                            "VALUES(:username, :accessRequested, :documentProof)");

                qry.bindValue(":username", username);
                qry.bindValue(":accessRequested", "Teacher");
                qry.bindValue(":documentProof", FinalDataToSave);

                if (qry.exec())
                {
                    QSqlDatabase::database().commit();
                    ui->proof_LA->setPixmap(scaledImage);
                }
                else
                {
                    qDebug() << qry.lastError();
                }
            }
        }
    }
    else
    {
        qDebug() << "invalid image";
    }
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
    m_logIn = std::make_shared<LogIn>();
    m_logIn->show();
}


void RequestAccess::on_request_PB_clicked()
{
    QMessageBox::information(this, "Acess Requested", "You have successfully requested access");
    this->hide();
    m_logIn = std::make_shared<LogIn>();
    m_logIn->show();
}


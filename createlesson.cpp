#include "createlesson.hpp"
#include "ui_createlesson.h"

CreateLesson::CreateLesson(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateLesson)
{
    ui->setupUi(this);
}

CreateLesson::~CreateLesson()
{
    delete ui;
}

void CreateLesson::on_createLesson_PB_clicked()
{
    QString heading = ui->heading_LE->text();
    QString lesson = ui->text_LE->toPlainText();
    QSqlQuery qry;

    qry.prepare("INSERT INTO lessons(Heading, Lesson) "
                "VALUES (:heading, :lesson)");
    qry.bindValue(":heading", heading);
    qry.bindValue(":lesson", lesson);
    if (qry.exec()) {
        QMessageBox::information(this, "Success", "Your registration to Trawma Bank has been successful. \n\nRedirecting to the login page..");
    }
}


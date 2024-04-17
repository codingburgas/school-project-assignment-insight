#include "mainwindow.hpp"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Courses_SW->setCurrentIndex(1);

    UpdateLessons();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCourseButtons(const QString &heading)
{
    ui->Navbar->setCurrentIndex(5);
    ui->heading_LA->setText(heading);
    QSqlQuery qry;
    qry.prepare("SELECT Lesson FROM lessons WHERE Heading = :heading");
    qry.bindValue(":heading", heading);
    if(qry.exec())
    {
        while(qry.next()) {
            ui->lesson_TB->setText(qry.value(0).toString());
        }
    }
    else
    {
        qDebug() << "Error executing query:" << qry.lastError();
    }
}
void MainWindow::paintEvent(QPaintEvent *)
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

void MainWindow::on_ITCourses_PB_clicked()
{
    ui->Courses_SW->setCurrentIndex(1);
}


void MainWindow::on_programmingCourses_PB_clicked()
{
    ui->Courses_SW->setCurrentIndex(2);
}


void MainWindow::on_homepage_PB_clicked()
{
    ui->Navbar->setCurrentIndex(0);
}


void MainWindow::on_schedule_PB_clicked()
{
    ui->Navbar->setCurrentIndex(1);
}


void MainWindow::on_classes_PB_clicked()
{
    ui->Navbar->setCurrentIndex(2);
}


void MainWindow::on_excelSpecialistCourse_PB_clicked()
{
    ui->Courses_SW->setCurrentIndex(5);
}


void MainWindow::on_createLesson_PB_clicked()
{
    ui->Navbar->setCurrentIndex(6);
}


void MainWindow::UpdateLessons()
{
    qDebug() << "sds";
    lessonHeadings.clear();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM lessons");

    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    while(qry.next())
    {
        lessonHeadings.push_back(qry.value(1).toString());
    }
    QWidget* lessonWidget = ui->courses_WG;
    int yOffset = 0; // Initial vertical offset
    int counter = 0;


    for (const QString& heading : lessonHeadings) {
        QLabel *iconLabel = new QLabel(lessonWidget);
        iconLabel->setObjectName(heading + "_icon_LA"); // Set object name
        QPixmap icon(":/Resources/Images/icons/Test Results.png");
        iconLabel->setPixmap(icon);
        iconLabel->setGeometry(20, yOffset, 50, 50);
        iconLabel->setStyleSheet("background-color:transparent");

        QLabel *label = new QLabel(heading, lessonWidget);
        label->setObjectName(heading + "_LA"); // Set object name
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(50, yOffset, 100, 50);

        QPushButton *acessCourse = new QPushButton(lessonWidget);
        acessCourse->setObjectName(heading + "_acessCourse_PB"); // Set object name
        acessCourse->setStyleSheet("background-color: transparent; border: 1px solid black;");
        acessCourse->setGeometry(10, yOffset, 500, 50);
        connect(acessCourse, &QPushButton::clicked, this, [=]() { handleCourseButtons(heading); });

        QPushButton *deleteButton = new QPushButton(lessonWidget);
        deleteButton->setObjectName(heading + "_delete_PB"); // Set object name
        deleteButton->setGeometry(465, yOffset + 5, 40, 40);
        QIcon closeIcon(":/Resources/Images/icons/Close.png");
        deleteButton->setIcon(closeIcon);
        deleteButton->setIconSize(QSize(30, 30));
        deleteButton->setStyleSheet("background-color: #900C0C; border: none;");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteLesson(heading); });

        QPushButton *editButton = new QPushButton(lessonWidget);
        editButton->setObjectName(heading + "_edit_PB"); // Set object name
        editButton->setGeometry(420, yOffset, 50, 50);
        QIcon editIcon(":/Resources/Images/icons/Edit.png");
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(30, 30));
        editButton->setStyleSheet("background-color: transparent; border: none;"); // Customize the appearance
        connect(editButton, &QPushButton::clicked, this, [=]() { editLesson(heading); });


        yOffset += 75;
        counter++;
    }

    qDebug() << counter;

}
void MainWindow::on_addLesson_PB_clicked()
{
    QString heading = ui->heading_LE->text();
    QString lesson = ui->material_PTE->toPlainText();
    QSqlQuery qry;

    qry.prepare("INSERT INTO lessons(Heading, Lesson) "
                "VALUES (:heading, :lesson)");
    qry.bindValue(":heading", heading);
    qry.bindValue(":lesson", lesson);
    if (qry.exec()) {
        QMessageBox::information(this, "Success", "You have successfully published a lesson.");
        UpdateLessons();
        ui->Navbar->setCurrentIndex(2);
        ui->heading_LE->clear();
        ui->material_PTE->clear();

    }
    else
    {
        QMessageBox::critical(this, "Failure", "An error occured. Please contact us immediately");
        qDebug() << qry.lastError();

    }
}

void MainWindow::editLesson(const QString& heading)
{
    ui->Navbar->setCurrentIndex(6);
    QSqlQuery qry;
    qry.prepare("SELECT * FROM lessons WHERE Heading = :heading");
    qry.bindValue(":heading", heading);

    if(qry.exec() && qry.next()) {
        ui->heading_LE->setText(qry.value(1).toString());
        ui->material_PTE->setPlainText(qry.value(2).toString());
        ui->addLesson_PB->setText("Update Lesson");
    } else {
        qDebug() << qry.lastError();
    }
}



void MainWindow::deleteLesson(const QString &heading) {
    QSqlQuery qry;
    qry.prepare("DELETE FROM lessons WHERE heading = :heading");
    qry.bindValue(":heading", heading);
    if(qry.exec())
    {
        // Remove the UI elements associated with the deleted lesson
        QList<QPushButton*> buttons = ui->courses_WG->findChildren<QPushButton*>(heading + "_acessCourse_PB");
        for (QPushButton* button : buttons) {
            delete button;
        }

        QList<QLabel*> labels = ui->courses_WG->findChildren<QLabel*>(heading + "_LA");
        for (QLabel* label : labels) {
            delete label;
        }

        QList<QLabel*> iconLabels = ui->courses_WG->findChildren<QLabel*>(heading + "_icon_LA");
        for (QLabel* iconLabel : iconLabels) {
            delete iconLabel;
        }

        QList<QPushButton*> newButtons = ui->courses_WG->findChildren<QPushButton*>(heading + "_delete_PB");
        for (QPushButton* newButton : newButtons) {
            delete newButton;
        }

        QList<QPushButton*> iconButtons = ui->courses_WG->findChildren<QPushButton*>(heading + "_edit_PB");
        for (QPushButton* iconButton : iconButtons) {
            delete iconButton;
        }

        UpdateLessons();
    }
    else
    {
        qDebug() << qry.lastError();
    }
}



void MainWindow::on_goBack_PB_clicked()
{
    ui->Navbar->setCurrentIndex(2);
}


void MainWindow::on_goBack_lesson_PB_clicked()
{
    ui->Navbar->setCurrentIndex(2);
}


void MainWindow::on_exams_PB_clicked()
{
    ui->Navbar->setCurrentIndex(3);
    ui->Exams_SW->setCurrentIndex(1);
}

void MainWindow::on_excelSpecialist_PB_clicked()
{
    ui->Exams_SW->setCurrentIndex(3);
}



void MainWindow::on_createExam_PB_clicked()
{
    ui->Exams_SW->setCurrentIndex(4);
}


void MainWindow::on_createQuestion_PB_clicked()
{
    ui->Exams_SW->setCurrentIndex(5);
}



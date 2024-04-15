#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "createlesson.hpp"
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Courses_SW->setCurrentIndex(1);
    m_CreateLesson = std::make_shared<CreateLesson>();

    std::vector<QString> lessonHeadings;
    std::vector<QString> lessonText;

    QSqlQuery qry;
    qry.prepare("SELECT * FROM lessons");

    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    while(qry.next())
    {
        lessonHeadings.push_back(qry.value(1).toString());
        //lessonText.push_back(qry.value(2).toString());
    }
    QWidget* lessonWidget = ui->courses_WG;

    int yOffset = 0; // Initial vertical offset

    for (const QString& heading : lessonHeadings) {
        QLabel *iconLabel = new QLabel(lessonWidget);
        QPixmap icon(":/Resources/Images/icons/Test Results.png");
        iconLabel->setPixmap(icon);
        iconLabel->setGeometry(10, yOffset, 50, 50);
        iconLabel->setStyleSheet("background-color:transparent");

        QLabel *label = new QLabel(heading, lessonWidget);
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(40, yOffset, 100, 50);

        QPushButton *button = new QPushButton(lessonWidget);
        button->setStyleSheet("background-color: transparent; border: 1px solid black;");
        button->setGeometry(0, yOffset, 200, 50);
        button->setObjectName(heading + "_PB");
        connect(button, &QPushButton::clicked, this, [=]() { handleCourseButtons(heading); });
        yOffset += 75;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCourseButtons(const QString &buttonName)
{
    QSqlQuery qry;
    qry.prepare("SELECT Lesson FROM lessons WHERE Heading = :heading");
    qry.bindValue(":heading", buttonName);
    if(qry.exec())
    {
        while(qry.next()) {
            qDebug() << qry.value(0);
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
    this->hide();
    m_CreateLesson->show();
}


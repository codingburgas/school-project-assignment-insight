#include "mainwindow.hpp"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Courses_SW->setCurrentIndex(1);

    UpdateLessons();
    UpdateExams();

    ui->scrollArea_3->setWidgetResizable(true); // Allow the widget to resize with the scroll area
    ui->scrollArea_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Set size policy

    // Set scroll area widget
    ui->scrollArea_3->setWidget(ui->scrollAreaWidgetContents_3);

    // Add widgets and adjust their sizes and geometries

    // After adding all widgets, adjust the size of the widget contents
    ui->scrollAreaWidgetContents_3->adjustSize();

    // Set scrollbar policy
    ui->scrollArea_3->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->questionType_SW->setCurrentIndex(0);
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
}
void MainWindow::on_addLesson_PB_clicked()
{
    ui->heading_LA->clear();
    ui->material_PTE->clear();
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
    ui->heading_LA->clear();
    ui->material_PTE->clear();
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


void MainWindow::on_addQuestion_PB_clicked()
{
    ui->Exams_SW->setCurrentIndex(5);
}



void MainWindow::on_createQuestion_PB_clicked()
{
    QString examName = ui->examName_LE->text();
    QString question = ui->question_LE->text();
    QString openAnswer = ui->openAnswer_TE->toPlainText();
    QString answerA = ui->answerA_LE->text();
    QString answerB = ui->answerB_LE->text();
    QString answerC = ui->answerC_LE->text();
    QString answerD = ui->answerD_LE->text();
    QString correctAnswers;
    QString points = ui->points_LE->text();

    for(int i = 0; i <= 4; i++)
    {
        QString correctCheckBoxName = "correct" + QString(QChar('A' + i)) + "_CB";
        QCheckBox* checkBox = findChild<QCheckBox*>(correctCheckBoxName);
        if(checkBox && checkBox->isChecked())
        {
            QString answerLineEditName = "answer" + QString(QChar('A' + i)) + "_LE";
            QLineEdit* answerLineEdit = findChild<QLineEdit*>(answerLineEditName);
            if(answerLineEdit)
            {
                QString answer = answerLineEdit->text();
                correctAnswers += answer + ";";
            }
        }
    }

    QSqlQuery qry;
    qry.prepare("INSERT INTO questions(`Exam Name`, `Question`, `Answer1`, `Answer2`, `Answer3`, `Answer4`, `Open Answer`, `Correct Answers`, `Points`)"
                "VALUES(:examName, :question, :answer1, :answer2, :answer3, :answer4, :openAnswer, :correctAnswers, :points)");
    qry.bindValue(":examName", examName);
    qry.bindValue(":question", question);
    qry.bindValue(":answer1", answerA);
    qry.bindValue(":answer2", answerB);
    qry.bindValue(":answer3", answerC);
    qry.bindValue(":answer4", answerD);
    qry.bindValue(":openAnswer", openAnswer);
    qry.bindValue(":correctAnswers", correctAnswers);
    qry.bindValue(":points", points);
    if(!qry.exec())
    {
        qDebug() << qry.lastError();
    }
    else
    {
        QMessageBox::information(this, "Success", "Question added");
        ui->Exams_SW->setCurrentIndex(4);
        UpdateQuestions(examName);
    }
}

void MainWindow::UpdateExams()
{
    examNames.clear();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM exams");

    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    while(qry.next())
    {
        examNames.push_back(qry.value(2).toString());
    }
    QWidget* examsWidget = ui->exams_WG;
    int yOffset = 0; // Initial vertical offset


    for (const QString& exam : examNames) {
        QLabel *iconLabel = new QLabel(examsWidget);
        iconLabel->setObjectName(exam + "_icon_LA"); // Set object name
        QPixmap icon(":/Resources/Images/icons/Test Results.png");
        iconLabel->setPixmap(icon);
        iconLabel->setGeometry(20, yOffset, 50, 50);
        iconLabel->setStyleSheet("background-color:transparent");

        QLabel *label = new QLabel(exam, examsWidget);
        label->setObjectName(exam + "_LA"); // Set object name
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(50, yOffset, 100, 50);

        QPushButton *acessExam = new QPushButton(examsWidget);
        acessExam->setObjectName(exam + "_acessExam_PB"); // Set object name
        acessExam->setStyleSheet("background-color: transparent; border: 1px solid black;");
        acessExam->setGeometry(10, yOffset, 500, 50);
        connect(acessExam, &QPushButton::clicked, this, [=]() { accessExam(exam); });

        QPushButton *deleteButton = new QPushButton(examsWidget);
        deleteButton->setObjectName(exam + "_delete_PB"); // Set object name
        deleteButton->setGeometry(465, yOffset + 5, 40, 40);
        QIcon closeIcon(":/Resources/Images/icons/Close.png");
        deleteButton->setIcon(closeIcon);
        deleteButton->setIconSize(QSize(30, 30));
        deleteButton->setStyleSheet("background-color: #900C0C; border: none;");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteExam(exam); });

        QPushButton *editButton = new QPushButton(examsWidget);
        editButton->setObjectName(exam + "_edit_PB"); // Set object name
        editButton->setGeometry(420, yOffset, 50, 50);
        QIcon editIcon(":/Resources/Images/icons/Edit.png");
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(30, 30));
        editButton->setStyleSheet("background-color: transparent; border: none;"); // Customize the appearance
        connect(editButton, &QPushButton::clicked, this, [=]() { editLesson(exam); });


        yOffset += 75;
    }

}


void MainWindow::UpdateQuestions(const QString& examName)
{
    questions.clear();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM questions WHERE `Exam Name` = :examName");
    qry.bindValue(":examName", examName);

    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    while(qry.next())
    {
        questions.push_back(qry.value(2).toString());
    }

    QWidget* questionsWidget = new QWidget; // Create a new widget to hold all the questions
    QVBoxLayout *layout = new QVBoxLayout(questionsWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop);
    for (const QString& question : questions) {
        QWidget *questionWidget = new QWidget; // Create a widget for each question

        // Set fixed size for the question widget
        questionWidget->setFixedSize(800, 100);

        QHBoxLayout *questionLayout = new QHBoxLayout(questionWidget); // Create a horizontal layout for each question

        QLabel *iconLabel = new QLabel; // Create icon label
        QPixmap icon(":/Resources/Images/icons/Test Results.png");
        iconLabel->setPixmap(icon);
        iconLabel->setStyleSheet("background-color:transparent");
        iconLabel->setFixedSize(50, 50);
        questionLayout->addWidget(iconLabel);

        QLabel *label = new QLabel(question); // Create label for the question text
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setFixedSize(100, 50);
        questionLayout->addWidget(label);

        QPushButton *accessExam = new QPushButton; // Create button to access the exam
        accessExam->setStyleSheet("background-color: transparent; border: 1px solid black;");
        connect(accessExam, &QPushButton::clicked, this, [=]() { accessQuestion(examName); });
        accessExam->setFixedSize(500, 50);
        questionLayout->addWidget(accessExam);

        QPushButton *deleteButton = new QPushButton; // Create button to delete the question
        QIcon closeIcon(":/Resources/Images/icons/Close.png");
        deleteButton->setIcon(closeIcon);
        deleteButton->setIconSize(QSize(30, 30));
        deleteButton->setStyleSheet("background-color: #900C0C; border: none;");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteQuestion(examName); });
        deleteButton->setFixedSize(40, 40);
        questionLayout->addWidget(deleteButton);

        QPushButton *editButton = new QPushButton; // Create button to edit the question
        QIcon editIcon(":/Resources/Images/icons/Edit.png");
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(30, 30));
        editButton->setStyleSheet("background-color: transparent; border: none;");
        connect(editButton, &QPushButton::clicked, this, [=]() { editExam_PB(accessExam->objectName()); });
        editButton->setFixedSize(50, 50);
        questionLayout->addWidget(editButton);

        layout->addWidget(questionWidget); // Add the question widget to the layout
    }

    ui->scrollArea_3->setWidget(questionsWidget); // Set the widget containing all questions as the content of the scroll area


}

void MainWindow::accessQuestion(const QString& question)
{

}

void MainWindow::editQuestion(const QString& question)
{

}
void MainWindow::deleteQuestion(const QString& question)
{

}

void MainWindow::accessExam(const QString& examName)
{
    UpdateQuestions(examName);
    ui->Exams_SW->setCurrentIndex(4);
    ui->examName_LA->hide();
    ui->examName_LE->hide();
    ui->subject_LA->setText(examName);
    ui->examName_LE->setText(examName);
    ui->publishExam_PB->setText("Edit exam");
    UpdateQuestions(examName);
}

void MainWindow::deleteExam(const QString& examName){
    QSqlQuery qry;
    qry.prepare("DELETE FROM exams WHERE `Exam Name` = :examName");
    qry.bindValue(":examName", examName);
    if(qry.exec())
    {
        // Remove the UI elements associated with the deleted lesson
        QList<QPushButton*> buttons = ui->exams_WG->findChildren<QPushButton*>(examName + "_acessExam_PB");
        for (QPushButton* button : buttons) {
            delete button;
        }

        QList<QLabel*> labels = ui->exams_WG->findChildren<QLabel*>(examName + "_LA");
        for (QLabel* label : labels) {
            delete label;
        }

        QList<QLabel*> iconLabels = ui->exams_WG->findChildren<QLabel*>(examName + "_icon_LA");
        for (QLabel* iconLabel : iconLabels) {
            delete iconLabel;
        }

        QList<QPushButton*> newButtons = ui->exams_WG->findChildren<QPushButton*>(examName + "_delete_PB");
        for (QPushButton* newButton : newButtons) {
            delete newButton;
        }

        QList<QPushButton*> iconButtons = ui->exams_WG->findChildren<QPushButton*>(examName + "_edit_PB");
        for (QPushButton* iconButton : iconButtons) {
            delete iconButton;
        }

        UpdateExams();
    }
    else
    {
        qDebug() << qry.lastError();
    }

}
void MainWindow::editExam_PB(const QString& examName){

    QSqlQuery qry;
    qry.prepare("SELECT * FROM questions WHERE `Exam Name` = :examName");
    qry.bindValue(":examName", examName);
    ui->Exams_SW->setCurrentIndex(5);
    if(qry.exec())
    {

    }
}


void MainWindow::on_publishExam_PB_clicked()
{
    QSqlQuery qry;
    QString examName = ui->examName_LE->text();
    qry.prepare("INSERT INTO exams (Subject, `Exam Name`) "
                "VALUES(:subject, :examName)");
    qry.bindValue(":subject", ui->subject_LA->text());
    qry.bindValue(":examName", examName);

    if(qry.exec())
    {
        qDebug() << "Inserted successfully.";
        ui->Exams_SW->setCurrentIndex(3);
        UpdateExams();
    }
    else
    {
        qDebug() << "Error executing query:" << qry.lastError().text();
    }
}


void MainWindow::on_closedAnswers_CB_stateChanged(int arg1)
{
    if(ui->closedAnswers_CB->isChecked())
    {
        ui->questionType_SW->setCurrentIndex(0);
    }
    else
    {
        ui->questionType_SW->setCurrentIndex(1);

    }
}


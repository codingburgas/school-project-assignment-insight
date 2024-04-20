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

    QWidget* lessonWidget = new QWidget(ui->courses_WG); // Create a new widget to hold all the lessons
    QVBoxLayout *layout = new QVBoxLayout(lessonWidget); // Create a vertical layout for the widget

    for (const QString& heading : lessonHeadings) {
        QWidget *lessonItemWidget = new QWidget; // Create a widget for each lesson
        lessonItemWidget->setFixedSize(800, 70);
        QLabel *iconLabel = new QLabel; // Create icon label
        QPixmap icon(":/Resources/Images/icons/Test Results.png");
        iconLabel->setPixmap(icon);
        iconLabel->setStyleSheet("background-color:transparent");

        QLabel *label = new QLabel(heading, lessonItemWidget);
        label->setObjectName(heading + "_LA"); // Set object name
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(10, 0, 100, 50);

        QPushButton *accessCourse = new QPushButton(lessonItemWidget); // Create button to access the course
        accessCourse->setFixedSize(500,50);
        accessCourse->setStyleSheet("background-color: transparent; border: 1px solid black; color: black;");
        connect(accessCourse, &QPushButton::clicked, this, [=]() { handleCourseButtons(heading); });

        QPushButton *deleteButton = new QPushButton(lessonItemWidget); // Create button to delete the lesson
        QIcon closeIcon(":/Resources/Images/icons/Close.png");
        deleteButton->setIcon(closeIcon);
        deleteButton->setIconSize(QSize(30, 30));
        deleteButton->setGeometry(455, 5, 40, 40);
        deleteButton->setStyleSheet("background-color: #900C0C; border: none;");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteLesson(heading); });

        QPushButton *editButton = new QPushButton(lessonItemWidget); // Create button to edit the lesson
        QIcon editIcon(":/Resources/Images/icons/Edit.png");
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(30, 30));
        editButton->setStyleSheet("background-color: transparent; border: none;");
        editButton->setGeometry(420, 5, 40 ,40);
        connect(editButton, &QPushButton::clicked, this, [=]() { editLesson(heading); });

        layout->addWidget(lessonItemWidget); // Add the lesson widget to the layout
    }

    ui->scrollArea_5->setWidget(lessonWidget); // Set the widget containing all lessons as the content of the scroll area
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
    ui->heading_LA->clear();
    ui->material_PTE->clear();
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

    QWidget* examsWidget = new QWidget(ui->scrollAreaWidgetContents); // Create a new widget to hold all the exams
    QVBoxLayout *layout = new QVBoxLayout(examsWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop); // Align widgets to the top

    for (const QString& exam : examNames) {
        QWidget *examWidget = new QWidget; // Create a widget for each exam
        examWidget->setFixedSize(500, 50);
        QLabel *iconLabel = new QLabel; // Create icon label
        iconLabel->setObjectName(exam + "_icon_LA"); // Set object name
        QPixmap icon(":/Resources/Images/icons/Test Results.png");
        iconLabel->setPixmap(icon);
        iconLabel->setStyleSheet("background-color:transparent");

        QLabel *label = new QLabel(exam, examWidget); // Create label for the exam name
        label->setObjectName(exam + "_LA"); // Set object name
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(20, 0, 100, 50);

        QPushButton *accessExam = new QPushButton(examWidget); // Create button to access the exam
        accessExam->setObjectName(exam + "_accessExam_PB"); // Set object name
        accessExam->setStyleSheet("background-color: transparent; border: 1px solid black;");
        accessExam->setFixedSize(500, 50);
        connect(accessExam, &QPushButton::clicked, this, [=]() { EnterExam(exam); });


        QPushButton *deleteButton = new QPushButton(examWidget); // Create button to delete the exam
        deleteButton->setObjectName(exam + "_delete_PB"); // Set object name
        QIcon closeIcon(":/Resources/Images/icons/Close.png");
        deleteButton->setIcon(closeIcon);
        deleteButton->setIconSize(QSize(30, 30));
        deleteButton->setStyleSheet("background-color: #900C0C; border: none;");
        deleteButton->setGeometry(455, 5, 40, 40);
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteExam(exam); });

        QPushButton *editButton = new QPushButton(examWidget); // Create button to edit the exam
        editButton->setObjectName(exam + "_edit_PB"); // Set object name
        QIcon editIcon(":/Resources/Images/icons/Edit.png");
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(30, 30));
        editButton->setGeometry(420, 5, 40 ,40);
        editButton->setStyleSheet("background-color: transparent; border: none;");
        connect(editButton, &QPushButton::clicked, this, [=]() { editExam(exam); });

        layout->addWidget(examWidget); // Add the exam widget to the layout
    }

    ui->scrollArea_6->setWidget(examsWidget); // Set the widget containing all exams as the content of the scroll area
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

    QWidget* questionsWidget = new QWidget(ui->CreateExam); // Create a new widget to hold all the questions
    QVBoxLayout *layout = new QVBoxLayout(questionsWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop);
    for (const QString& question : questions) {
        QWidget *questionWidget = new QWidget; // Create a widget for each question

        // Set fixed size for the question widget
        questionWidget->setFixedSize(800, 70);


        QLabel *label = new QLabel(question, questionWidget);
        label->setObjectName(question + "_LA"); // Set object name
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(20, 0, 100, 50);

        QPushButton *accessExam = new QPushButton(questionWidget); // Create button to access the exam
        accessExam->setStyleSheet("background-color: transparent; border: 1px solid black; color: black;");
        connect(accessExam, &QPushButton::clicked, this, [=]() { accessQuestion(question); });
        accessExam->setFixedSize(500, 50);

        QPushButton *deleteButton = new QPushButton(questionWidget); // Create button to delete the question
        QIcon closeIcon(":/Resources/Images/icons/Close.png");
        deleteButton->setIcon(closeIcon);
        deleteButton->setIconSize(QSize(30, 30));
        deleteButton->setGeometry(455, 5, 40, 40);
        deleteButton->setStyleSheet("background-color: #900C0C; border: none; color: black;");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteQuestion(examName); });

        QPushButton *editButton = new QPushButton(questionWidget); // Create button to edit the question
        QIcon editIcon(":/Resources/Images/icons/Edit.png");
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(30, 30));
        editButton->setGeometry(420, 5, 40 ,40);
        editButton->setStyleSheet("background-color: transparent; border: none; color: black;");
        connect(editButton, &QPushButton::clicked, this, [=]() { editQuestion(question); });

        layout->addWidget(questionWidget); // Add the question widget to the layout
    }

    ui->scrollArea_3->setWidget(questionsWidget); // Set the widget containing all questions as the content of the scroll area

}

void MainWindow::accessQuestion(const QString& examName)
{

}


void MainWindow::editQuestion(const QString& examName)
{
    ui->Exams_SW->setCurrentIndex(5);
    QSqlQuery qry;
    qry.prepare("SELECT * FROM questions WHERE Question = :examName");
    qry.bindValue(":examName", examName);
    QString question;
    QString answer1;
    QString answer2;
    QString answer3;
    QString answer4;
    QString points;
    QString correctAnswers;
    if(qry.exec())
    {
        if(qry.next())
        {
            question = qry.value("Question").toString();
            answer1 = qry.value("Answer1").toString();
            answer2 = qry.value("Answer2").toString();
            answer3 = qry.value("Answer3").toString();
            answer4 = qry.value("Answer4").toString();
            points = qry.value("Points").toString();
            correctAnswers = qry.value("Correct Answers").toString();
        }
    }
    ui->question_LE->setText(question);
    ui->answerA_LE->setText(answer1);
    ui->answerB_LE->setText(answer2);
    ui->answerC_LE->setText(answer3);
    ui->answerD_LE->setText(answer4);
    ui->points_LE->setText(points);

    // Split correctAnswers into individual answers
    QStringList correctAnswersList = correctAnswers.split(';');

    // Check the checkboxes based on correctAnswers
    for(const QString& correctAnswer : correctAnswersList)
    {
        qDebug() << correctAnswer;
        int answerIndex = correctAnswer.toInt() - 1; // Assuming answers are 1-indexed
        if(answerIndex >= 0 && answerIndex <= 3)
        {
            QString checkBoxName = "correct" + QString(QChar('A' + answerIndex)) + "_CB";
            QCheckBox* checkBox = findChild<QCheckBox*>(checkBoxName);
            if(checkBox)
                checkBox->setChecked(true);
        }
    }

}
void MainWindow::deleteQuestion(const QString& examName)
{

}

void MainWindow::EnterExam(const QString& examName)
{
    QString questionDB;
    QString answerA;
    QString answerB;
    QString answerC;
    QString answerD;
    QString pointsDB;
    qDebug() << examName;
    questions.clear();
    ui->Exams_SW->setCurrentIndex(6);

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
        questionDB = qry.value("Question").toString();
        answerA = qry.value("Answer1").toString();
        answerB = qry.value("Answer2").toString();
        answerC = qry.value("Answer3").toString();
        answerD = qry.value("Answer4").toString();
        pointsDB = qry.value("Points").toString();
    }

    QWidget* questionsWidget = new QWidget(ui->ExamOverview); // Create a new widget to hold all the questions
    QVBoxLayout *layout = new QVBoxLayout(questionsWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop);
    int counter = 1;
    for (const QString& question : questions) {
        counter++;
        qDebug() << questionDB;

        QWidget *questionWidget = new QWidget; // Create a widget for each question

        // Set fixed size for the question widget
        questionWidget->setFixedSize(470, 300);
        questionsWidget->setStyleSheet("border: 1px solid black;");


        QLabel *questionNumber = new QLabel("Question 1", questionWidget);
        questionNumber->setObjectName(questionDB + "_number_LA"); // Set object name
        questionNumber->setText("Question ");
        questionNumber->setStyleSheet("QLabel{background-color:transparent;qproperty-alignment: AlignLeft;color:black;font: 16pt;border:none;}");
        questionNumber->setGeometry(10, 10, 190, 20);

        QLabel *points = new QLabel("Points: ", questionWidget);
        points->setText("Points: " + pointsDB);
        points->setObjectName(questionDB + "_points_LA"); // Set object name
        points->setStyleSheet("QLabel{background-color:transparent;qproperty-alignment: AlignRight;color:black;font: 16pt;border:none;}");
        points->setGeometry(230, 10, 230, 20);

        QLabel *question_LA = new QLabel(questionDB, questionWidget);
        question_LA->setObjectName(questionDB + "_LA"); // Set object name
        question_LA->setStyleSheet("QLabel{background-color:transparent;qproperty-alignment: AlignCenter;color:black;font: 16pt;border:none;}");
        question_LA->setGeometry(10, 60, 450, 20);

        QCheckBox *answer1 = new QCheckBox("A: ", questionWidget);
        answer1->setObjectName(questionDB + "_CB"); // Set object name
        answer1->setText("A: " + answerA);
        answer1->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
        answer1->setGeometry(30, 120, 180, 70);

        QCheckBox *answer2 = new QCheckBox("B: ", questionWidget);
        answer2->setObjectName(questionDB + "_CB"); // Set object name
        answer2->setText("B: " + answerB);
        answer2->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
        answer2->setGeometry(240, 120, 180, 70);

        QCheckBox *answer3 = new QCheckBox("C: ", questionWidget);
        answer3->setObjectName(questionDB + "_CB"); // Set object name
        answer3->setText("C: " + answerC);
        answer3->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
        answer3->setGeometry(30, 210, 180, 70);

        QCheckBox *answer4 = new QCheckBox("D: ", questionWidget);
        answer4->setObjectName(questionDB + "_CB"); // Set object name
        answer4->setText("D: " + answerD);
        answer4->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
        answer4->setGeometry(240, 210, 180, 70);

        layout->addWidget(questionWidget); // Add the question widget to the layout
    }

    ui->scrollArea_4->setWidget(questionsWidget); // Set the widget containing all questions as the content of the scroll area

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
void MainWindow::editExam(const QString& examName){
    UpdateQuestions(examName);
    ui->Exams_SW->setCurrentIndex(4);
    ui->examName_LA->hide();
    ui->examName_LE->hide();
    ui->subject_LA->setText("Exam " + examName);
    ui->examName_LE->setText(examName);
    ui->publishExam_PB->setText("Edit exam");
    UpdateQuestions(examName);

    // ui->Exams_SW->setCurrentIndex(6);
    // QWidget* QuestionsWidget = ui->scrollAreaWidgetContents_4;
    // QWidget* questionWidget = ui->question_WG;
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


void MainWindow::on_submitExam_PB_clicked()
{
    // Find all widgets within the scroll area
    QList<QWidget*> allWidgets = ui->scrollArea_4->findChildren<QWidget*>();
    qDebug() << "Total widgets found in scroll area:" << allWidgets.size();

    // Iterate through all found widgets
    for (QWidget* questionWidget : allWidgets) {
        // Find the question label within the current question widget
        QLabel* questionLabel = questionWidget->findChild<QLabel*>(questionWidget->objectName() + "_LA");
        if (questionLabel) {
            qDebug() << "Processing question:" << questionLabel->text();

            QString questionText = questionLabel->text();
            QStringList checkedAnswers;

            // Find all checkboxes within the current question widget
            QList<QCheckBox*> checkboxes = questionWidget->findChildren<QCheckBox*>();
            for (QCheckBox* checkbox : checkboxes) {
                // Check if the checkbox is a direct child of the current question widget
                if (checkbox->parentWidget() == questionWidget) {
                    // If the checkbox is checked, add its text to the list of checked answers
                    if (checkbox->isChecked()) {
                        QString answerText = checkbox->text();
                        // Remove the prefix
                        answerText.remove(0, 3); // Remove the first 3 characters (e.g., "D: ")
                        qDebug() << "  Checked answer:" << answerText;
                        checkedAnswers.append(answerText);
                    }
                }
            }

            // Add the question and its checked answers to the map
            answersMap.insert(questionText, checkedAnswers);
        }
    }

    // Output the map for testing or further processing
    QMapIterator<QString, QStringList> iter(answersMap);
    while (iter.hasNext()) {
        iter.next();
        qDebug() << "Question:" << iter.key() << "Checked answers:" << iter.value();
    }
}




void MainWindow::Grade(QString question, QString answer){

}



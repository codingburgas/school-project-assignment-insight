#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "login.hpp"
MainWindow::MainWindow(LogIn* login, const QString& username_ref, const QString& role_ref, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Courses_SW->setCurrentIndex(1);

    UpdateExams();

    ui->questionType_SW->setCurrentIndex(0);

    m_username = username_ref;
    m_role = role_ref;

    ui->Navbar->setCurrentIndex(2);
    ui->Courses_SW->setCurrentIndex(1);

    UpdateHomepage();

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
    ui->Navbar->setCurrentIndex(2);
    ui->Courses_SW->setCurrentIndex(1);
}


void MainWindow::on_programmingCourses_PB_clicked()
{
    ui->Courses_SW->setCurrentIndex(2);
}


void MainWindow::on_homepage_PB_clicked()
{
    ui->Navbar->setCurrentIndex(0);
    UpdateHomepage();
}


void MainWindow::on_grades_PB_clicked()
{
    ui->Navbar->setCurrentIndex(1);
    UpdateGrades();
}


void MainWindow::on_classes_PB_clicked()
{
    ui->Navbar->setCurrentIndex(2);
    ui->Courses_SW->setCurrentIndex(1);
}


void MainWindow::on_excelSpecialistCourse_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    UpdateLessons("Excel Specialist");
    ui->subject_LA->setText("Excel Specialist");
}


void MainWindow::on_createLesson_PB_clicked()
{
    ui->Navbar->setCurrentIndex(6);
}


void MainWindow::UpdateLessons(QString subject)
{
    lessonHeadings.clear();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM lessons WHERE Subject = :subject");
    qry.bindValue(":subject", subject);

    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    while(qry.next())
    {
        lessonHeadings.push_back(qry.value("Heading").toString());
    }

    QWidget* lessonWidget = new QWidget(ui->courses_WG); // Create a new widget to hold all the lessons
    QVBoxLayout *layout = new QVBoxLayout(lessonWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop);
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
        label->setGeometry(10, 0, 400, 50);

        QPushButton *accessCourse = new QPushButton(lessonItemWidget); // Create button to access the course
        accessCourse->setFixedSize(500,50);
        accessCourse->setStyleSheet("background-color: transparent; border: 1px solid black; color: black;");
        connect(accessCourse, &QPushButton::clicked, this, [=]() { handleCourseButtons(heading); });
        if(m_role == "Teacher" || m_role == "Principal" || m_role == "Sub-Teacher")
        {
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
        }

        layout->addWidget(lessonItemWidget); // Add the lesson widget to the layout
    }

    ui->scrollArea_5->setWidget(lessonWidget); // Set the widget containing all lessons as the content of the scroll area
}

void MainWindow::on_addLesson_PB_clicked()
{
    QString heading = ui->heading_LE->text();
    QString lesson = ui->material_PTE->toPlainText();
    QString subject = ui->subject_LA->text();
    QSqlQuery qry;

    qry.prepare("INSERT INTO lessons(Subject, Heading, Lesson) "
                "VALUES (:subject, :heading, :lesson)");
    qry.bindValue(":subject", subject);
    qry.bindValue(":heading", heading);
    qry.bindValue(":lesson", lesson);
    if (qry.exec()) {
        QMessageBox::information(this, "Success", "You have successfully published a lesson.");
        UpdateLessons(subject);
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
        ui->heading_LE->setText(qry.value("Heading").toString());
        ui->material_PTE->setPlainText(qry.value("Lesson").toString());
        ui->addLesson_PB->hide();
        QPushButton* updateLesson_PB = new QPushButton(ui->Navbar->widget(6));
        updateLesson_PB->setGeometry(90, 600, 291, 31);
        updateLesson_PB->setText("Update Lesson");
        updateLesson_PB->setStyleSheet("QPushButton{background-color:#4D4C4C;color:white;border-radius:5px;}");

        updateLesson_PB->show(); // Ensure the button is visible

        // Set the widget as the parent of the button
        updateLesson_PB->setParent(ui->Navbar->widget(6));
        connect(updateLesson_PB, &QPushButton::clicked, this, [=]() { UpdateLesson(heading); });


    } else {
        qDebug() << qry.lastError();
    }
}


void MainWindow::UpdateLesson(const QString &heading)
{
    QSqlQuery qry;
    QString headingUpdated = ui->heading_LE->text();
    QString lessonUpdated = ui->material_PTE->toPlainText();
    qry.prepare("UPDATE lessons SET Heading = :newHeading, Lesson = :newLesson WHERE Heading = :oldHeading");
    qry.bindValue(":newHeading", headingUpdated);
    qry.bindValue(":newLesson", lessonUpdated);
    qry.bindValue(":oldHeading", heading);
    if(qry.exec())
    {
        ui->Navbar->setCurrentIndex(7);
        QMessageBox::information(this, "Update Successful", "Your lesson was updated successful");
    }
    else
    {
        // Handle update failure
        qDebug() << "Update failed:" << qry.lastError().text();
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

        UpdateLessons(ui->subject_LA->text());
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
    ui->Navbar->setCurrentIndex(7);
}


void MainWindow::on_goBack_lesson_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
}


void MainWindow::on_exams_PB_clicked()
{
    ui->Navbar->setCurrentIndex(3);
    ui->Exams_SW->setCurrentIndex(1);
}

void MainWindow::on_excelSpecialist_PB_clicked()
{
    ui->Navbar->setCurrentIndex(8);
    ui->createExam_SW->setCurrentIndex(3);
    ui->subject_exam_LA->setText("Excel Specialist");
}



void MainWindow::on_createExam_PB_clicked()
{
    ui->createExam_SW->setCurrentIndex(0);
}


void MainWindow::on_addQuestion_PB_clicked()
{
    ui->createExam_SW->setCurrentIndex(1);
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
    QString answer;
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
                answer = answerLineEdit->text();
            }
        }
    }

    QSqlQuery qry;
    qry.prepare("INSERT INTO questions(`Exam Name`, `Question`, `Answer1`, `Answer2`, `Answer3`, `Answer4`, `Open Answer`, `Correct Answer`, `Points`)"
                "VALUES(:examName, :question, :answer1, :answer2, :answer3, :answer4, :openAnswer, :correctAnswer, :points)");
    qry.bindValue(":examName", examName);
    qry.bindValue(":question", question);
    qry.bindValue(":answer1", answerA);
    qry.bindValue(":answer2", answerB);
    qry.bindValue(":answer3", answerC);
    qry.bindValue(":answer4", answerD);
    qry.bindValue(":openAnswer", openAnswer);
    qry.bindValue(":correctAnswer", answer);
    qry.bindValue(":points", points);
    if(!qry.exec())
    {
        qDebug() << qry.lastError();
    }
    else
    {
        QMessageBox::information(this, "Success", "Question added");
        ui->createExam_SW->setCurrentIndex(0);
        UpdateQuestions(examName);
    }

    ui->question_LE->clear();
    ui->answerA_LE->clear();
    ui->answerB_LE->clear();
    ui->answerC_LE->clear();
    ui->answerD_LE->clear();
    ui->points_LE->clear();


}

void MainWindow::UpdateExams()
{
    examNames.clear();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM exams WHERE Subject = :subject");
    qry.bindValue(":subject", ui->subject_exam_LA->text());

    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    while(qry.next())
    {
        examNames.push_back(qry.value("Exam Name").toString());
    }

    QWidget* examsWidget = new QWidget(ui->scrollAreaWidgetContents); // Create a new widget to hold all the exams
    QVBoxLayout *layout = new QVBoxLayout(examsWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop); // Align widgets to the top

    for (const QString& exam : examNames) {
        QWidget *examWidget = new QWidget; // Create a widget for each exam
        examWidget->setFixedSize(500, 50);

        QLabel *label = new QLabel(exam, examWidget); // Create label for the exam name
        label->setObjectName(exam + "_LA"); // Set object name
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(20, 0, 500, 50);

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
        questions.push_back(qry.value("Question").toString());
    }

    QWidget* questionsWidget = new QWidget(ui->scrollArea_6); // Create a new widget to hold all the questions
    QVBoxLayout *layout = new QVBoxLayout(questionsWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop);
    for (const QString& question : questions) {
        QWidget *questionWidget = new QWidget; // Create a widget for each question

        // Set fixed size for the question widget
        questionWidget->setFixedSize(800, 70);


        QLabel *label = new QLabel(question, questionWidget);
        label->setObjectName(question + "_LA"); // Set object name
        label->setStyleSheet("QLabel { color: black; font-size: 16px; }");
        label->setGeometry(20, 0, 700, 50);

        QPushButton *accessExam = new QPushButton(questionWidget); // Create button to access the exam
        accessExam->setStyleSheet("background-color: transparent; border: 1px solid black; color: black;");
        accessExam->setFixedSize(700, 50);

        QPushButton *deleteButton = new QPushButton(questionWidget); // Create button to delete the question
        QIcon closeIcon(":/Resources/Images/icons/Close.png");
        deleteButton->setIcon(closeIcon);
        deleteButton->setIconSize(QSize(30, 30));
        deleteButton->setGeometry(655, 5, 40, 40);
        deleteButton->setStyleSheet("background-color: #900C0C; border: none; color: black;");
        connect(deleteButton, &QPushButton::clicked, this, [=]() { deleteQuestion(examName); });

        QPushButton *editButton = new QPushButton(questionWidget); // Create button to edit the question
        QIcon editIcon(":/Resources/Images/icons/Edit.png");
        editButton->setIcon(editIcon);
        editButton->setIconSize(QSize(30, 30));
        editButton->setGeometry(620, 5, 40 ,40);
        editButton->setStyleSheet("background-color: transparent; border: none; color: black;");
        connect(editButton, &QPushButton::clicked, this, [=]() { editQuestion(question); });

        layout->addWidget(questionWidget); // Add the question widget to the layout
    }

    ui->scrollArea_3->setWidget(questionsWidget); // Set the widget containing all questions as the content of the scroll area

}

void MainWindow::editQuestion(const QString& examName)
{
    ui->createExam_SW->setCurrentIndex(1);
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
    questions.clear();
    ui->createExam_SW->setCurrentIndex(2);
    ui->examName_LA_2->setText(examName);

    QSqlQuery qry;
    qry.prepare("SELECT * FROM questions WHERE `Exam Name` = :examName");
    qry.bindValue(":examName", examName);
    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    while(qry.next())
    {
        questions.push_back(qry.value("Question").toString());
    }

    QWidget* questionsWidget = new QWidget(ui->ExamOverview); // Create a new widget to hold all the questions
    QVBoxLayout *layout = new QVBoxLayout(questionsWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop);
    int counter = 1;
    for (const QString& question : questions) {
        qry.prepare("SELECT * FROM questions WHERE Question = :question");
        qry.bindValue(":question", question);
        if(qry.exec())
        {
            while(qry.next()){
                QWidget *questionWidget = new QWidget; // Create a widget for each question

                // Set fixed size for the question widget
                questionWidget->setFixedSize(800, 300);
                questionsWidget->setStyleSheet("border: 1px solid black;");


                QLabel *questionNumber = new QLabel(questionWidget);
                questionNumber->setText("Question " + QString::number(counter));
                questionNumber->setStyleSheet("QLabel{background-color:transparent;qproperty-alignment: AlignLeft;color:black;font: 16pt;border:none;}");
                questionNumber->setGeometry(20, 10, 190, 50);

                QLabel *points = new QLabel("Points: ", questionWidget);
                points->setText("Points: " + qry.value("Points").toString());
                points->setObjectName(question + "_points_LA"); // Set object name
                points->setStyleSheet("QLabel{background-color:transparent;qproperty-alignment: AlignRight;color:black;font: 16pt;border:none;}");
                points->setGeometry(500, 10, 230, 60);

                QLabel *question_LA = new QLabel(question, questionWidget);
                question_LA->setObjectName(question + "_LA"); // Set object name
                question_LA->setStyleSheet("QLabel{background-color:transparent;qproperty-alignment: AlignCenter;color:black;font: 16pt;border:none;}");
                question_LA->setGeometry(0, 60, 800, 100);
                question_LA->setWordWrap(true); // Enable word wrapping



                QButtonGroup *answerGroup = new QButtonGroup(questionWidget); // Group the checkboxes together

                QCheckBox *answer1 = new QCheckBox("A: ", questionWidget);
                answer1->setObjectName(qry.value("Answer1").toString() + "_CB"); // Set object name
                answer1->setText("A: " + qry.value("Answer1").toString());
                answer1->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
                answer1->setGeometry(180, 120, 300, 70);
                answerGroup->addButton(answer1);

                QCheckBox *answer2 = new QCheckBox("B: ", questionWidget);
                answer2->setObjectName(qry.value("Answer2").toString() + "_CB"); // Set object name
                answer2->setText("B: " + qry.value("Answer2").toString());
                answer2->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
                answer2->setGeometry(500, 120, 300, 70);
                answerGroup->addButton(answer2);

                QCheckBox *answer3 = new QCheckBox("C: ", questionWidget);
                answer3->setObjectName(qry.value("Answer3").toString() + "_CB"); // Set object name
                answer3->setText("C: " + qry.value("Answer3").toString());
                answer3->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
                answer3->setGeometry(180, 180, 300, 70);
                answerGroup->addButton(answer3);

                QCheckBox *answer4 = new QCheckBox("D: ", questionWidget);
                answer4->setObjectName(qry.value("Answer4").toString() + "_CB"); // Set object name
                answer4->setText("D: " + qry.value("Answer4").toString());
                answer4->setStyleSheet("background-color:transparent;color:black;font: 16pt;border:none;");
                answer4->setGeometry(500, 180, 300, 70);
                answerGroup->addButton(answer4);

                connect(answerGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onAnswerClicked(QAbstractButton*))); // Connect the button group's signal to a slot

                layout->addWidget(questionWidget); // Add the question widget to the layout
                counter++;
            }
        }
    }

    ui->scrollArea_4->setWidget(questionsWidget); // Set the widget containing all questions as the content of the scroll area
}


void MainWindow::onAnswerClicked(QAbstractButton* button)
{
    QCheckBox* clickedBox = qobject_cast<QCheckBox*>(button);
    if (!clickedBox)
        return;

    QButtonGroup* group = clickedBox->group();
    if (!group)
        return;

    QList<QAbstractButton*> buttons = group->buttons();
    for (auto btn : buttons) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(btn);
        if (checkBox && checkBox != clickedBox) {
            checkBox->setChecked(false);
        }
    }
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
    ui->createExam_SW->setCurrentIndex(0);
    ui->examName_LA->hide();
    ui->examName_LE->hide();
   ui->examName_LE->setText(examName);
    ui->publishExam_PB->setText("Edit exam");
    UpdateQuestions(examName);
    // QWidget* QuestionsWidget = ui->scrollAreaWidgetContents_4;
    // QWidget* questionWidget = ui->question_WG;
}


void MainWindow::on_publishExam_PB_clicked()
{
    QSqlQuery qry;
    QString examName = ui->examName_LE->text();
    qry.prepare("INSERT INTO exams (Subject, `Exam Name`) "
                "VALUES(:subject, :examName)");
    qry.bindValue(":subject", ui->subject_exam_LA->text());
    qry.bindValue(":examName", examName);

    if(qry.exec())
    {
        QMessageBox::information(this, "Exam Published", "Your exam has been published");
        UpdateExams();
        ui->Navbar->setCurrentIndex(3);
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
    QList<QString> plainAnswers;

    // Step 1: Collect plain answers
    collectPlainAnswers(plainAnswers);

    // Step 2: Compare with correct answers
    compareAnswers(plainAnswers);
}

void MainWindow::collectPlainAnswers(QList<QString> &plainAnswers)
{
    for (QWidget* questionWidget : ui->scrollArea_4->findChildren<QWidget*>()) {
        // Find all checkboxes within the current question widget
        QList<QCheckBox*> checkboxes = questionWidget->findChildren<QCheckBox*>();
        for (QCheckBox* checkbox : checkboxes) {
            // Check if the checkbox is a direct child of the current question widget
            if (checkbox->parentWidget() == questionWidget) {
                // If the checkbox is checked, print its text
                if (checkbox->isChecked()) {
                    QString plainAnswer = checkbox->text().remove(0, 3);
                    plainAnswers.push_back(plainAnswer);
                }
            }
        }
    }
}

void MainWindow::compareAnswers(const QList<QString> &plainAnswers)
{
    QSqlQuery qry;
    int examPointsTotal = 0;
    int studentPoints = 0;


    qry.prepare("SELECT Points FROM questions WHERE `Exam Name` = :examName");
    qry.bindValue(":examName", ui->examName_LA_2->text());
    if(!qry.exec())
    {
        qDebug() << qry.lastError();
    }
    while(qry.next())
    {
        examPointsTotal += qry.value(0).toInt();
    }
    for (int i = 0; i < questions.size(); ++i) {
        QString question = questions[i];

        qry.prepare("SELECT * FROM questions WHERE Question = :question");
        qry.bindValue(":question", question);

        if (qry.exec() && qry.next()) {
            int points = qry.value("Points").toInt(); // Retrieve points for the current question
            QString correctAnswer = qry.value("Correct Answer").toString();

            // Iterate over plain answers
            if (i < plainAnswers.size()) {


                if (plainAnswers[i] == correctAnswer)
                    studentPoints += points;
            }

            // Insert student's exam data into database if needed
        } else {
            qDebug() << "Error retrieving data for question: " << question;
        }
    }

    int grade = static_cast<int>((static_cast<float>(studentPoints) / examPointsTotal) * 100);
    GradeExam(grade);
}

void MainWindow::GradeExam(int grade)
{
    QSqlQuery qry;
    int mark;
    if(grade < 50)
    {
        mark = 2;
    }
    else if(grade >= 50 && grade <= 60)
    {
        mark = 3;
    }
    else if(grade > 60 && grade <= 75)
    {
        mark = 4;
    }
    else if(grade > 75 && grade <= 88)
    {
        mark = 5;
    }
    else
    {
        mark = 6;
    }
    qDebug() << ui->subject_exam_LA->text();
    qry.prepare("INSERT INTO studentgrades(Username, Subject, Mark) "
                "VALUES(:username, :subject, :mark)");
    qry.bindValue(":username", m_username);
    qry.bindValue(":subject", ui->subject_exam_LA->text());
    qry.bindValue(":mark", mark);
    if(qry.exec())
    {   
        qry.prepare("SELECT `Exams Submitted Counter` FROM users WHERE Username = :username");
        qry.bindValue(":username", m_username);
        int examsDoneCounter = qry.value(0).toInt();
        examsDoneCounter++;
        if(qry.exec())
        {
            qry.prepare("UPDATE users SET `Exams Submitted Counter` = :examsDoneCounter WHERE username = :username");
            qry.bindValue(":examsDoneCounter", examsDoneCounter);
            qry.bindValue(":username", m_username);
            QMessageBox::information(this, "Exam submitted", "You submitted your exam");
            ui->Navbar->setCurrentIndex(3);
            ui->Exams_SW->setCurrentIndex(1);
            if(!qry.exec())
            {
                qDebug() << "error:" << qry.lastError();
            }
        }
    }
    else
    {
        qDebug() << qry.lastError();
    }

}



void MainWindow::UpdateHomepage()
{
    UpdatePfp();
    ui->createLesson_PB->hide();
    ui->createExam_PB->hide();
    ui->reviewRequests_PB->hide();


    QSqlQuery qry;
    int counter = 0;
    int studentGrades = 0;
    double avgGrade = 0;
    int examsSubmittedCounter = 0;
    qry.prepare("SELECT * FROM users WHERE Username = :username");
    qry.bindValue(":username", m_username);
    if(qry.exec() && qry.next())
    {
        QString firstName = qry.value("First Name").toString();
        QString lastName = qry.value("Last Name").toString();
        QString fullName = firstName + " " + lastName;
        QString role = qry.value("Role").toString();
        examsSubmittedCounter = qry.value("Exams Submitted Counter").toInt();
        ui->studentName_LA->setText(fullName);
        ui->greeting_LA->setText("Hello again, " + fullName);
        ui->role_LA->setText(role);
        qry.prepare("SELECT * FROM studentgrades WHERE Username = :username");
        qry.bindValue(":username", m_username);
        if(!qry.exec())
        {
            qDebug() << qry.lastError();
        }
        while(qry.next())
        {
            counter++;
            studentGrades += qry.value("Mark").toInt();
            qDebug() << studentGrades;
        }

        avgGrade = static_cast<double>(studentGrades) / counter;

        ui->averageGrade_LA->setText(QString::number(avgGrade));
        ui->examsSubmitted_LA->setText(QString::number(examsSubmittedCounter));
    }
    else
    {
        qDebug() << qry.lastError();
    }

    if(m_role == "Teacher" || m_role == "Principal" || m_role == "Sub-Teacher")
    {
        ui->createLesson_PB->show();
        ui->createExam_PB->show();
    }
    if(m_role == "Principal")
    {
        ui->reviewRequests_PB->show();
    }


}

void MainWindow::on_excelExpertCourses_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Excel Expert");
    UpdateLessons("Excel Expert");
}


void MainWindow::on_wordSpecialistCourses_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Word Specialist");
    UpdateLessons("Word Specialist");
}


void MainWindow::on_wordExpertCourses_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Word Expert");
    UpdateLessons("Word Expert");
}


void MainWindow::on_ITEssentialsCourses_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("IT Essentials");
    UpdateLessons("IT Essentials");
}


void MainWindow::on_dbFundamentalsCourses_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Database Fundamentals");
    UpdateLessons("Database Fundamentals");
}


void MainWindow::on_logOut_PB_clicked()
{
    this->hide();
    m_login = std::make_shared<LogIn>();
    m_login->show();
}


void MainWindow::UpdateGrades()
{
    subjects.clear();
    marks.clear(); // Clear the marks vector
    QSqlQuery qry;
    qry.prepare("SELECT Subject, Mark FROM studentgrades WHERE Username = :username");
    qry.bindValue(":username", m_username);
    if (!qry.exec()) {
        qDebug() << "Error executing query:" << qry.lastError().text();
        return;
    }

    QWidget* gradesWidget = new QWidget(ui->scrollAreaWidgetContents_7); // Create a new widget to hold all the exams
    QVBoxLayout *layout = new QVBoxLayout(gradesWidget); // Create a vertical layout for the widget
    layout->setAlignment(Qt::AlignTop); // Align widgets to the top

    QMap<QString, QList<int>> subjectMarksMap; // Map to store subject and corresponding marks
    QString color;
    while(qry.next())
    {
        QString subject = qry.value("Subject").toString();
        int mark = qry.value("Mark").toInt(); // Retrieve the mark

        // Add mark to the corresponding subject in the map
        subjectMarksMap[subject].append(mark);
    }
    // Iterate through the map to create widgets for each subject with its marks
    for (auto it = subjectMarksMap.begin(); it != subjectMarksMap.end(); ++it)
    {
        QString subject = it.key();
        QList<int> marks = it.value();

        QWidget *gradeWidget = new QWidget; // Create a widget for each subject
        gradeWidget->setFixedSize(800, 50);

        QLabel *subjectLabel = new QLabel(subject, gradeWidget); // Create label for the subject name
        subjectLabel->setObjectName(subject + "_LA"); // Set object name
        subjectLabel->setStyleSheet("QLabel { color: black; font-size: 16px; border: 1px solid black; border-radius:5px;}");
        subjectLabel->setGeometry(20, 0, 700, 50);

        QWidget *gradesContainer = new QWidget(gradeWidget); // Create container for the marks
        gradesContainer->setGeometry(500, 8, 200, 40);
        QHBoxLayout *horizontalLayout = new QHBoxLayout(gradesContainer); // Create a vertical layout for the widget

        // Iterate through marks to create and add labels for each mark
        QString color;
        for (int mark : marks) {
            QLabel* gradeLabel = new QLabel(QString::number(mark), gradesContainer);
            switch (mark)
            {
            case 2:
                color = "#000000";
                break;
            case 3:
                color = "#400C58";
                break;
            case 4:
                color = "#2D0B66";
                break;
            case 5:
                color = "#0B5666";
                break;
            case 6:
                color = "#26C281";
                break;
            }
            gradeLabel->setStyleSheet("QLabel{background-color: " + color + ";border-radius:15px;qproperty-alignment: AlignCenter; color:white}");
            gradeLabel->setFixedSize(30, 30); // Set fixed size for circular shape
            gradeLabel->setAlignment(Qt::AlignLeft); // Center the text inside the circular label
            horizontalLayout->addWidget(gradeLabel); // Add the mark label to the horizontal layout
        }



        layout->addWidget(gradeWidget); // Add the subject widget to the layout
    }



    ui->scrollArea_7->setWidget(gradesWidget); // Set the widget containing all exams as the content of the scroll area
}


void MainWindow::on_cppCourse_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("CPP");
    UpdateLessons("CPP");
}


void MainWindow::on_objCourse_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Objective C");
    UpdateLessons("Objective C");
}


void MainWindow::on_CSharpCourse_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("C Sharp");
    UpdateLessons("C Sharp");
}


void MainWindow::on_javaCourse_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Java");
    UpdateLessons("Java");
}


void MainWindow::on_pythonCourse_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Python");
    UpdateLessons("Python");
}




void MainWindow::on_javascriptCourse_PB_clicked()
{
    ui->Navbar->setCurrentIndex(7);
    ui->subject_LA->setText("Javascript");
    UpdateLessons("Javascript");
}


void MainWindow::on_settings_PB_clicked()
{
    ui->Navbar->setCurrentIndex(9);
    UpdateSettings();
}

void MainWindow::UpdateSettings()
{
    QSqlQuery qry;
    qry.prepare("SELECT * FROM users WHERE Username = :username");
    qry.bindValue(":username", m_username);
    if(qry.exec())
    {
        if(qry.next())
        {
            QString firstName = qry.value("First Name").toString();
            QString lastName = qry.value("Last Name").toString();
            QString username = qry.value("Username").toString();
            QString email = qry.value("Email").toString();
            QString bio = qry.value("Bio").toString();
            QString password = qry.value("Password").toString();
            QString phoneNumber = qry.value("Phone Number").toString();
            QString country = qry.value("Country").toString();

            ui->firstName_LE->setText(firstName);
            ui->lastName_LE->setText(lastName);
            ui->username_LE->setText(username);
            ui->bio_LE->setText(bio);
            ui->password_LE->setText(password);
            ui->phoneNumber_LE->setText(phoneNumber);
            ui->email_LE->setText(email);
        }
    }
}


void MainWindow::on_profile_PB_clicked()
{
    ui->settings_SW->setCurrentIndex(0);
}


void MainWindow::on_appearence_PB_clicked()
{
    ui->settings_SW->setCurrentIndex(1);
}


void MainWindow::on_language_PB_clicked()
{
    ui->settings_SW->setCurrentIndex(2);

}


void MainWindow::on_security_PB_clicked()
{
    ui->settings_SW->setCurrentIndex(3);

}


void MainWindow::on_changePicture_PB_clicked()
{
    QString ImagePath = QFileDialog::getOpenFileName(this, tr("Select Image"), QCoreApplication::applicationDirPath(), tr("Image Files (*.jpg *.png)"), 0);

    if (!ImagePath.isEmpty())
    {
        QPixmap Image(ImagePath);
        QPixmap scaledImage = Image.scaled(ui->pfp_settings_LA->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
                qry.prepare("UPDATE users SET pfp = :pfp WHERE Username = :username");
                qry.bindValue(":pfp", FinalDataToSave);
                qry.bindValue(":username", m_username);

                if (qry.exec())
                {
                    QSqlDatabase::database().commit();
                    ui->pfp_settings_LA->setPixmap(scaledImage);
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Unsupported Format"), tr("Only JPG and PNG files are supported."));
            }
        }
    }
    else
    {
        qDebug() << "invalid image";
    }
}

void MainWindow::UpdatePfp()
{
    QLabel *imageLabel = ui->pfp_settings_LA;
    QLabel *imageLabel2 = ui->pfp_LA;
    QFrame *frame = ui->navbar_FR;
    QFrame *frame2 = ui->settings_FR;

    QSqlQuery query;
    query.prepare("SELECT pfp FROM users WHERE Username = :username");
    query.bindValue(":username", m_username);

    if (query.exec() && query.first())
    {
        QByteArray imageData = query.value("pfp").toByteArray();
        QPixmap userPixmap;
        userPixmap.loadFromData(QByteArray::fromBase64(imageData));

        if (!userPixmap.isNull())
        {
            // Set up circular mask and style for profile picture
            SetCircularMaskAndStyle(frame, imageLabel, userPixmap);
            SetCircularMaskAndStyle(frame2, imageLabel2, userPixmap);
        }
        else
        {
            qDebug() << "Invalid or empty image data.";
        }
    }
}

void MainWindow::SetCircularMaskAndStyle(QFrame* frame, QLabel* imageLabel, const QPixmap& userPixmap)
{
    // Set up circular mask
    QBitmap mask(frame->size());
    mask.fill(Qt::color0); // Transparent mask
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::color1); // Fill with opaque color
    painter.drawEllipse(mask.rect());
    frame->setMask(mask);

    // Set circular style for the frame
    frame->setStyleSheet("background-color: white; border-radius: " + QString::number(frame->width() / 2) + "px;");

    // Scale the pixmap to fit within the circular frame and stretch if necessary
    QPixmap scaledPixmap = userPixmap.scaled(frame->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Set parent and geometry for the imageLabel
    imageLabel->setParent(frame);
    imageLabel->setGeometry(0, 0, frame->width(), frame->height());

    // Set the scaled pixmap for the imageLabel
    imageLabel->setPixmap(scaledPixmap);
}



void MainWindow::on_reviewRequests_PB_clicked()
{
    ui->homepage_SW->setCurrentIndex(1);
    UpdateRequests();

}

void MainWindow::UpdateRequests()
{
    QSqlQuery qry;
    QLabel* imageLabel = ui->proof_LA;
    qry.prepare("SELECT * FROM accessrequests");
    if(qry.exec() && qry.next())
    {
        QString username = qry.value("Username").toString();
        QString role = qry.value("Access Requested").toString();
        QByteArray imageData = qry.value("Document Proof").toByteArray();
        QPixmap userPixmap;
        userPixmap.loadFromData(QByteArray::fromBase64(imageData));
        if (!userPixmap.isNull())
        {
            // Set up circular mask and style for profile picture
            imageLabel->setPixmap(userPixmap);
        }
        else
        {
            qDebug() << "Invalid or empty image data.";
        }
        ui->candidatRole_LA->setText("Role Requested: " + role);
        ui->candidatUsername_LE->setText(username);
    }
}



void MainWindow::on_approveRequest_PB_clicked()
{
    QString username = ui->candidatUsername_LE->text();
    QString roleRequested = ui->candidatRole_LA->text();
    roleRequested.erase(roleRequested.begin(), roleRequested.begin() + 16);
    QSqlQuery qry;
    qry.prepare("UPDATE users SET Role = :newRole WHERE Username = :username");
    qry.bindValue(":username", username);
    qry.bindValue(":newRole", roleRequested);
    if(qry.exec() && qry.next())
    {
        QMessageBox::information(this, "User promoted", "The user you have approved is now promoted");
    }
    else
    {
        qDebug() <<qry.lastError();
    }
    ui->homepage_SW->setCurrentIndex(0);
}


void MainWindow::on_ignoreRequest_PB_clicked()
{
    ui->homepage_SW->setCurrentIndex(0);
}


void MainWindow::on_ITExams_PB_clicked()
{
    ui->Exams_SW->setCurrentIndex(1);

}

void MainWindow::on_programmingExams_PB_clicked()
{
    ui->Exams_SW->setCurrentIndex(2);

}
void MainWindow::on_naturalExams_PB_clicked()
{
    ui->Navbar->setCurrentIndex(3);
    ui->Exams_SW->setCurrentIndex(3);
}


void MainWindow::on_socialExams_PB_clicked()
{
    ui->Navbar->setCurrentIndex(3);
    ui->Exams_SW->setCurrentIndex(4);

}

void MainWindow::on_naturalSciences_PB_clicked()
{
    ui->Courses_SW->setCurrentIndex(3);

}


void MainWindow::on_socialSciences_PB_clicked()
{
    ui->Courses_SW->setCurrentIndex(4);
}


void MainWindow::on_cppExams_PB_clicked()
{
    ui->Navbar->setCurrentIndex(8);
    ui->createExam_SW->setCurrentIndex(3);
    ui->subject_exam_LA->setText("Programming C++");
    UpdateExams();
}


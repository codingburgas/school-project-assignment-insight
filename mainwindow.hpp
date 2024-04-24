#pragma once

#include <QWidget>
#include <QPainter>
#include <QSqlError>
#include <QLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QComboBox>
#include <QButtonGroup>

namespace Ui {
class MainWindow;
}

class LogIn;

class CreateLesson;
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(LogIn* login, const QString& username_ref, const QString& role_ref, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ITCourses_PB_clicked();

    void on_programmingCourses_PB_clicked();

    void on_homepage_PB_clicked();

    void on_grades_PB_clicked();

    void on_classes_PB_clicked();

    void on_excelSpecialistCourse_PB_clicked();

    void on_createLesson_PB_clicked();

    void handleCourseButtons(const QString &heading);

    void on_addLesson_PB_clicked();

    void on_goBack_PB_clicked();

    void deleteLesson(const QString &heading);

    void on_goBack_lesson_PB_clicked();

    void editLesson(const QString& heading);

    void on_excelSpecialist_PB_clicked();

    void on_exams_PB_clicked();

    void on_createExam_PB_clicked();

    void on_addQuestion_PB_clicked();

    void on_createQuestion_PB_clicked();

    void deleteExam(const QString& examName);
    void editExam(const QString& examName);
    void editQuestion(const QString& examName);
    void deleteQuestion(const QString& examName);
    void UpdateGrades();


    void on_publishExam_PB_clicked();

    void on_closedAnswers_CB_stateChanged(int arg1);
    void EnterExam(const QString& examName);



    void on_submitExam_PB_clicked();

    void onAnswerClicked(QAbstractButton* button);


    void on_excelExpertCourses_PB_clicked();

    void on_wordSpecialistCourses_PB_clicked();

    void on_wordExpertCourses_PB_clicked();

    void on_ITEssentialsCourses_PB_clicked();

    void on_dbFundamentalsCourses_PB_clicked();

    void UpdateLesson(const QString& heading);

    void on_logOut_PB_clicked();

    void on_cppCourse_PB_clicked();

    void on_objCourse_PB_clicked();

    void on_CSharpCourse_PB_clicked();

    void on_javaCourse_PB_clicked();

    void on_pythonCourse_PB_clicked();

    void on_javascriptCourse_PB_clicked();

private:
    Ui::MainWindow *ui;
    virtual void paintEvent(QPaintEvent*) override;

    void UpdateLessons(QString subject);
    void UpdateExams();
    void UpdateQuestions(const QString& examName);

    std::vector<QString> lessonHeadings;
    std::vector<QString> lessonText;
    std::vector<QString> examNames;
    std::vector<QString> questions;
    std::vector<QString> subjects;
    std::vector<int> marks;
    QString points;
    QString m_username;
    QString m_role;

    std::shared_ptr<LogIn> m_login;

    void UpdateHomepage();
    void collectPlainAnswers(QList<QString> &plainAnswers);
    void compareAnswers(const QList<QString> &plainAnswers);
    void GradeExam(int grade);
};

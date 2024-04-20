#pragma once

#include <QWidget>
#include <QPainter>
#include <QSqlError>
#include <QLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class CreateLesson;
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ITCourses_PB_clicked();

    void on_programmingCourses_PB_clicked();

    void on_homepage_PB_clicked();

    void on_schedule_PB_clicked();

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

    void accessExam(const QString& examName);
    void deleteExam(const QString& examName);
    void editExam(const QString& examName);
    void accessQuestion(const QString& examName);
    void editQuestion(const QString& examName);
    void deleteQuestion(const QString& examName);


    void on_publishExam_PB_clicked();

    void on_closedAnswers_CB_stateChanged(int arg1);


private:
    Ui::MainWindow *ui;
    virtual void paintEvent(QPaintEvent*) override;

    void UpdateLessons();
    void UpdateExams();
    void UpdateQuestions(const QString& examName);
    std::vector<QString> lessonHeadings;
    std::vector<QString> lessonText;
    std::vector<QString> examNames;
    std::vector<QString> questions;
};

#pragma once

#include <QWidget>
#include <QPainter>
#include <QSqlError>
#include <QLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QMessageBox>
#include <algorithm>
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

private:
    Ui::MainWindow *ui;
    virtual void paintEvent(QPaintEvent*) override;

    void UpdateLessons();

    std::vector<QString> lessonHeadings;
    std::vector<QString> lessonText;

    QMap<QString, QList<QWidget*>> uiElementsMap; // Map to store UI elements associated with each lesson


};

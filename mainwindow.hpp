#pragma once

#include <QWidget>
#include <QPainter>
#include <QSqlError>
#include <QLayout>
#include <QLabel>
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

    void handleCourseButtons(const QString &buttonName);

private:
    Ui::MainWindow *ui;
    virtual void paintEvent(QPaintEvent*) override;

    std::shared_ptr<CreateLesson> m_CreateLesson;
};

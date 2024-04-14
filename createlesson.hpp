#pragma once

#include <QWidget>
#include <QSqlQuery>
#include <QSql>
#include <QMessageBox>
namespace Ui {
class CreateLesson;
}

class CreateLesson : public QWidget
{
    Q_OBJECT

public:
    explicit CreateLesson(QWidget *parent = nullptr);
    ~CreateLesson();

private slots:
    void on_createLesson_PB_clicked();

private:
    Ui::CreateLesson *ui;
};

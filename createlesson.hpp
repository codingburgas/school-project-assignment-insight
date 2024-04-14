#pragma once

#include <QWidget>

namespace Ui {
class CreateLesson;
}

class CreateLesson : public QWidget
{
    Q_OBJECT

public:
    explicit CreateLesson(QWidget *parent = nullptr);
    ~CreateLesson();

private:
    Ui::CreateLesson *ui;
};

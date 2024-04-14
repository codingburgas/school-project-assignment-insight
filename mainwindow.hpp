#pragma once

#include <QWidget>
#include <QPainter>
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


private:
    Ui::MainWindow *ui;
    virtual void paintEvent(QPaintEvent*) override;

};

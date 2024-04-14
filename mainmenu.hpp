#pragma once

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QFontDatabase>

class LogIn;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainMenu;
}
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:

    void on_LogIn_PB_clicked();
private:
    Ui::MainMenu *ui;


    virtual void paintEvent(QPaintEvent*) override;

    std::shared_ptr<LogIn> m_logIn;
};

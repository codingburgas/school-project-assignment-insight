#include "createlesson.hpp"
#include "ui_createlesson.h"

CreateLesson::CreateLesson(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateLesson)
{
    ui->setupUi(this);
}

CreateLesson::~CreateLesson()
{
    delete ui;
}

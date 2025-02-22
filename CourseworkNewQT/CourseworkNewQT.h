#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CourseworkNewQT.h"

class CourseworkNewQT : public QMainWindow
{
    Q_OBJECT

public:
    CourseworkNewQT(QWidget *parent = nullptr);
    ~CourseworkNewQT();

private:
    Ui::CourseworkNewQTClass ui;
};

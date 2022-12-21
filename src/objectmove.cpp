#include "objectmove.hpp"
#include "ui_objectmove.h"

#include <QErrorMessage>

ObjectMove::ObjectMove(QWidget *parent) : QDialog(parent), ui(new Ui::ObjectMove)
{
    ui->setupUi(this);
    this->setWindowTitle("Изменение положения детали");
}

ObjectMove::~ObjectMove()
{
    delete ui;
}

int ObjectMove::getNewXCell()
{
    return newXCell;
}

int ObjectMove::getNewYCell()
{
    return newYCell;
}


void ObjectMove::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit_4->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    newXCell = gotText.toInt();

    gotText = this->ui->lineEdit_3->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    newYCell = gotText.toInt();

    if (newXCell <= 0 || newYCell <= 0)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Введите натуральные числа!");
        return;
    }
    else
        status = OK;

    this->close();
}

void ObjectMove::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}

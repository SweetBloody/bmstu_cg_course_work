#include "addlight.hpp"
#include "ui_addlight.h"

#include <QErrorMessage>

AddLight::AddLight(QWidget *parent) : QDialog(parent), ui(new Ui::AddLight)
{
    ui->setupUi(this);
    this->setWindowTitle("Расположение источника света");
}

AddLight::~AddLight()
{
    delete ui;
}

void AddLight::setXAngle(int degrees)
{
    xAngle = degrees;
}

int AddLight::getXAngle()
{
    return xAngle;
}

void AddLight::setYAngle(int degrees)
{
    yAngle = degrees;
}

int AddLight::getYAngle()
{
    return yAngle;
}

void AddLight::on_buttonBox_accepted()
{
    status = CANCEL;
    bool xAngleOk, yAngleOk;

    QString gotText = this->ui->XAngleInp->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xAngle = gotText.toInt(&xAngleOk);

    gotText = this->ui->YAngleInp->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yAngle = gotText.toInt(&yAngleOk);

    if (!xAngleOk || !yAngleOk)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Введите целые числа!");
        return;
    }
    else
        status = OK;

    this->close();
}


void AddLight::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}

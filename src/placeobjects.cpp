#include "placeobjects.h"
#include "ui_addbrick.h"
#include "ui_addplate.h"
#include "ui_addtile.h"
#include "ui_addarc.h"
#include "ui_addcylinder.h"

#include <QErrorMessage>
#include <QDebug>


AddBrick::AddBrick(QWidget *parent) : QDialog(parent), ui(new Ui::AddBrick)
{
    ui->setupUi(this);
    this->setWindowTitle("Расположение кирпичика");
}

AddBrick::~AddBrick() { delete ui; }

void AddBrick::setXCell(int xCell_) { xCell = xCell_; }

int AddBrick::getXCell() { return xCell; }

void AddBrick::setYCell(int yCell_) { yCell = yCell_; }

int AddBrick::getYCell() { return yCell; }

int AddBrick:: getModelLength() { return modelLength; }

int AddBrick::getModelWidth() { return modelWidth; }

void AddBrick::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    gotText = ui->lineEdit_3->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelLength = gotText.toInt();

    gotText = ui->lineEdit_4->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelWidth = gotText.toInt();

    if (xCell <= 0 || yCell <= 0)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Введите натуральные числа!");
        return;
    }
    else if (modelLength <= 0 || modelWidth <= 0)
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

void AddBrick::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}




AddPlate::AddPlate(QWidget *parent) : QDialog(parent), ui(new Ui::AddPlate)
{
    ui->setupUi(this);
    this->setWindowTitle("Расположение пластинки");
}

AddPlate::~AddPlate() { delete ui; }

void AddPlate::setXCell(int xCell_) { xCell = xCell_; }

int AddPlate::getXCell() { return xCell; }

void AddPlate::setYCell(int yCell_) { yCell = yCell_; }

int AddPlate::getYCell() { return yCell; }

int AddPlate:: getModelLength() { return modelLength; }

int AddPlate::getModelWidth() { return modelWidth; }

void AddPlate::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    gotText = ui->lineEdit_3->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelLength = gotText.toInt();

    gotText = ui->lineEdit_4->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelWidth = gotText.toInt();

    if (xCell <= 0 || yCell <= 0)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Введите натуральные числа!");
        return;
    }
    else if (modelLength <= 0 || modelWidth <= 0)
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

void AddPlate::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}






AddTile::AddTile(QWidget *parent) : QDialog(parent), ui(new Ui::AddTile)
{
    ui->setupUi(this);
    this->setWindowTitle("Расположение плитки");
}

AddTile::~AddTile() { delete ui; }

void AddTile::setXCell(int xCell_) { xCell = xCell_; }

int AddTile::getXCell() { return xCell; }

void AddTile::setYCell(int yCell_) { yCell = yCell_; }

int AddTile::getYCell() { return yCell; }

int AddTile:: getModelLength() { return modelLength; }

int AddTile::getModelWidth() { return modelWidth; }

void AddTile::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    gotText = ui->lineEdit_3->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelLength = gotText.toInt();

    gotText = ui->lineEdit_4->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    modelWidth = gotText.toInt();

    if (xCell <= 0 || yCell <= 0)
    {
        this->close();
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Введите натуральные числа!");
        return;
    }
    else if (modelLength <= 0 || modelWidth <= 0)
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

void AddTile::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}




AddArc::AddArc(QWidget *parent) : QDialog(parent), ui(new Ui::AddArc)
{
    ui->setupUi(this);
    this->setWindowTitle("Расположение арки");
}

AddArc::~AddArc() { delete ui; }

void AddArc::setXCell(int xCell_) { xCell = xCell_; }

int AddArc::getXCell() { return xCell; }

void AddArc::setYCell(int yCell_) { yCell = yCell_; }

int AddArc::getYCell() { return yCell; }

int AddArc:: getModelLength() { return modelLength; }

int AddArc::getModelWidth() { return modelWidth; }

void AddArc::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    if (xCell <= 0 || yCell <= 0)
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

void AddArc::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}





AddCylinder::AddCylinder(QWidget *parent) : QDialog(parent), ui(new Ui::AddCylinder)
{
    ui->setupUi(this);
    this->setWindowTitle("Расположение цилиндра");
}

AddCylinder::~AddCylinder() { delete ui; }

void AddCylinder::setXCell(int xCell_) { xCell = xCell_; }

int AddCylinder::getXCell() { return xCell; }

void AddCylinder::setYCell(int yCell_) { yCell = yCell_; }

int AddCylinder::getYCell() { return yCell; }

int AddCylinder:: getModelLength() { return modelLength; }

int AddCylinder::getModelWidth() { return modelWidth; }

void AddCylinder::on_buttonBox_accepted()
{
    status = CANCEL;

    QString gotText = this->ui->lineEdit->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    xCell = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    if (gotText == "")
    {
        this->close();
        return;
    }
    yCell = gotText.toInt();

    if (xCell <= 0 || yCell <= 0)
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

void AddCylinder::on_buttonBox_rejected()
{
    status = CANCEL;
    this->close();
}

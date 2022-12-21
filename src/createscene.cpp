#include "createscene.hpp"
#include "ui_createscene.h"

#include <QErrorMessage>

CreateScene::CreateScene(QWidget *parent) : QDialog(parent), ui(new Ui::CreateScene)
{
    ui->setupUi(this);
    this->setWindowTitle("Размеры платформы");
}

CreateScene::~CreateScene() { delete ui; }

void CreateScene::on_buttonBox_accepted()
{
    QString gotText = this->ui->lineEdit->text();
    int heightValue = gotText.toInt();

    gotText = this->ui->lineEdit_2->text();
    int widthValue = gotText.toInt();

    if (heightValue > 0 && widthValue > 0)
    {
        height = heightValue;
        width = widthValue;
    }
    else
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Введите натуральные числа!");
    }
    this->close();
}

void CreateScene::setWidth(int width_) { width = width_; }

int CreateScene::getWidth() { return width; }

void CreateScene::setheight(int height_) { height = height_; }

int CreateScene::getheight() { return height; }

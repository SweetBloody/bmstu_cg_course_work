#include "objectdelete.hpp"
#include "ui_objectdelete.h"

#include <QErrorMessage>
#include <QDebug>

ObjectDelete::ObjectDelete(SceneInf *scene_, QWidget *parent)
    : QDialog(parent), ui(new Ui::ObjectDelete)
{
    ui->setupUi(this);
    this->setWindowTitle("Удаление объектов сцены");

    scene = scene_;

    PolygonModel model;
    int count = 0;

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        count++;
        ui->listWidget->addItem(
                QString::number(count) + ". " +
                                scene->getModel(i).getName()       + " - (" +
                QString::number(scene->getModel(i).getUsedXCell() + 1) + "; " +
                QString::number(scene->getModel(i).getUsedYCell() + 1) + "; " +
                QString::number(scene->getModel(i).getUsedZCell()) + ")");
    }

    for (size_t i = 0; i < scene->getLightNum(); i++)
        ui->listWidget->addItem(
                    QString::number(count + i + 1)                       + ". Источ. света (" +
                    QString::number(scene->getLight(i).getXAngle()) + "°; " +
                    QString::number(scene->getLight(i).getYAngle()) + "°)");
}

ObjectDelete::~ObjectDelete() { delete ui; }

void ObjectDelete::on_pushButton_clicked()
{
    size_t curRow = size_t(this->ui->listWidget->currentRow());
    if (curRow < 0)
        return;

    size_t modelsNum = scene->getModelsNum();
    bool flag = true;
    PolygonModel model;

    if (curRow < modelsNum)
    {
        int xCell, yCell, modelLength, modelWidth;
        double zCell;

        model = scene->getModel(curRow);
        xCell = model.getUsedXCell();
        yCell = model.getUsedYCell();
        zCell = model.getUsedZCell();
        modelLength = model.getLengthModel();
        modelWidth = model.getWidthModel();

        for (int i = xCell; i < xCell + modelLength; i++)
        {
            for (int j = yCell; j < yCell + modelWidth; j++)
            {
                if (zCell < scene->getUsedCellsZ()[j][i].back() || \
                    (scene->getUsedCellsZ()[j][i].back() == -1.0 && zCell < scene->getUsedCellsZ()[j][i].at(scene->getUsedCellsZ()[j][i].size() - 2)))
                {
                    flag = false;
                    break;
                }
            }
        }
        if (flag)
        {
            scene->deleteModel(curRow);
            modelsNum --;
        }
        else
        {
            QErrorMessage *err = new QErrorMessage();
            err->showMessage("Невозможно удалить!\n"
                             "Над этим элементом находятся\n"
                             "другие элементы LEGO!");
        }

        recalculationModelsNum();
    }
    else
    {
        scene->deleteLight(curRow - modelsNum);
    }

    close();
}


void ObjectDelete::recalculationModelsNum()
{
    size_t modelsNum = scene->getModelsNum();
    PolygonModel model;
    int cur = 0;

    for (size_t i = 0; i < modelsNum; i++)
    {
        scene->getModel(cur).setModelNum(i);
        model = scene->getModel(cur);
    }
}

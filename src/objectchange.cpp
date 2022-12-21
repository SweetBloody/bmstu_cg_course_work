#include "objectchange.hpp"
#include "ui_objectchange.h"

#include "objectmove.hpp"

#include <QErrorMessage>
#include <QDebug>

ObjectChange::ObjectChange(Facade *facade_, QWidget *parent)
    : QDialog(parent), ui(new Ui::ObjectChange)
{
    ui->setupUi(this);
    this->setWindowTitle("Перемещение объектов сцены");

    facade = facade_;
    SceneInf *scene = facade->getScene();

    PolygonModel model;
    int count = 0;

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        count++;
        ui->listWidget->addItem(
                QString::number(count) + ". " +
                                scene->getModel(i).getName()      + " - (" +
                QString::number(scene->getModel(i).getUsedXCell() + 1) + "; " +
                QString::number(scene->getModel(i).getUsedYCell() + 1) + "; " +
                QString::number(scene->getModel(i).getUsedZCell()) + ")");
    }
}

ObjectChange::~ObjectChange() { delete ui; }

void ObjectChange::on_pushButton_clicked()
{
    size_t curRow = size_t(this->ui->listWidget->currentRow());
    if (curRow < 0)
        return;

    SceneInf *scene = facade->getScene();
    size_t modelsNum = scene->getModelsNum();
    PolygonModel model;
    bool flag = true;

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
        ObjectMove objectMoveWindow(nullptr);
        objectMoveWindow.setModal(true);
        objectMoveWindow.exec();

        if (objectMoveWindow.status == ObjectMove::OK)
        {
            scene->clearUsedCellsZ(curRow);
            int ch = changeModel(scene->getModel(curRow),
                        objectMoveWindow.getNewXCell(),
                        objectMoveWindow.getNewYCell());
           if (ch == 1)
           {
               close();
               scene->markUsedCellsZ(curRow);
               QErrorMessage *err = new QErrorMessage();
               err->showMessage("На некоторых ячейках стоит плитка!\n"
                                "На плитку нельзя ставить элементы LEGO!");
               return;
           }
           else if(ch == 2)
           {
               close();
               scene->markUsedCellsZ(curRow);
               QErrorMessage *err = new QErrorMessage();
               err->showMessage("Объект не помещается на сцене!");
               return;
           }
           else
           {
               scene->deleteModel(curRow);
               scene->markUsedCellsZ(scene->getModelsNum() - 1);
               scene->printUsedCellsZ();
           }
           modelsNum --;
           recalculationModelsNum();

        }
        else
            return;
    }
    else
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Невозможно переместить!\n"
                         "Над этим элементом находятся\n"
                         "другие элементы LEGO!");
    }
    close();
}

int ObjectChange::changeModel(PolygonModel &model, int newXCell, int newYCell)
{
    int ch = 0;

    PolygonModel::model_t modelType_ = model.getModelType();

    if (modelType_ == PolygonModel::model_t::Brick)
    {
        ch = facade->addBrick(newXCell, newYCell,
                                   model.getLengthModel(),
                                   model.getWidthModel());
    }
    else if (modelType_ == PolygonModel::model_t::Plate)
    {
        ch = facade->addPlate(newXCell, newYCell,
                                   model.getLengthModel(),
                                   model.getWidthModel());
    }
    else if (modelType_ == PolygonModel::model_t::Tile)
    {
        ch = facade->addTile(newXCell, newYCell,
                                   model.getLengthModel(),
                                   model.getWidthModel());
    }
    else if (modelType_ == PolygonModel::model_t::Arc41)
    {
        ch = facade->addArc41(newXCell, newYCell);
    }
    else if (modelType_ == PolygonModel::model_t::Arc14)
    {
        ch = facade->addArc14(newXCell, newYCell);
    }
    else if (modelType_ == PolygonModel::model_t::Cylinder1)
    {
        ch = facade->addCylinder1(newXCell, newYCell);
    }
    else if (modelType_ == PolygonModel::model_t::Cylinder2)
    {
        ch = facade->addCylinder2(newXCell, newYCell);
    }

    return ch;
}

void ObjectChange::recalculationModelsNum()
{
    SceneInf *scene     = facade->getScene();
    size_t modelsNum = scene->getModelsNum();
    PolygonModel model;
    int cur = 0;

    for (size_t i = 0; i < modelsNum; i++)
    {
        scene->getModel(cur).setModelNum(i);
        model = scene->getModel(cur);
    }
}

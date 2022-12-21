#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDebug"
#include <QErrorMessage>
#include <QShortcut>
#include <QTimer>

#include "config.hpp"

#include "addlight.hpp"
#include "objectdelete.hpp"
#include "objectchange.hpp"
#include "placeobjects.h"
#include "createscene.hpp"
#include "specialgraphicsview.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    facade = new Facade;

    QListWidgetItem *brick     = new QListWidgetItem(" Кирпичик");
    ui->listWidget->addItem(brick);

    QListWidgetItem *plate       = new QListWidgetItem(" Пластинка");
    ui->listWidget->addItem(plate);

    QListWidgetItem *tile       = new QListWidgetItem(" Плитка");
    ui->listWidget->addItem(tile);

    QListWidgetItem *arc41       = new QListWidgetItem(" Арка (4х1)");
    ui->listWidget->addItem(arc41);

    QListWidgetItem *arc14       = new QListWidgetItem(" Арка (1x4)");
    ui->listWidget->addItem(arc14);

    QListWidgetItem *cylinder1       = new QListWidgetItem(" Цилиндр (1x1)");
    ui->listWidget->addItem(cylinder1);

    QListWidgetItem *cylinder2       = new QListWidgetItem(" Цилиндр (2x2)");
    ui->listWidget->addItem(cylinder2);

    QListWidgetItem *flashlight = new QListWidgetItem(" Источник света");
    ui->listWidget->addItem(flashlight);


    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QShortcut *shortcutDown = new QShortcut(QKeySequence("down"), this);
    QObject::connect(shortcutDown, SIGNAL(activated()), this, SLOT(pictureDown()));

    QShortcut *shortcutUp = new QShortcut(QKeySequence("up"), this);
    QObject::connect(shortcutUp, SIGNAL(activated()), this, SLOT(pictureUp()));

    QShortcut *shortcutLeft = new QShortcut(QKeySequence("left"), this);
    QObject::connect(shortcutLeft, SIGNAL(activated()), this, SLOT(pictureLeft()));

    QShortcut *shortcutRight = new QShortcut(QKeySequence("right"), this);
    QObject::connect(shortcutRight, SIGNAL(activated()), this, SLOT(pictureRight()));

    QShortcut *shortcutScaleUp = new QShortcut(QKeySequence("+"), this);
    QObject::connect(shortcutScaleUp, SIGNAL(activated()), this, SLOT(pictureScaleUp()));

    QShortcut *shortcutScaleDown = new QShortcut(QKeySequence("-"), this);
    QObject::connect(
        shortcutScaleDown, SIGNAL(activated()), this, SLOT(pictureScaleDown()));

    QShortcut *shortcutRotateXRight = new QShortcut(QKeySequence("s"), this);
    QObject::connect(
        shortcutRotateXRight, SIGNAL(activated()), this, SLOT(pictureRotateXRight()));

    QShortcut *shortcutRotateXLeft = new QShortcut(QKeySequence("w"), this);
    QObject::connect(
        shortcutRotateXLeft, SIGNAL(activated()), this, SLOT(pictureRotateXLeft()));

    QShortcut *shortcutRotateYRight = new QShortcut(QKeySequence("a"), this);
    QObject::connect(
        shortcutRotateYRight, SIGNAL(activated()), this, SLOT(pictureRotateYRight()));

    QShortcut *shortcutRotateYLeft = new QShortcut(QKeySequence("d"), this);
    QObject::connect(
        shortcutRotateYLeft, SIGNAL(activated()), this, SLOT(pictureRotateYLeft()));

    QShortcut *shortcutRotateZRight = new QShortcut(QKeySequence("q"), this);
    QObject::connect(
        shortcutRotateZRight, SIGNAL(activated()), this, SLOT(pictureRotateZRight()));

    QShortcut *shortcutRotateZLeft = new QShortcut(QKeySequence("e"), this);
    QObject::connect(
        shortcutRotateZLeft, SIGNAL(activated()), this, SLOT(pictureRotateZLeft()));
}

MainWindow::~MainWindow() { delete ui; }


void MainWindow::pictureDown()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->moveDownScene(MOVE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureUp()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->moveUpScene(MOVE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureLeft()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->moveLeftScene(MOVE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRight()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->moveRightScene(MOVE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureScaleUp()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->scaleScene(SCALE_VAL + 1, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureScaleDown()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->scaleScene(1 - SCALE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateXRight()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->rotateXScene(ROTATE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateXLeft()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->rotateXScene(-ROTATE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateYRight()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->rotateYScene(ROTATE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateYLeft()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->rotateYScene(-ROTATE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateZRight()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->rotateZScene(ROTATE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureRotateZLeft()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->rotateZScene(-ROTATE_VAL, ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::pictureToCenter()
{
    if (!facade->isSceneSet())
        return;

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();

    QGraphicsScene *setScene = facade->toCenter(ui->graphicsView->rect());
    ui->graphicsView->setScene(setScene);
}

void MainWindow::on_pushButton_createScene_clicked()
{
    CreateScene Window(nullptr);
    Window.setModal(true);
    Window.exec();

    if (Window.getWidth() == -1 || Window.getheight() == -1)
        return;

    facade->setSceneInf(Window.getWidth(), Window.getheight());
    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}

void MainWindow::on_pushButton_addModel_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    int curRow = this->ui->listWidget->currentRow();
    if (curRow < 0)
        return;

    if (curRow >= 0 && curRow < 7)
    {
        int ch = 0;

        if (curRow == 0)
        {
            AddBrick placeBrickWindow(nullptr);
            placeBrickWindow.setModal(true);
            placeBrickWindow.exec();

            if (placeBrickWindow.status == AddBrick::OK)
                ch = facade->addBrick(
                        placeBrickWindow.getXCell(),
                        placeBrickWindow.getYCell(),
                        placeBrickWindow.getModelLength(),
                        placeBrickWindow.getModelWidth());
            else
                return;
        }
        else if (curRow == 1)
        {
            AddPlate placePlateWindow(nullptr);
            placePlateWindow.setModal(true);
            placePlateWindow.exec();

            if (placePlateWindow.status == AddPlate::OK)
                ch = facade->addPlate(
                        placePlateWindow.getXCell(),
                        placePlateWindow.getYCell(),
                        placePlateWindow.getModelLength(),
                        placePlateWindow.getModelWidth());
            else
                return;
        }
        else if (curRow == 2)
        {
            AddTile placeTileWindow(nullptr);
            placeTileWindow.setModal(true);
            placeTileWindow.exec();

            if (placeTileWindow.status == AddTile::OK)
                ch = facade->addTile(
                        placeTileWindow.getXCell(),
                        placeTileWindow.getYCell(),
                        placeTileWindow.getModelLength(),
                        placeTileWindow.getModelWidth());
            else
                return;
        }
        else if (curRow == 3)
        {
            AddArc placeArcWindow(nullptr);
            placeArcWindow.setModal(true);
            placeArcWindow.exec();

            if (placeArcWindow.status == AddArc::OK)
                ch = facade->addArc41(
                        placeArcWindow.getXCell(),
                        placeArcWindow.getYCell());
            else
                return;
        }
        else if (curRow == 4)
        {
            AddArc placeArcWindow(nullptr);
            placeArcWindow.setModal(true);
            placeArcWindow.exec();

            if (placeArcWindow.status == AddArc::OK)
                ch = facade->addArc14(
                        placeArcWindow.getXCell(),
                        placeArcWindow.getYCell());
            else
                return;
        }
        else if (curRow == 5)
        {
            AddCylinder placeCylinderWindow(nullptr);
            placeCylinderWindow.setModal(true);
            placeCylinderWindow.exec();

            if (placeCylinderWindow.status == AddCylinder::OK)
                ch = facade->addCylinder1(
                        placeCylinderWindow.getXCell(),
                        placeCylinderWindow.getYCell());
            else
                return;
        }
        else if (curRow == 6)
        {
            AddCylinder placeCylinderWindow(nullptr);
            placeCylinderWindow.setModal(true);
            placeCylinderWindow.exec();

            if (placeCylinderWindow.status == AddCylinder::OK)
                ch = facade->addCylinder2(
                        placeCylinderWindow.getXCell(),
                        placeCylinderWindow.getYCell());
            else
                return;
        }


        if (ch == 1)
        {
            QErrorMessage *err = new QErrorMessage();
            err->showMessage("На некоторых ячейках стоит плитка!\n"
                             "На плитку нельзя ставить элементы LEGO!");
            return;
        }
        else if(ch == 2)
        {
            QErrorMessage *err = new QErrorMessage();
            err->showMessage("Объект не помещается на сцене!");
            return;
        }
    }
    else
    {
        AddLight lightPlaceWindow(nullptr);
        lightPlaceWindow.setModal(true);
        lightPlaceWindow.exec();

        if (lightPlaceWindow.status == AddLight::OK)
            facade->addLight(
                lightPlaceWindow.getXAngle(),
                lightPlaceWindow.getYAngle());
        else
            return;
    }

    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}


void MainWindow::on_pushButton_addCastle_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    int ch = 0;

    ch += facade->addPlate(3, 3, 10, 12);
    if (ch != 0)
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("На некоторых ячейках стоит плитка\n"
                         "или замок выходит за границы сцены!");
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        facade->addBrick(4, 3, 8, 1);
        facade->addBrick(3, 4, 1, 9);
        facade->addBrick(12, 4, 1, 9);
    }

    for (int i = 0; i < 2; i++)
    {
        facade->addBrick(4, 13, 3, 1);
        facade->addBrick(9, 13, 3, 1);
    }

    facade->addBrick(4, 13, 2, 1);
    facade->addBrick(10, 13, 2, 1);

    for (int i = 0; i < 5; i++)
    {
        facade->addCylinder1(3, 3);
        facade->addCylinder1(3, 13);
        facade->addCylinder1(12, 3);
        facade->addCylinder1(12, 13);

        facade->addCylinder1(6, 5);
        facade->addCylinder1(6, 7);
        facade->addCylinder1(6, 9);
        facade->addCylinder1(9, 5);
        facade->addCylinder1(9, 7);
        facade->addCylinder1(9, 9);
    }
    facade->addTile(6, 5, 4, 5);


    for (int i = 0; i < 8; i += 2)
    {
        facade->addBrick(i + 5, 3, 1, 1);
        facade->addTile(i + 4, 3, 1, 1);
    }
    for (int i = 0; i < 8; i += 2)
    {
        facade->addBrick(3, i + 5, 1, 1);
        facade->addBrick(12, i + 5, 1, 1);
        facade->addTile(3, i + 4, 1, 1);
        facade->addTile(12, i + 4, 1, 1);
    }


    facade->addArc41(6, 13);
    facade->addTile(4, 13, 8, 1);

    facade->addTile(3, 3, 1, 1);
    facade->addTile(3, 13, 1, 1);
    facade->addTile(12, 3, 1, 1);
    facade->addTile(12, 13, 1, 1);

    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}


void MainWindow::on_pushButton_deleteModel_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    ObjectDelete objectDeleteWindow(facade->getScene(), nullptr);
    objectDeleteWindow.setModal(true);
    objectDeleteWindow.exec();

    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}


void MainWindow::on_pushButton_moveModel_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    ObjectChange objectChangeWindow(facade, nullptr);
    objectChangeWindow.setModal(true);
    objectChangeWindow.exec();

    QGraphicsScene *setScene = facade->drawScene(ui->graphicsView->rect());

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    ui->graphicsView->setScene(setScene);
}

void MainWindow::on_pushButton_sceneToInitianPosition_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    if (ui->graphicsView->scene())
        delete ui->graphicsView->scene();
    QGraphicsScene *setScene = facade->toCenter(ui->graphicsView->rect());

    ui->graphicsView->setScene(setScene);
}



void MainWindow::on_pushButton_up_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureRotateXLeft();
}

void MainWindow::on_pushButton_down_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureRotateXRight();
}

void MainWindow::on_pushButton_left_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureRotateYRight();
}

void MainWindow::on_pushButton_right_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureRotateYLeft();
}

void MainWindow::on_pushButton_leftCircle_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureRotateZRight();
}

void MainWindow::on_pushButton_rightCircle_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureRotateZLeft();
}



void MainWindow::on_pushButton_up_scene_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureUp();
}

void MainWindow::on_pushButton_down_scene_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureDown();
}

void MainWindow::on_pushButton_left_scene_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureLeft();
}

void MainWindow::on_pushButton_right_scene_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Необходимо создать сцену!");
        return;
    }

    pictureRight();
}



void MainWindow::on_pushButton_zoom_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Сцена ещё не была задана.");
        return;
    }

    pictureScaleUp();
}

void MainWindow::on_pushButton_distance_clicked()
{
    if (!facade->isSceneSet())
    {
        QErrorMessage *err = new QErrorMessage();
        err->showMessage("Сцена ещё не была задана.");
        return;
    }

    pictureScaleDown();
}




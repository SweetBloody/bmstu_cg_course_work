#ifndef PLACEOBJECTS_H
#define PLACEOBJECTS_H

#include <QDialog>

namespace Ui
{
    class AddBrick;
    class AddPlate;
    class AddTile;
    class AddArc;
    class AddCylinder;
}

class AddBrick : public QDialog
{
    Q_OBJECT

public:
    explicit AddBrick(QWidget *parent = nullptr);
    ~AddBrick();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelWidth();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddBrick *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 0;
    int modelWidth = 0;
};


class AddPlate : public QDialog
{
    Q_OBJECT

public:
    explicit AddPlate(QWidget *parent = nullptr);
    ~AddPlate();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelWidth();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddPlate *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 0;
    int modelWidth = 0;
};


class AddTile : public QDialog
{
    Q_OBJECT

public:
    explicit AddTile(QWidget *parent = nullptr);
    ~AddTile();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelWidth();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddTile *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 0;
    int modelWidth = 0;
};


class AddArc : public QDialog
{
    Q_OBJECT

public:
    explicit AddArc(QWidget *parent = nullptr);
    ~AddArc();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelWidth();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddArc *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 0;
    int modelWidth = 0;
};

class AddCylinder : public QDialog
{
    Q_OBJECT

public:
    explicit AddCylinder(QWidget *parent = nullptr);
    ~AddCylinder();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXCell(int xCell_);
    int  getXCell();

    void setYCell(int yCell_);
    int  getYCell();

    int getModelLength();
    int getModelWidth();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddCylinder *ui;

    int xCell = -1;
    int yCell = -1;

    int modelLength = 0;
    int modelWidth = 0;
};

#endif // PLACEOBJECTS_H

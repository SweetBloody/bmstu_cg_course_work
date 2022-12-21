#ifndef ADDLIGHT_H
#define ADDLIGHT_H

#include <QDialog>

namespace Ui
{
class AddLight;
}

class AddLight : public QDialog
{
    Q_OBJECT

public:
    explicit AddLight(QWidget *parent = nullptr);
    ~AddLight();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    void setXAngle(int degrees);
    int getXAngle();

    void setYAngle(int degrees);
    int getYAngle();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddLight *ui;

    int xAngle = 0;
    int yAngle = 0;
};

#endif

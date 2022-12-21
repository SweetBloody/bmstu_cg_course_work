#ifndef OBJECTMOVE_H
#define OBJECTMOVE_H

#include <QDialog>

namespace Ui
{
class ObjectMove;
}

class ObjectMove : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectMove(QWidget *parent = nullptr);
    ~ObjectMove();

    enum stat
    {
        CANCEL,
        OK
    };

    stat status;

    int getNewXCell();
    int getNewYCell();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ObjectMove *ui;

    int newXCell = -1;
    int newYCell = -1;
};

#endif

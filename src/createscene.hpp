#ifndef CREATESCENE_H
#define CREATESCENE_H

#include <QDialog>

namespace Ui
{
class CreateScene;
}

class CreateScene : public QDialog
{
    Q_OBJECT

public:
    explicit CreateScene(QWidget *parent = nullptr);
    ~CreateScene();

    void setWidth(int width_);
    int getWidth();

    int getheight();
    void setheight(int height_);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateScene *ui;

    int width = -1;
    int height = -1;
};

#endif // CREATESCENE_H

#ifndef FALLINGFIGURES_H
#define FALLINGFIGURES_H

#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QRadioButton>

enum class ShapeType {Rect,Ellips};

#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FallingFigures; }
QT_END_NAMESPACE

class FallingFigures : public QMainWindow
{
    Q_OBJECT
private:
QRadioButton* but_rect;
QRadioButton* but_ellipse;

public:
    FallingFigures(QWidget *parent = nullptr);
    ~FallingFigures();

signals:
void selected_radbutton(ShapeType);

public slots:

void select_radbutton(){
    if (but_rect->isChecked() ==true) { emit selected_radbutton(ShapeType::Rect);}
    else if (but_ellipse->isChecked()==true) { emit selected_radbutton(ShapeType::Ellips);}
    }


private:
    Ui::FallingFigures *ui;



    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);


};
#endif // FALLINGFIGURES_H

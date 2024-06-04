#ifndef FALLINGFIGURES_H
#define FALLINGFIGURES_H

#include <QMainWindow>
#include <QWidget>
#include <QDebug>

enum class ShapeType {Rect,Ellips};

#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FallingFigures; }
QT_END_NAMESPACE

class FallingFigures : public QMainWindow
{
    Q_OBJECT

public:
    FallingFigures(QWidget *parent = nullptr);
    ~FallingFigures();

private:
    Ui::FallingFigures *ui;



    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);
};
#endif // FALLINGFIGURES_H

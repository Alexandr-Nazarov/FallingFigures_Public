#ifndef SCENE_H
#define SCENE_H

#include <QDebug>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsRectItem>
#include <QGraphicsItem>

#include <QGraphicsLineItem>
#include <QEvent>


#include "fallingfigures.h"
#include "mygraphicsitem.h"
#include "movingellipse.h"
#include "movingrect.h"
#include "myevent.h"

#include "plank.h"


class Scene : public QGraphicsScene
{
    Q_OBJECT
private:
QAbstractGraphicsShapeItem* m_current{nullptr};

QAbstractGraphicsShapeItem *selected_item_for_collides{nullptr}; //элемент, который будем проверять на столкновение с другими

QAbstractGraphicsShapeItem* m_frame{nullptr};

QAbstractGraphicsShapeItem* m_point{nullptr};

qreal m_width;
qreal m_height;

bool m_drawingInProcess{false};
ShapeType drawing_figure;
QPointF m_startPoint;
//QPen m_pen;

QTimer *m_timer;

QPointF  select_point;




public:
    explicit Scene(qreal, qreal, QWidget *parent = nullptr);

~Scene(){ delete m_current;
          delete selected_item_for_collides;
          delete m_frame;
          delete m_timer;
        delete m_point;}



signals:

void frame_height_width(qreal* ,qreal*);

public slots:

void refresh();
void upd();

void create_frame_of_scene();

void slot_to_check_collides(QAbstractGraphicsShapeItem*);   //проверка на столкновения при авто-движении

void addp(QPoint& point){m_point->setX(point.x());m_point->setY(point.y()); }

void select_figure(ShapeType type) {drawing_figure=type;}

// QGraphicsScene interface
protected:
virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

// QGraphicsScene interface
protected:
virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

// QGraphicsScene interface
protected:
virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

// QObject interface
public:
bool event(QEvent *event);
};

#endif // SCENE_H

#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QTimer>
#include <QPainter>
#include <QtWidgets>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QObject>
#include <cmath>    //для вычисления физики

#include "scene.h"


struct constants{
 //  const  int   c_hertz=5;   //частота обновления
 const qreal c_g=0.0011;  //ускорение падения
 const  qreal c_e=0.7;     //коэф-т упругости
 const  qreal c_z=0.02;   //погрешность для замирания фигуры (против "дрожания" при остановке)
};

class MovingEllipse : public QObject, public QAbstractGraphicsShapeItem        //Важно! QObject, чтобы  конструкторе сделать connect
{
    Q_OBJECT
private:

qreal m_x;
qreal m_y;
qreal m_width;
qreal m_height;

QTimer *m_timer;

qreal m_v{0};          //скорость
qreal m_v_vert{0};     //вертикальная скорость
qreal m_v_horr{0};     //горизонт-я скорость
qreal angle_conc{0};   //угол касания
qreal angle_after{0};  //угол разлета
qreal m_mass{0};       //масса
bool m_falling{true};  //признак падения
bool m_toright{false}; //признак движения вправо
//bool m_checked_for_collides{false}; //признак для проверки при столкновении только единожды


//виртуальные границы перемещения
qreal m_frame_bottom{0};
qreal m_frame_left{0};
qreal m_frame_height;
qreal m_frame_width;
//----
qreal m_top_phy_point; //верхняя точка при отскоке

qreal m_moving{0};
constants m_constants;

ShapeType m_shape_type;


public:
    MovingEllipse(qreal, qreal,qreal, qreal, qreal, qreal, /*QGraphicsItem*/ QAbstractGraphicsShapeItem *parent=0);

    void SetRect(QRectF);

    void Stop_moving()  {m_v_vert=0; m_v_horr=0; m_moving=0;}               //останавливаем движение
    void Start_moving() {m_v_vert=0; m_v_horr=0; m_moving=1;}               //начальное движение-падение



    void physics (MovingEllipse *figure=0);  //для столкновений с другими или просто движение

    qreal getX() const  {return m_x;}
    qreal getY() const  {return m_y;}

    void set_m_XY(qreal x, qreal y) { m_y=y; m_x=x; }


protected:


  virtual ~MovingEllipse() override { delete m_timer;}            //ВАЖНО! в протектед


  virtual QRectF boundingRect() const override{
        return QRect(m_x,m_y, m_width, m_height);
    }

   virtual QPainterPath shape()const override{                 //переопределяю для точного определяения границ (для проверки на столкновения).
                                                          // используется в collidesWithItem() при углубленной проверки (авт-ки)

        QPainterPath path;
        path.addEllipse(boundingRect());              //если фигура будет элиипсом, то изменить   !!!
     //   path.addRect(boundingRect());
        return path;
    }

  virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{

       painter->setBrush(Qt::green);
       painter->drawEllipse(m_x,m_y,m_width,m_height);

       painter->setBrush(Qt::blue);
      // painter->drawRect(m_x,m_y,m_width,m_height);
       painter->drawEllipse(m_x+m_width/5,m_y+m_height/5,m_width/3,m_height/3);

    }


private slots:               //заменил на private
  void animate();


public slots:



signals:

void position_to_check_collides(QAbstractGraphicsShapeItem*);

};





#endif // MYGRAPHICSITEM_H

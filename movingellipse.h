#ifndef MOVINGELLIPSE_H
#define MOVINGELLIPSE_H


#include "mygraphicsitem.h"

class MovingEllipse: public MovingFigure
{
    Q_OBJECT

private:

public:
    MovingEllipse(qreal, qreal,qreal, qreal);

public:


    virtual QRectF boundingRect() const override{
          return QRect(this->getX(),this->getY(),this->getWidth(),this->getHeight()/*m_x,m_y, m_width, m_height*/);
      }


    virtual QPainterPath shape()const override{                 //переопределяю для точного определяения границ (для проверки на столкновения).
        // используется в collidesWithItem() при углубленной проверки (авт-ки)

        QPainterPath path;
        path.addEllipse(this->boundingRect());              //если фигура будет элиипсом, то изменить   !!!
        //  path.addRect(boundingRect());
        return path;
    }

    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {

         painter->setRenderHint(QPainter::Antialiasing,true); //сглаживание

        painter->setBrush(Qt::green);
         painter->drawEllipse(this->getX(),this->getY(),this->getWidth(),this->getHeight()/*m_x,m_y,m_width,m_height*/);
       //  painter->drawRect(m_x,m_y,m_width,m_height);

         painter->setBrush(Qt::blue);
        // painter->drawRect(m_x,m_y,m_width,m_height);
         painter->drawEllipse(this->getX()+this->getWidth()/5,this->getY()+this->getHeight()/5,this->getWidth()/3,this->getHeight()/3);

        // qDebug()<<this->getX()<<this->getY()<<this->getWidth()<<this->getHeight();
      }


};

#endif // MOVINGELLIPSE_H

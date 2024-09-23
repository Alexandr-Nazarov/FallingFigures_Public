#ifndef PLANK_H
#define PLANK_H

#include "mygraphicsitem.h"

//class Plank: public MovingFigure
//{
//    Q_OBJECT

//private:

//public:
//    Plank(qreal, qreal,qreal, qreal);

//public:

//    virtual void physics (MovingFigure *figure=0) override;


//    virtual QRectF boundingRect() const override{
//          return QRect(this->getX(),this->getY(),this->getWidth(),this->getHeight()/*m_x,m_y, m_width, m_height*/);
//      }


//    virtual QPainterPath shape()const override{                 //переопределяю для точного определяения границ (для проверки на столкновения).
//        // используется в collidesWithItem() при углубленной проверки (авт-ки)

//        QPainterPath path;
//        path.addRect(this->boundingRect());              //если фигура будет элиипсом, то изменить   !!!

//        return path;
//    }



//    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {

//         painter->setRenderHint(QPainter::Antialiasing,true); //сглаживание

//         painter->setBrush(Qt::white);
//         painter->drawRect(this->getX(),this->getY(),this->getWidth(),this->getHeight()/*m_x,m_y,m_width,m_height*/);


//     //    painter->setBrush(Qt::yellow);
//      //   painter->drawRect(this->getX()+this->getWidth()/5,this->getY()+this->getHeight()/5,this->getWidth()/3,this->getHeight()/3);

//      }


//};

#endif // PLANK_H

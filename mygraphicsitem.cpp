#include "mygraphicsitem.h"

MovingEllipse::MovingEllipse(qreal x, qreal y ,qreal width, qreal height, qreal frame_width, qreal frame_height, QAbstractGraphicsShapeItem *parent )
    :  QAbstractGraphicsShapeItem(parent), m_x(x), m_y(y), m_width(width), m_height(height), m_frame_height(frame_height),m_frame_width(frame_width)

{

 m_timer=new QTimer(this);
 QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));
 m_timer->start(5);                //меньше не делать, а то не успевает обновляться

}


void MovingEllipse::SetRect(QRectF rect){

    m_x=rect.x();
    m_y=rect.y();
    m_width=rect.width();
    m_height=rect.height();
    m_mass=m_width*m_height/100;
  //  m_timer->start((10000/(m_width*m_height))<3 ? 3: (10000/(m_width*m_height))>10 ? 10: 10000/(m_width*m_height));         //устанавливаю таймер для скорости движения фигуры от объема
  //  m_timer->start(5);

}




//SLOTS
void MovingEllipse::animate(){

    physics();

  //+ update();
 emit position_to_check_collides(static_cast<QAbstractGraphicsShapeItem*>(this));//посылаем сигнал, чтобы проверить текущее положение на столкновение
}


//Физика


void MovingEllipse::physics(MovingEllipse* other){


  //---вертикальное движение
    //общие законы

    m_y+=(m_v_vert+(m_constants.c_g)/2)*m_moving;                          //расчет s=v0*t+(a*t^2)/2
    m_v_vert+=m_constants.c_g/**m_mass*/;                                  //v=v0+a*t
  //  qDebug()<<m_v_vert;

    if (m_v_vert >=0) {                             //вводим признак падения, чтобы при ударе об нижнюю границу первое условие выполнилось единожды
        m_falling=true;
    }
    else {
        m_falling=false;
    }

    //===
    //--просто движение без сталкивания с другой фигурой
    if (!other){
    if (((m_y+m_height)>=(m_frame_bottom)) && m_falling ) {      //удар об нижнюю границу
        m_v_vert=-m_v_vert*m_constants.c_e;
    }
    }
   // --- удар об верхнюю точку
//    if (((m_y)<=m_frame_height) || ((m_y)<=m_top_phy_point)) {     //удар об потолок или верхнюю возможную точку
//        m_dy*=-1;
//    }

  //==== удар об другой объект
    if (other){

       //--точка пересечения
        QRectF coord_Rect_1=this->mapRectToScene(this->boundingRect());         //получаем координаты ограничивающих прям-к
        QRectF coord_Rect_2=other->mapRectToScene(other->boundingRect());       //получаем координаты ограничивающих прям-к
        QRectF intersection=coord_Rect_2.intersected(coord_Rect_1);            //область пересечения
         qDebug()<<intersection ;
       //---

     //--разъединение объектов
      //  qreal tmp_height=m_height;
     //   m_height-=intersection.height();
      //!  m_y-=m_y+m_height-intersection.y();
      m_y-=m_y+m_height-intersection.y()/*+intersection.height()*/;
    //  m_y-=m_y+intersection.y();
    //    other->m_y-=other->m_y-intersection.y();
     //   m_height-=intersection.height();
     //   m_y-=/*m_y+m_height-intersection.y()-*/m_y+intersection.y();

     //---

     //--скорость разслета
        if (((m_y+m_height)>=intersection.y()) && m_falling && !other->m_falling ) {          //удар об другой объект при падении
                m_v_vert=-m_v_vert*m_constants.c_e;                                           //здесь потом учесть массу
                other->m_v_vert=-other->m_v_vert*other->m_constants.c_e-m_v_vert;

        } else if (((m_y+m_height)>=intersection.y()) && m_falling && other->m_falling ) {
             m_v_vert=-m_v_vert*m_constants.c_e;
             other->m_v_vert=other->m_v_vert*other->m_constants.c_e-m_v_vert;

        }
     //--
 //   m_height=tmp_height;

//       qreal tmp=m_v_vert*m_constants.c_e;
//       m_v_vert=-m_v_vert*m_constants.c_e+other->m_v_vert*other->m_constants.c_e;
//       other->m_v_vert=other->m_v_vert*other->m_constants.c_e+tmp;


    }
  //======

}


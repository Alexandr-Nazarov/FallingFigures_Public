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
   // m_timer->setInterval(m_mass);
}




//SLOTS
void MovingEllipse::animate(){

    physics();

  //+ update();
 emit position_to_check_collides(static_cast<QAbstractGraphicsShapeItem*>(this));//посылаем сигнал, чтобы проверить текущее положение на столкновение
}

QPoint& MovingEllipse::center(){
    m_center.setX(m_x+m_width/2);
    m_center.setY(m_y+m_height/2);
    return m_center;
}


//Физика


void MovingEllipse::physics(MovingEllipse* other){

  m_v=m_v_vert+m_v_horr;
  //---вертикальное движение
    //общие законы

    m_y+=(m_v_vert+(m_constants.c_g)/2)*m_moving;                         //расчет s=v0*t+(a*t^2)/2
    m_v_vert=m_v_vert+m_constants.c_g;                                    //v=v0+a*t


    if (m_v_vert >0) {                             //вводим признак падения, чтобы при ударе об нижнюю границу первое условие выполнилось единожды
        m_falling=true;
    }
    else {
        m_falling=false;
    }
    if (m_y+m_height>m_frame_bottom) m_y=m_frame_bottom-m_height;

    //if (std::abs(m_v_vert)>=m_constants.c_g) m_moving=1; else m_moving=0;

    //===

    //---горизонтальное движение
         //общие законы
          m_x+=m_v_horr*m_moving;
          if (m_v_horr >=0) {                             //вводим признак падения, чтобы при ударе об нижнюю границу первое условие выполнилось единожды
              m_toright=true;
          }
          else {
              m_toright=false;
          }
          if (m_x+m_width>m_frame_width) m_x=m_frame_width-m_width;
          if (m_x<m_frame_left) m_x=m_frame_left;

          //===

          //--просто движение без сталкивания с другим объектом
          if (!other){
          if (((m_x+m_width)>=(m_frame_width)) && m_toright ) {      //удар об правую границу
              m_v_horr=-m_v_horr*m_constants.c_e;
          }
          if ((m_x<=m_frame_left) && !m_toright ) {                  //удар об левую границу
              m_v_horr=-m_v_horr*m_constants.c_e;
          }
          if ((m_y+m_height)>=(m_frame_bottom)){                     //торможение при касании нижней границы
              m_v_horr=m_v_horr*m_constants.c_e;
          }

          }
    //========


    //--просто движение без сталкивания с другим объектом
    if (!other){
    if (((m_y+m_height)>=(m_frame_bottom)) && m_falling ) {      //удар об нижнюю границу
        m_v_vert=-m_v_vert*m_constants.c_e;                     //при отскоке от земли масса не имеет значение, только коэфф.упругости
    }
    }
   // --- удар об верхнюю точку
//    if (((m_y)<=m_frame_height) || ((m_y)<=m_top_phy_point)) {     //удар об потолок или верхнюю возможную точку
//        m_dy*=-1;
//    }

  //==== удар об другой объект
    if (other){

        if ((this->center().y()<=other->center().y())){  //рассматриваем только для верхнего объекта относительно 3 случаев столкновения (вн-вв, вн-вн, вв-вв)

       //--точка пересечения
        QRectF coord_Rect_1=this->mapRectToScene(this->boundingRect());         //получаем координаты ограничивающих прям-к
        QRectF coord_Rect_2=other->mapRectToScene(other->boundingRect());       //получаем координаты ограничивающих прям-к
        QRectF intersection=coord_Rect_2.intersected(coord_Rect_1);            //область пересечения
       //---


        //---

       //вспомогательный расчет
        qreal a=m_v+(other->m_mass*other->m_v)/m_mass;
        qreal b=((m_mass*std::pow(m_v,2)+other->m_mass*std::pow(other->m_v,2))/2);
        qreal c=b-(m_mass*std::pow(a,2))/2;
        qreal d=other->m_mass*(other->m_mass+m_mass)/(2*m_mass);

        //расчет угла столкновения
        qreal b_ocr=std::sqrt(std::pow(intersection.x()-center().x(),2)+std::pow(intersection.y()-center().y(),2));

         if (this->center().x()<=other->center().x()){
           angle_conc=M_PI/2+std::asin((intersection.x()-center().x())/ b_ocr)/**180/M_PI*/;      //угол касания //теорема синусов// в радианах все
        //    angle_conc=std::asin((center().x()-intersection.x())/ b_ocr);
           qDebug()<< "left"<< angle_conc;

         } else {
           angle_conc=std::asin((intersection.x()-center().x())/ b_ocr)/**180/M_PI*/;
          //  angle_conc=M_PI/2+std::asin((center().x()-intersection.x())/ b_ocr);
         //     angle_conc=M_PI/2-std::asin((intersection.x()-center().x())/ b_ocr);
           qDebug()<< "right"<< angle_conc;
         }
       //  qDebug()<< angle_conc;

        //---
        //угол разлета
        other->angle_after=std::atan(-(m_mass*std::sin(angle_conc))/(other->m_mass-m_mass*std::cos(angle_conc)))/**180/M_PI*/;
        angle_after=angle_conc-other->angle_after;
       //


    // --скорость разслета
            if (m_falling && !other->m_falling){    //удар об другой объект при падении


                  //расчет упругого отскока

                   other->m_v=(other->m_mass*a+std::sqrt(std::pow(other->m_mass*a,2)+4*d*c))/(2*d);
                   m_v=a-(other->m_mass/m_mass)*other->m_v;

                   m_v_vert=m_v*std::cos(angle_after);
                   other->m_v_vert=other->m_v*std::cos(other->angle_after);

                   m_v_horr=m_v*std::sin(angle_after);
                   other->m_v_horr=-other->m_v*std::sin(other->angle_after);
                  //--

              //  qDebug()<<"1v";

                 //разъединение объектов
                 m_y-=m_y+m_height-intersection.y()-intersection.height()+0.5;
                  if (this->center().x()<other->center().x()) {
                       m_x-=m_x+m_width-intersection.x()-intersection.width()+0.5;
                  } else {
                      m_x+=other->m_x+other->m_width-intersection.x()-intersection.width()+0.5;
                  }

        } else if (m_falling && other->m_falling){


                  //расчет упругого отскока

                  other->m_v=(other->m_mass*a+std::sqrt(std::pow(other->m_mass*a,2)+4*d*c))/(2*d);
                  m_v=a-(other->m_mass/m_mass)*other->m_v;

                  m_v_vert=m_v*std::cos(angle_after);
                  other->m_v_vert=other->m_v*std::cos(other->angle_after);


                  m_v_horr=m_v*std::sin(angle_after);
                  other->m_v_horr=-other->m_v*std::sin(other->angle_after);
                  //--

                  //qDebug()<<"2v";


                  //разъединение объектов
                  m_y-=m_y+m_height-intersection.y()-intersection.height()+0.5;
                  if (this->center().x()<other->center().x()) {
                    m_x-=m_x+m_width-intersection.x()-intersection.width()+0.5;
                   } else {
                     m_x+=other->m_x+other->m_width-intersection.x()-intersection.width()+0.5;
                   }
        }

        else if (!m_falling && !other->m_falling){


                 //расчет упругого отскока

                 other->m_v=(other->m_mass*a+std::sqrt(std::pow(other->m_mass*a,2)+4*d*c))/(2*d);
                 m_v=a-(other->m_mass/m_mass)*other->m_v;

                 m_v_vert=m_v*std::cos(angle_after);
                 other->m_v_vert=other->m_v*std::cos(other->angle_after);


                 m_v_horr=m_v*std::sin(angle_after);
                 other->m_v_horr=-other->m_v*std::sin(other->angle_after);
                 //--

                //qDebug()<<"3v";

                 //разъединение объектов
                  m_y-=m_y+m_height-intersection.y()-intersection.height()+0.5;
                  if (this->center().x()<other->center().x()) {
                      m_x-=m_x+m_width-intersection.x()-intersection.width()+0.5;
                  } else {
                      m_x+=other->m_x+other->m_width-intersection.x()-intersection.width()+0.5;
                  }
        }
     //--


    }
    }
  //======


}


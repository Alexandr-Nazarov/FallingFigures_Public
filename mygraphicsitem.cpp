#include "mygraphicsitem.h"


MovingEllipse::MovingEllipse(qreal x, qreal y ,qreal width, qreal height,/* qreal frame_width, qreal frame_height, */QAbstractGraphicsShapeItem *parent )
    :  QAbstractGraphicsShapeItem(parent), m_x(x), m_y(y), m_width(width), m_height(height)/*, m_frame_height(frame_height),m_frame_width(frame_width)*/

{
 m_timer=new QTimer(this);
 QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));
 m_timer->start(5);                //меньше не делать, а то не успевает обновляться

}

void MovingEllipse::set_m_frame_height_width(qreal* h,qreal *w){

    m_frame_height=*h;
    m_frame_width=*w;
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

  //  this->setTransformOriginPoint(this->boundingRect().center());     //устанавливаем центр вращения
   //  this->setTransformOriginPoint(this->center().x(),this->center().y());     //устанавливаем центр вращения
   // current_transform=this->transform();
}




//SLOTS
void MovingEllipse::animate(){

    MyEvent* eve=new MyEvent(200);
    QApplication::postEvent(this, eve);

  //  physics();
  //  emit position_to_check_collides(static_cast<QAbstractGraphicsShapeItem*>(this));//посылаем сигнал, чтобы проверить текущее положение на столкновение
}

QPoint& MovingEllipse::center(){
    m_center.setX(m_x+m_width/2);
    m_center.setY(m_y+m_height/2);
    return m_center;
}

bool MovingEllipse::event(QEvent *event){
    if (event->type()==static_cast<QEvent::Type>(UpdateEv)) {
        physics();
        emit position_to_check_collides(static_cast<QAbstractGraphicsShapeItem*>(this));//посылаем сигнал, чтобы проверить текущее положение на столкновение
        return true;
    }
    //return MovingEllipse::event(event);
}


//Физика

void MovingEllipse::physics(MovingEllipse* other){

  m_v=m_v_vert+m_v_horr;
 // qDebug()<<m_v;

  //угол вращения
//  this->setTransformOriginPoint(this->center().x(),this->center().y());     //устанавливаем центр вращения
//  current_transform=this->transform();

// //  m_angle_rotate*=2;

//  rotation.rotate(m_angle_rotate);
////  this->setRotation(m_angle_rotate);
//  QTransform new_transform=current_transform*rotation;
//    this->setTransform(new_transform);

  //---

  //---вертикальное движение
    //общие законы

    m_y-=(m_v_vert+(m_constants.c_g)/2)*m_moving;   //00                      //расчет s=v0*t+(a*t^2)/2
    m_v_vert=m_v_vert+m_constants.c_g;                                    //v=v0+a*t

    if (m_v_vert >0) {                             //вводим признак падения, чтобы при ударе об нижнюю границу первое условие выполнилось единожды
        m_falling=true;
    }
    else {
        m_falling=false;
    }
    if (m_y/*+m_height*/<m_frame_bottom) m_y=m_frame_bottom/*+m_height*/;           ///0000

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
          if ((m_y)<=(m_frame_bottom) && m_falling){                     //торможение при касании нижней границы
              m_v_horr=m_v_horr*m_constants.c_e;
          }
          if ((m_y+m_height)>=(m_frame_height) && !m_falling){                     //торможение при касании нижней границы
              m_v_horr=m_v_horr*m_constants.c_e;
          }

          }
    //========


    //--просто движение без сталкивания с другим объектом
    if (!other){
    if (((m_y/*+m_height*/)<=(m_frame_bottom)) && m_falling ) {      //удар об нижнюю границу
        m_v_vert=-m_v_vert*m_constants.c_e;                     //при отскоке от земли масса не имеет значение, только коэфф.упругости
    }

   // --- удар об верхнюю точку
//    else if (m_y<=m_frame_height  && !m_falling ) {     //удар об потолок или верхнюю возможную точку
//         m_v_vert=-m_v_vert*m_constants.c_e;
//    }
   }
  //==== удар об другой объект
    if (other){
   if ((m_falling && !other->m_falling) || (m_falling && other->m_falling) || (!m_falling && !other->m_falling)) {

        if ((this->center().y()>=other->center().y())){  //рассматриваем только для верхнего объекта относительно 3 случаев столкновения (вн-вв, вн-вн, вв-вв)

       //--точка пересечения
        QRectF coord_Rect_1=this->mapRectToScene(this->boundingRect());         //получаем координаты ограничивающих прям-к
        QRectF coord_Rect_2=other->mapRectToScene(other->boundingRect());       //получаем координаты ограничивающих прям-к
        QRectF intersection=coord_Rect_2.intersected(coord_Rect_1);            //область пересечения
       //---

        qreal angle_conc{0};
        //---

        //разъединение объектов
        m_y+=(m_y/*+m_height*/-intersection.y()/*+0.5*/);
         if (this->center().x()<other->center().x()) {
             m_x-=m_x+m_width-intersection.x()-intersection.width()+0.5;
         } else {
             m_x+=other->m_x+other->m_width-intersection.x()-intersection.width()+0.5;

         }


         //----

       //вспомогательный расчет
        qreal a=m_v+(other->m_mass*other->m_v)/m_mass;
        qreal b=((m_mass*std::pow(m_v,2)+other->m_mass*std::pow(other->m_v,2))/2);
        qreal c=b-(m_mass*std::pow(a,2))/2;
        qreal d=other->m_mass*(other->m_mass+m_mass)/(2*m_mass);

        //расчет угла столкновения (подумать над вариациями (1)-норм и (2)-норм для улучшения)
        qreal b_ocr=std::sqrt(std::pow(intersection.center().x()-center().x(),2)+std::pow(intersection.center().y()-center().y(),2));
        if (std::abs(intersection.center().y()-center().y())<=b_ocr && b_ocr!=0){
        if (this->center().x()<=other->center().x()){
//             angle_conc=(std::asin((-center().y()+intersection.center().y())/ b_ocr)-M_PI/2); // (1)  //!! //угол касания //теорема синусов// в радианах все

        } else {
          //добавить width    qreal b_ocr=std::sqrt(std::pow(intersection.x()-center().x(),2)+std::pow(intersection.y()+intersection.height()-center().y(),2));
          //   angle_conc=/*M_PI/2-*/std::asin((intersection.y()-center().y())/ b_ocr);/*+M_PI/2; *///!!
           //    angle_conc=/*M_PI/2-*/std::asin((-intersection.y()+center().y())/ b_ocr);      // (2)
            //   angle_conc=M_PI-(std::asin((-intersection.y()+center().y())/ b_ocr)-M_PI/2);
           angle_conc=(-std::asin((center().y()-intersection.center().y())/b_ocr)+3*M_PI/2);             // (2)  //!!
         }
        } else { angle_conc=0;}

        //---
        //угол разлета
        other->angle_after=std::atan(-(m_mass*std::sin(angle_conc))/(other->m_mass-m_mass*std::cos(angle_conc)))/**180/M_PI*/;
        angle_after=(angle_conc-other->angle_after);
       //

            //расчет упругого столкновения
            other->m_v=(other->m_mass*a+std::sqrt(std::pow(other->m_mass*a,2)+4*d*c))/(2*d);
            m_v=a-(other->m_mass/m_mass)*other->m_v;

            m_v_vert=m_v*std::cos(angle_after);
            other->m_v_vert=other->m_v*std::cos(other->angle_after);


            m_v_horr=m_v*std::sin(angle_after);
            if (this->center().x()<=other->center().x()){
              other->m_v_horr=other->m_v*std::sin(other->angle_after);
             }
            else {
                other->m_v_horr=-other->m_v*std::sin(other->angle_after);
            }
            //--
                //поворот
                //  this->setTransformOriginPoint(center().x(),center().y());     //устанавливаем центр вращения
                //  this->setRotation(m_angle_rotate);
             //   m_angle_rotate+=1;;
                //---

            //разъединение объектов

//            m_y+=(m_y/*+m_height*/-intersection.y()/*+0.5*/);
//             if (this->center().x()<other->center().x()) {
//                 m_x-=m_x+m_width-intersection.x()-intersection.width()/*+0.5*/;
//             } else {
//                 m_x+=other->m_x+other->m_width-intersection.x()-intersection.width()/*+0.5*/;
//             }


           //
    }
    }
    }
  //======

}


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

void MovingEllipse::rotate(qreal v1,qreal v2,qreal& rasst){


    //угол вращения
  this->setTransformOriginPoint(this->center().x(),this->center().y());     //устанавливаем центр вращения
  //  current_transform=this->transform();
  //  m_angle_rotate=0.0001;
  //  m_rotation.translate(this->boundingRect().center().x(),this->boundingRect().center().y() /*this->center().x(), this->center().y()*/);       //сдвиг, чтобы совпадал с центрои
  //  m_rotation.rotate(m_angle_rotate);
  //  m_rotation.translate(-this->boundingRect().center().x(),-this->boundingRect().center().y()/*-this->center().x(), -this->center().y()*/);    //возвращаю обратно
  //    QTransform new_transform=current_transform*m_rotation;
  //    this->setTransform(new_transform);

     qreal J=m_mass*(v2-v1);  //импульс силы J=m(v2-v1)
     qreal L=J*rasst;         //момент силы
     qreal I=m_mass*(std::pow(m_width/2,2)+std::pow(m_height/2,2))/4;            //момент инерции I=(1/4)*m(a^2+b^2)

     if (L&&I){                                             //если столкновение
   //!  m_vspom_angle+=(m_vspom_angle+L/I);
   //!  m_vspom_angle=std::fmod(m_vspom_angle,360.0);


     if(m_angle_rotate>=0) {                                                        // от 0 до 360!!!  переделать с учетом этого
       // m_vspom_angle+=(m_vspom_angle-L/I);
         m_vspom_angle+=(m_vspom_angle+L/I);
          qDebug()<<"m";
     } else if (m_angle_rotate<0){                                                  //может быть <0 по часовой
         //m_vspom_angle-=(m_vspom_angle+L/I);
         m_vspom_angle-=(m_vspom_angle-L/I);
         qDebug()<<"h";
     }

    m_vspom_angle=std::fmod(m_vspom_angle,360.0);
   }




     m_angle_rotate+=m_vspom_angle;
     m_angle_rotate=std::fmod(m_angle_rotate,360.0);
    // m_angle_rotate=std::abs(m_angle_rotate);
    // m_angle_rotate+=1;


  if(m_angle_rotate){
  //   QPoint oldPoint(m_x+m_width/2,0);
   // QPoint oldPoint(m_x/*+m_width/2*/,/*-std::abs*/std::sqrt(std::pow((m_height/2)*std::sin(qDegreesToRadians( m_angle_rotate)),2)+std::pow((m_width/2)*std::cos(qDegreesToRadians( m_angle_rotate)),2)));
   //QPoint oldPoint(m_x,(m_height/2*std::sin(qDegreesToRadians( m_angle_rotate))-(m_width/2*std::cos(qDegreesToRadians( m_angle_rotate))))/2);
  // if (m_height>=m_width){
//   QPoint oldPoint(m_x,(std::sqrt(std::pow(m_height/2,2)+std::pow(m_width/2,2)))*std::sin(qDegreesToRadians( m_angle_rotate))/2);

   qreal p_y=0;
   qreal b=0;
   if (m_height>=m_width){

//   if (std::abs(m_angle_rotate)>=0 && std::abs(m_angle_rotate)<=90 )
//   p_y=(m_height/2*std::sin(qDegreesToRadians( m_angle_rotate))+(m_width/2*std::sin(qDegreesToRadians( m_angle_rotate))))/2;
//   else if (std::abs(m_angle_rotate)>90 && std::abs(m_angle_rotate)<=180 )
//   p_y=(m_height/2*std::sin(qDegreesToRadians( m_angle_rotate))-(m_width/2*std::sin(qDegreesToRadians( m_angle_rotate))))/2;
//   }

  // qreal b=std::abs(std::sin(qDegreesToRadians(m_angle_rotate))*(m_y-m_x/*m_height/2*/));
  // p_y=-std::abs(m_height/2-b);


     //  if (std::abs(static_cast<int>(m_angle_rotate))%180<90){
       if (std::abs(m_angle_rotate)<90 || std::abs(m_angle_rotate)>=270 ){
     //  qreal b=m_height/2*(1-std::sin(qDegreesToRadians(90-std::abs(m_angle_rotate))))-m_width/2*(1-std::sin(qDegreesToRadians(90-std::abs(m_angle_rotate)))) ;
       b=(m_height-m_width)/2*(1-std::sin(qDegreesToRadians(90-std::abs(m_angle_rotate)))) ;
       p_y=-std::abs(b);}

      //if (std::abs(static_cast<int>(m_angle_rotate))%180>=90){
        else /*if (std::abs(m_angle_rotate)>=90 && std::abs(m_angle_rotate)<270 )*/{
   //    qreal b=m_height/2*(1+std::sin(qDegreesToRadians(90-std::abs(m_angle_rotate))))-m_width/2*(1+std::sin(qDegreesToRadians(90-std::abs(m_angle_rotate)))) ;
        b=(m_height-m_width)/2*(1+std::sin(qDegreesToRadians(90-std::abs(m_angle_rotate)))) ;
       p_y=-std::abs(b);
       }

    //  qreal tmp_x=m_x;
    //  QPoint op(m_x,m_y);
//       QTransform transform;
//       transform.translate(0,m_y*std::sin(qDegreesToRadians( m_angle_rotate)));
//       transform.rotate(m_angle_rotate);
//       this->setTransform(transform);
//      // QPoint np=transform.map(op);


}


  // }

   QPoint oldPoint(m_x+m_width/2,p_y);
   m_frame_bottom=oldPoint.y();
   QPoint newPoint=oldPoint;
   emit addpoint(newPoint);
  // }

   //!!!   QPoint oldPoint(m_x,m_height/2*std::sin(qDegreesToRadians( m_angle_rotate)));
   //!!!   m_frame_bottom=oldPoint.y()/2;

   this->setRotation(m_angle_rotate);

 //   QPoint newPoint=oldPoint;

 //   emit addpoint(newPoint);

//   QTransform transform;
//    transform.translate(center().x(),center().y());
//       transform.rotate(m_angle_rotate);
//       oldPoint = transform.map(oldPoint);
//       m_frame_bottom=oldPoint.y();
 //      this->setTransform(transform);


    //  QPoint oldPoint(m_x,m_frame_bottom);

    // QTransform old_transform=this->transform();
  //+   QTransform new_transform=this->transform().rotate(-m_angle_rotate);

  // this->setTransform(new_transform);
  // m_rotation.translate(this->boundingRect().center().x(),this->boundingRect().center().y() /*this->center().x(), this->center().y()*/);
 //  this->setTransformOriginPoint(this->transform().dx(),this->transform().dy());
  //       QTransform new_transform=this->transform();

  //  new_transform.translate(m_x,0);


    // this->setTransform(new_transform);
  //  this->setTransform(new_transform);
   //  this->resetTransform();

    // qDebug()<<this->transform().rotate(m_angle_rotate)  ;
 //    QTransform new_transform=this->transform().rotate(m_angle_rotate);
     //qDebug()<<new_transform;
   //   QTransform new_transform=this->transform();
    //  QPoint oldPoint(m_x,m_y);
  //    QPointF oldPoint = this->mapToParent(QPointF(m_x, m_y));


     //  qDebug()<<newPoint<<oldPoint;

     // qDebug()<<m_angle_rotate<<oldPoint<<newPoint <<"a" ;
    //  m_delta_y=std::abs(oldPoint.y()-newPoint.y())-m_width;

    //  if (m_falling){
    //    QPoint newPoint=old_transform.map(oldPoint);
    // QTransform new_transform2=new_transform*this->transform();

//        QPoint newPoint=oldPoint;//new_transform.map(oldPoint);
//      //   QPointF newPoint=this->transform().rotate(m_angle_rotate).map(oldPoint);
//        emit addpoint(newPoint);

    //    m_delta_y=std::abs(-oldPoint.y()+std::abs(newPoint.y()));
    //    m_frame_bottom=-oldPoint.y();
    //    m_frame_bottom=newPoint.y()-toBott;
      //  m_delta_y=oldPoint.y()-newPoint.y();
        //m_delta_y=std::abs(newPoint.y());
     //   m_frame_bottom=-m_delta_y+m_height;
     //   m_frame_bottom=-std::abs(newPoint.y())+m_delta_y;
       // m_frame_bottom=new_transform.map(m_frame_bottom);
     //   QPoint nullp(0,0);
      //  m_frame_bottom=new_transform.map(QPoint(m_x,0)).y()+m_delta_y;
   //   }
     // else {
     //     m_frame_bottom=0;
     // }

   /// m_frame_bottom=-m_width;
      // m_delta_y=newPoint.y();
   //   qDebug()<<m_frame_bottom<<"p";//newPoint<<m_delta_y;
  //  qDebug()/*<<oldPoint*/<<m_frame_bottom<<newPoint<<"p";

      //  qDebug()<<m_delta_y;
  //    if (m_angle_rotate>-180){
     //     m_frame_bottom=-m_delta_y;
 //      } else {
      // m_angle_rotate=0;
      // m_vspom_angle=0;
           //m_frame_bottom=m_delta_y;
   //    }
  //  this->setTransform(new_transform*this->transform());
    // qDebug()<<m_y<<m_delta_y<<m_height;
  //   m_delta_y+=newPoint.y()-m_y;
 //    qDebug()<<m_delta_y;
   //  m_x=newPoint.x();
   //  m_y=newPoint.y();
    //---
   //  qDebug()<<   m_angle_rotate;
    rasst=0;
  }
}



void MovingEllipse::physics(MovingEllipse* other){

//  if (std::abs(m_v_vert)>m_constants.c_g) m_moving=1; else m_moving=0;

  if  (m_moving){
  qreal v1=m_v;
  m_v=m_v_vert+m_v_horr;
  rotate(v1,m_v,m_rasst);
 // m_frame_bottom=0;
  }

  //if (m_v>2)
  //qDebug()<<m_v;



  //---вертикальное движение
    //общие законы

    m_y-=(m_v_vert+(m_constants.c_g)/2)*m_moving;                         //расчет s=v0*t+(a*t^2)/2
    m_v_vert=m_v_vert+m_constants.c_g;                                    //v=v0+a*t

    if (m_v_vert >0) {                             //вводим признак падения, чтобы при ударе об нижнюю границу первое условие выполнилось единожды
        m_falling=true;
    }
    else {
        m_falling=false;
    }
    if (m_y<m_frame_bottom && m_falling) {m_y=m_frame_bottom; }

   // if (std::abs(m_v_vert)>=m_constants.c_g) m_moving=1; else m_moving=0;

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
    if (((m_y)<=(m_frame_bottom)) && m_falling ) {      //удар об нижнюю границу
        qreal v1=m_v_vert;
        m_v_vert=-m_v_vert*m_constants.c_e;                     //при отскоке от земли масса не имеет значение, только коэфф.упругости

        m_rasst=center().y();                                                                                       //ЗДЕСЬ ПЕРЕДЕЛАТЬ!!!!
      //  m_rasst=std::sqrt(std::pow(intersection.center().x()-center().x(),2)+std::pow(center().y(),2));
        rotate(v1,m_v_vert,m_rasst);
   //     m_frame_bottom=0;
     //   qDebug()<<v1*m_constants.c_e<<m_v_vert;
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
        m_y+=(m_y-intersection.y()-0.49);                                    //если поставить 0.5 то будут проваливаться друг-в-друга. иожно и без значения
         if (this->center().x()<other->center().x()) {
//             qDebug()<<other->m_x-intersection.x()-0.5;
//              m_x+=other->m_x-intersection.x()-0.5;
             m_x+=-0.5;
         } else if (this->center().x()>=other->center().x()){
           //  qDebug()<<other->m_x+other->m_width-intersection.x()-intersection.width()+0.5;
           //  m_x+=other->m_x+other->m_width-intersection.x()-intersection.width()+0.5;
               m_x+=0.5;
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
         //    angle_conc=-(std::asin((-center().y()+intersection.center().y())/b_ocr)+M_PI/2);

         if (this->center().x()<other->center().x()){
        angle_conc=(std::asin((-center().y()+intersection.center().y())/b_ocr)+M_PI/2);               //(1) //!!
        } else  if (this->center().x()>other->center().x()) {
        angle_conc=-(std::asin((-center().y()+intersection.center().y())/b_ocr)+M_PI/2);             // (2)  //!!super
         }
        } else { angle_conc=0;}

        //---
        //угол разлета
        other->angle_after=std::atan(-(m_mass*std::sin(angle_conc))/(other->m_mass-m_mass*std::cos(angle_conc)));
        angle_after=(angle_conc-other->angle_after);
       //

            //расчет упругого столкновения
            other->m_v=(other->m_mass*a+std::sqrt(std::pow(other->m_mass*a,2)+4*d*c))/(2*d);
            m_v=a-(other->m_mass/m_mass)*other->m_v;

            m_v_vert=m_v*std::cos(angle_after);
            other->m_v_vert=other->m_v*std::cos(other->angle_after);


            m_v_horr=m_v*std::sin(angle_after);
//            if (this->center().x()<other->center().x()){
//              other->m_v_horr=other->m_v*std::sin(other->angle_after);
//             }
//            else if (this->center().x()>other->center().x()) {
//                other->m_v_horr=-other->m_v*std::sin(other->angle_after);
//            }
            other->m_v_horr=-other->m_v*std::sin(other->angle_after);
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


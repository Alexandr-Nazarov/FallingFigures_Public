#include "scene.h"

Scene::Scene(qreal width, qreal height, QWidget *parent) : QGraphicsScene(parent), m_width(width), m_height(-height)        // -height для удобства работы с физикой
{

    QGraphicsScene::setSceneRect(0,0,m_width,m_height);

    //границы сцены
  //     create_frame_of_scene();
//        //т.к. фигуры двигаются с определенной частотой по таймеру, заданном в их конструкторе, то чтобы не было мелькания или пропадания фигур
//        // сделал частоту обновления сцены:
    m_timer=new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(5);

    //===


}

void Scene::create_frame_of_scene(){
    //границы сцены
    m_frame=new QGraphicsRectItem(1,1,m_width-3,m_height-3);
    m_frame->setZValue(-1);     //устанавливаем на задний фон
    m_frame->setBrush(Qt::transparent);
  //  m_frame->setActive(false);


    addItem(m_frame);

    //===

}

void Scene::refresh(){
   // m_timer->stop();
  //  killTimer(m_timer->timerId()-1);
    this->clear();
  //  m_timer=new QTimer(this);
  //  m_timer->start(5);
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   // if(!m_frame) create_frame_of_scene();

    if (event->buttons()==Qt::RightButton) {
           m_drawingInProcess=true;

           m_startPoint.setX(event->scenePos().x());
           m_startPoint.setY(event->scenePos().y());

      //+     if (drawing_figure==ShapeType::Rect){
            m_current=new /*QGraphicsRectItem*/MovingEllipse (m_startPoint.x(),m_startPoint.y(),0.,0.,m_width,m_height);
          QObject::connect( static_cast<MovingEllipse*>(m_current), SIGNAL(position_to_check_collides(QAbstractGraphicsShapeItem*)),
                            this, SLOT(slot_to_check_collides(QAbstractGraphicsShapeItem*)));

       //+     }

           //добавить потом классы-фигуры для разнообразия
       //     if (drawing_figure==ShapeType::Ellips)
      //     {
      //       m_current=new /*QGraphicsEllipseItem*/MyGraphicsItem (m_startPoint.x(),m_startPoint.y(),0.,0.);}

         //  if (m_current){
       //+!    m_current->setFlags(QGraphicsItem::ItemIsMovable) ;                  !!! не использую, т.к. происходит двойное координирование и проблемы с m_x,m_y
           addItem(m_current);
        //   }

       }


     if (event->buttons()==Qt::LeftButton) {
         m_drawingInProcess=false;

         //выбираем элемент для дальнейшей проверки на столкновения

           selected_item_for_collides =qgraphicsitem_cast<QAbstractGraphicsShapeItem* >( itemAt(event->scenePos(), QTransform()));
           if (selected_item_for_collides!=m_frame && selected_item_for_collides){
           static_cast<MovingEllipse*>(selected_item_for_collides)->Stop_moving();

                 //для корректировки координат вирт-рамки при перетаскивании
             //  qDebug()<<static_cast<MovingEllipse*>(selected_item_for_collides)->m_y;
               m_startPoint.setX(static_cast<MovingEllipse*>(selected_item_for_collides)->getX()-event->scenePos().x());
               m_startPoint.setY(static_cast<MovingEllipse*>(selected_item_for_collides)->getY()-event->scenePos().y());
                 //---
           }
         //===
     }

        QGraphicsScene::mousePressEvent(event);

}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_drawingInProcess) {
         QPointF curPoint;
         curPoint.setX(event->scenePos().x());
         curPoint.setY(event->scenePos().y());

         if ((curPoint-m_startPoint).manhattanLength()>QApplication::startDragDistance()) {     //проверка, что случайно не двинули мышкой

         QRectF tmp_Rect(m_startPoint, curPoint);

     //    if (drawing_figure==ShapeType::Rect){
        // qgraphicsitem_cast<QGraphicsRectItem*/*MyGraphicsItem*/>(m_current)->setRect(tmp_Rect.normalized());
          static_cast<MovingEllipse*>(m_current)->SetRect(tmp_Rect.normalized());

       //+      this->update();
      //   }

     //переделать потом !    if (drawing_figure==ShapeType::Ellips){
     //    qgraphicsitem_cast<QGraphicsEllipseItem*>(m_current)->setRect(tmp_Rect.normalized());}
        }
    }



    //проверка на столкновение при перемещении мышкой
         if (!m_drawingInProcess && selected_item_for_collides){
      //    qDebug()<<static_cast<MovingEllipse*>(selected_item_for_collides)->m_y;
        slot_to_check_collides(selected_item_for_collides);

        static_cast<MovingEllipse*>(selected_item_for_collides)->set_m_XY(
                event->scenePos().x()+m_startPoint.x(),
                event->scenePos().y()+m_startPoint.y());
        update();
         }
    //====

  QGraphicsScene::mouseMoveEvent(event);

}


void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //начальное движение-падение
    if (m_drawingInProcess){
      m_drawingInProcess=false;
      selected_item_for_collides=nullptr;                       //для того, чтобы не запускалось движение
    static_cast<MovingEllipse*>(m_current)-> Start_moving();

    }
    //===

    //запуск движения перетаскиваемого объекта
    if (!m_drawingInProcess && selected_item_for_collides && selected_item_for_collides!=m_frame){
    //    qDebug()<<static_cast<MovingEllipse*>(selected_item_for_collides)->m_y;
        //корректировка координат вирт-рамки при перетаскивании
  //+!      static_cast<MovingEllipse*>(selected_item_for_collides)->set_m_XY(
  //+!              event->scenePos().x()-m_startPoint.x(), event->scenePos().y()-m_startPoint.y() );
  //+!    update();
        //---

     static_cast<MovingEllipse*>(selected_item_for_collides)->Start_moving();
   // selected_item_for_collides->scenePos().x()

    }
    //===

    m_drawingInProcess=false;
    m_current=nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::slot_to_check_collides(QAbstractGraphicsShapeItem* item)   //проверка на столкновения при авто-движении
{
      if (/*!m_drawingInProcess &&*/ item && item!=m_frame){
       for (auto x : this->items()){
           if((x!=item) && (x!=m_frame) ) {                            //еще раз подумать и разобраться с рамкой(исчезновением фигур)          

                    //тут
               if (item->collidesWithItem(x,Qt::IntersectsItemShape)/* || x->collidesWithItem(item,Qt::IntersectsItemShape)*/) { /* && !static_cast<MovingEllipse*>(x)->isInside(static_cast<MovingEllipse*>(item))*/    //подумать, если заезжает фигура

//нужное    //       coord_Rect_1.intersects(coord_Rect_2)*/ /*(!intersection.isEmpty() && (intersection.width()<=1|| intersection.height()<=1 ))
                   /*QRectF coord_Rect_1=item->mapRectToScene(item->boundingRect());    //получаем координаты ограничивающих прям-к
                   QRectF coord_Rect_2=item->mapRectToScene(x->boundingRect());       //получаем координаты ограничивающих прям-к
                   QRectF intersection=coord_Rect_2.intersected(coord_Rect_1); */       //область пересечения


                 //  qDebug()<<intersection ;                                                            //ДАЛЕЕ ПРОПИСАТЬ ФИЗИКУ
                    if (x && item /*&& dynamic_cast<MovingEllipse*>(item) && dynamic_cast<MovingEllipse*>(x)*/) { // включить потом проверку
                    static_cast<MovingEllipse*>(item)->physics(static_cast<MovingEllipse*>(x));
                    //      static_cast<MovingEllipse*>(item)->Change_dy();
                    //      static_cast<MovingEllipse*>(item)->Change_dx();
                    }


             }
           }
       }
      }
}


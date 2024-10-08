#include "scene.h"

Scene::Scene(qreal width, qreal height, QWidget *parent) : QGraphicsScene(parent), m_width(width), m_height(/*-*/height)        // -height для удобства работы с физикой
{

    QGraphicsScene::setSceneRect(0,0,m_width,m_height);

    //границы сцены

    m_timer=new QTimer(this);
    m_timer->start(5);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(upd())); //т.к. из-за таймера проблемы с потоками при отображении рамки и обектов, то отправляю обработки в событие
    create_frame_of_scene();

    //===

  //  plank=new Plank(50,50,100,100);
  //  addItem(plank);
}

void Scene::create_frame_of_scene(){
    //границы сцены
    m_frame=new QGraphicsRectItem(1,1,m_width-3,m_height-3);
    m_frame->setZValue(-1);     //устанавливаем на задний фон
    m_frame->setBrush(Qt::transparent);
  //  m_frame->setActive(false);
 //   m_frame->setSelected(false);
     m_frame->setFlag(QGraphicsItem::ItemIsSelectable,false);

    addItem(m_frame);

    //===

}


void Scene::upd(){
      MyEvent* eve=new MyEvent(200);
      QApplication::postEvent(this, eve);
   // this->update();
}

void Scene::refresh(){

     this->clear();
     create_frame_of_scene();
}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   // if(!m_frame) create_frame_of_scene();

    if (event->buttons()==Qt::RightButton) {
           m_drawingInProcess=true;

           m_startPoint.setX(event->scenePos().x());
           m_startPoint.setY(event->scenePos().y());

           if (drawing_figure==ShapeType::Ellips)
            m_current=new /*QGraphicsRectItem*/MovingEllipse(m_startPoint.x(),m_startPoint.y(),0.,0.);
           if (drawing_figure==ShapeType::Rect)
            m_current=new /*QGraphicsRectItem*/MovingRect(m_startPoint.x(),m_startPoint.y(),0.,0.);


          QObject::connect(  this, SIGNAL(frame_height_width(qreal* ,qreal*)),
                             static_cast<MovingFigure*>(m_current), SLOT(set_m_frame_height_width(qreal*,qreal*)));
          emit frame_height_width(&m_height,&m_width);

          QObject::connect( static_cast<MovingFigure*>(m_current), SIGNAL(position_to_check_collides(QAbstractGraphicsShapeItem*)),
                            this, SLOT(slot_to_check_collides(QAbstractGraphicsShapeItem*)));

            m_point=new QGraphicsEllipseItem(0,0,2,2);
           QObject::connect(static_cast<MovingFigure*>(m_current), SIGNAL(addpoint(QPoint&)),this,SLOT(addp(QPoint&)));
        //!   addItem(m_point);
        //   m_current->setSelected(true);
          //  m_current->setZValue(1);
            m_current->setFlag(QGraphicsItem::ItemIsSelectable,true);
       //+     }

           //добавить потом классы-фигуры для разнообразия
       //     if (drawing_figure==ShapeType::Ellips)
      //     {
      //       m_current=new /*QGraphicsEllipseItem*/MyGraphicsItem (m_startPoint.x(),m_startPoint.y(),0.,0.);}

         //  if (m_current){
       //+    m_current->setFlags(QGraphicsItem::ItemIsMovable) ;                  !!! не использую, т.к. происходит двойное координирование и проблемы с m_x,m_y
           addItem(m_current);
        //   }

       }


     if (event->buttons()==Qt::LeftButton) {
//         MyEvent* eve=new MyEvent(201);
//         QApplication::postEvent(this, eve);

         m_drawingInProcess=false;

         //выбираем элемент для дальнейшей проверки на столкновения


//        QList<QGraphicsItem* >items=this->items(event->scenePos());

//         for (auto item:items) {
//             qDebug()<<item;}
//         if (item->ItemIsSelectable){
//             selected_item_for_collides =qgraphicsitem_cast<QAbstractGraphicsShapeItem* >(item);
//            // qDebug()<<selected_item_for_collides;
//         }
//         }

     //    do {
    //     selected_item_for_collides =qgraphicsitem_cast<QAbstractGraphicsShapeItem* >( itemAt(event->scenePos(), QTransform()));
    //    qDebug()<<selected_item_for_collides;
    //     if (selected_item_for_collides!=m_frame) break; /*qDebug()<<"frame";*/
    //     }
    //     while(selected_item_for_collides!=m_frame);

         select_point=event->scenePos();

         MyEvent* eve=new MyEvent(201);
         QApplication::sendEvent(this, eve);

           selected_item_for_collides =qgraphicsitem_cast<QAbstractGraphicsShapeItem* >( itemAt(event->scenePos(), QTransform()));
           if (selected_item_for_collides!=m_frame && selected_item_for_collides){
           static_cast<MovingFigure*>(selected_item_for_collides)->Stop_moving();

                 //для корректировки координат при перетаскивании
               m_startPoint.setX(static_cast<MovingFigure*>(selected_item_for_collides)->getX()-event->scenePos().x());
               m_startPoint.setY(static_cast<MovingFigure*>(selected_item_for_collides)->getY()-event->scenePos().y());
                 //---
           }
         //===
      //   qDebug()<<selected_item_for_collides;
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
          static_cast<MovingFigure*>(m_current)->SetRect(tmp_Rect.normalized());

       //+      this->update();
      //   }

     //переделать потом !    if (drawing_figure==ShapeType::Ellips){
     //    qgraphicsitem_cast<QGraphicsEllipseItem*>(m_current)->setRect(tmp_Rect.normalized());}
        }
    }



    //проверка на столкновение при перемещении мышкой
         if (!m_drawingInProcess && selected_item_for_collides && selected_item_for_collides!=m_frame ){
      //    qDebug()<<static_cast<MovingFigure*>(selected_item_for_collides)->m_y;
        slot_to_check_collides(selected_item_for_collides);

        static_cast<MovingFigure*>(selected_item_for_collides)->set_m_XY(
                event->scenePos().x()+m_startPoint.x(),
                event->scenePos().y()+m_startPoint.y());
       //!!!! update();
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
    static_cast<MovingFigure*>(m_current)-> Start_moving();

    }
    //===

    //запуск движения перетаскиваемого объекта
    if (!m_drawingInProcess && selected_item_for_collides && selected_item_for_collides!=m_frame){
    //    qDebug()<<static_cast<MovingFigure*>(selected_item_for_collides)->m_y;
        //корректировка координат вирт-рамки при перетаскивании
  //+!      static_cast<MovingFigure*>(selected_item_for_collides)->set_m_XY(
  //+!              event->scenePos().x()-m_startPoint.x(), event->scenePos().y()-m_startPoint.y() );
  //+!    update();
        //---

     static_cast<MovingFigure*>(selected_item_for_collides)->Start_moving();
   // selected_item_for_collides->scenePos().x()

    }
    //===

    m_drawingInProcess=false;
    m_current=nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::slot_to_check_collides(QAbstractGraphicsShapeItem* item)   //проверка на столкновения при авто-движении
{
      if (/*!m_drawingInProcess &&*/ item && item!=m_frame ){
       for (auto x : this->items()){
           if((x!=item) && (x!=m_frame) && (x!=m_point)) {                            //еще раз подумать и разобраться с рамкой(исчезновением фигур)

                    //тут
               if (item->collidesWithItem(x,Qt::IntersectsItemShape)/* || x->collidesWithItem(item,Qt::IntersectsItemShape)*/) { /* && !static_cast<MovingFigure*>(x)->isInside(static_cast<MovingFigure*>(item))*/    //подумать, если заезжает фигура

//нужное    //       coord_Rect_1.intersects(coord_Rect_2)*/ /*(!intersection.isEmpty() && (intersection.width()<=1|| intersection.height()<=1 ))
                   /*QRectF coord_Rect_1=item->mapRectToScene(item->boundingRect());    //получаем координаты ограничивающих прям-к
                   QRectF coord_Rect_2=item->mapRectToScene(x->boundingRect());       //получаем координаты ограничивающих прям-к
                   QRectF intersection=coord_Rect_2.intersected(coord_Rect_1); */       //область пересечения


                 //  qDebug()<<intersection ;                                                            //ДАЛЕЕ ПРОПИСАТЬ ФИЗИКУ
                    if (x && item /*&& dynamic_cast<MovingFigure*>(item) && dynamic_cast<MovingFigure*>(x)*/) { // включить потом проверку
                    static_cast<MovingFigure*>(item)->physics(static_cast<MovingFigure*>(x));
                    //      static_cast<MovingFigure*>(item)->Change_dy();
                    //      static_cast<MovingFigure*>(item)->Change_dx();
                    }


             }
           }
       }
      }
}


bool Scene::event(QEvent *event){
    if (event->type()==static_cast<QEvent::Type>(UpdateEv)) {
     //   delete m_frame;
   //     create_frame_of_scene();

        this->update();
   //     delete m_frame;

        return true;
    }
    if (event->type()==static_cast<QEvent::Type>(SelectEv)) {

        QList<QGraphicsItem* >items=this->items(select_point);

                 for (auto item:items) {
                     qDebug()<<item;
                 }

        return true;
    }


    return QGraphicsScene::event(event);
}

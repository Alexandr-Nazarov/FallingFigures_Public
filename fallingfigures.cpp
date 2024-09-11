#include "fallingfigures.h"
#include "ui_fallingfigures.h"

FallingFigures::FallingFigures(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FallingFigures)
{
    ui->setupUi(this);
    Scene* scene=new Scene(300,400);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing,true); //сглаживание
    //scene->create_frame_of_scene();
    ui->graphicsView->scale(1,-1);

   // ui->radioButton_Circle->pressed();
   // ui->radioButton_Circle->setChecked(true);

    QObject::connect(ui->clearButton , SIGNAL(clicked()), scene, SLOT(refresh()));

   //----выбор фигуры

    but_rect=new QRadioButton("Rect");
    but_ellipse=new QRadioButton("Circle");
    QButtonGroup *but_group=new QButtonGroup(this);
    but_group->addButton(but_rect);
    but_group->addButton(but_ellipse);
     but_ellipse->setChecked(true);
     ui->verticalLayout->addWidget(but_rect);
     ui->verticalLayout->addWidget(but_ellipse);
     ui->verticalLayout->addWidget(ui->clearButton);

    QObject::connect(but_rect, SIGNAL (clicked()), this, SLOT (select_radbutton()));
    QObject::connect(but_ellipse, SIGNAL (clicked()), this, SLOT (select_radbutton()));

    QObject::connect(this, SIGNAL(selected_radbutton(ShapeType)), scene , SLOT(select_figure(ShapeType)));
  //----



}

FallingFigures::~FallingFigures()
{
    delete ui;
}



void FallingFigures::paintEvent(QPaintEvent *event)
{

    //ui->verticalLayout->setGeometry(tmp_rect);

   ui->graphicsView->setGeometry(5,15,this->width()-100, this->height()-35);

   ui->verticalLayoutWidget->setGeometry(ui->graphicsView->width()+15,15, 80, 100);



}

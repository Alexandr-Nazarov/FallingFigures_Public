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


    ui->radioButton_Rect->pressed();
    ui->radioButton_Rect->setChecked(true);

    QObject::connect(ui->clearButton , SIGNAL(clicked()), scene, SLOT(clear()));

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

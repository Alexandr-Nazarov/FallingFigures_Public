#include <QApplication>


#include "fallingfigures.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FallingFigures w;
    w.show();
    return a.exec();
}



//#include <cstdlib> // Для функции rand()
//#include <ctime> // Для функции time()

//// Функция для инициализации генератора случайных чисел
//void initRandom() {
//    srand(static_cast<unsigned int>(time(nullptr)));
//}

//// Функция для получения случайного цвета
//QColor getRandomColor() {
//    initRandom(); // Инициализируем генератор случайных чисел
//    switch (rand() % 4) {
//        case 0: return Qt::red;
//        case 1: return Qt::yellow;
//        case 2: return Qt::blue;
//        case 3: return Qt::green;
//        default: return Qt::black; // На всякий случай, если rand() вернет неожиданное значение
//    }
//}

#include <QApplication>
#include "ui/mainwindow.h"
#include <QScreen>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
//  auto scr = QApplication::screens()[0];
//  qDebug() << scr->size() << '\n' << scr->physicalSize() << '\n' << scr->availableSize() << '\n'
//  << "virutal size:" << scr->virtualSize();
  MainWindow mw;
  mw.show();
  return QApplication::exec();
}
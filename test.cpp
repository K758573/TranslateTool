#include <QApplication>
#include <QMessageBox>
#include "ui/mainwindow.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  MainWindow mw;
  mw.show();
  return QApplication::exec();
}
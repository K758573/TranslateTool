//
// Created by Shiro on 2023/9/12.
//

#ifndef TRANSLATETOOL_MAINWINDOW_H
#define TRANSLATETOOL_MAINWINDOW_H

#include <QWidget>
#include "../src/ClipboardTool.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow :
  public QWidget
{
Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  
  ~MainWindow() override;

protected:
  virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
  
  void focusOutEvent(QFocusEvent *event) override;

private:
  Ui::MainWindow *ui;
  ClipboardTool ct_;
};


#endif //TRANSLATETOOL_MAINWINDOW_H

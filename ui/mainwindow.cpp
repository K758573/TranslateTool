//
// Created by Shiro on 2023/9/12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "../src/TranslateTool.h"
#include "../src/BasicLog.h"
#include <QFocusEvent>

MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setWindowFlag(Qt::WindowStaysOnTopHint, true);
  setAttribute(Qt::WA_DeleteOnClose, false);
}

MainWindow::~MainWindow()
{
  delete ui;
}


bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
  if (((MSG *) message)->message != WM_HOTKEY) {
    return false;
  }
  if (ClipboardTool::isHotkeyTriggered((MSG *) message)) {
    klog("hot key triggered");
    hide();
    const std::string &select = ct_.getSelect();
    const std::string &translated = TranslateTool::translate(select);
    ui->edit_seleted->setText(select.data());
    ui->edit_translated->setText(translated.data());
    show();
  }
  return false;
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
  if (event->lostFocus()) {
    hide();
  }
}


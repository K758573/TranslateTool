//
// Created by Shiro on 2023/9/12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "../src/TranslateTool.h"
#include "../src/BasicLog.h"
#include <QFocusEvent>
#include "TranslateResult.h"


MainWindow::MainWindow(QWidget *parent) :
  QWidget(parent), ui(new Ui::MainWindow), ct_((void *) winId())
{
  ui->setupUi(this);
  setWindowFlag(Qt::WindowStaysOnTopHint, true);
  setAttribute(Qt::WA_DeleteOnClose, false);
  connect(qApp, &QGuiApplication::applicationStateChanged, this, [this](Qt::ApplicationState state) {
    if (state == Qt::ApplicationInactive) {
      hide();
    }
  });
  connect(ui->btn_translate, &QPushButton::clicked, this, &MainWindow::translateCurrentText);
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
    hide();
    klog("key triggered");
    const std::string &select = ct_.getSelect();
    ui->edit_seleted->setText(select.data());
    translateCurrentText();
    show();
    activateWindow();
    raise();
  }
  return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->ignore();
  hide();
}

void MainWindow::showEvent(QShowEvent *event)
{
  const QPoint &point_current = QCursor::pos();
  const QRect &geo_old = geometry();
  
  int geo_x = point_current.x();
  int geo_y = point_current.y();
  int geo_w = geo_old.width();
  int geo_h = geo_old.height();
  geo_x = geo_x - geo_w / 2;
  if (geo_x < 0) {
    geo_x = 0;
  } else if (geo_x + geo_w > 1280) {
    geo_x = 1280 - geo_w;
  }
  geo_y = geo_y + 24;
  if (geo_y + geo_h > 720) {
    geo_y = 720 - geo_h;
  }
  
  setGeometry(geo_x, geo_y, geo_w, geo_h);
  QWidget::showEvent(event);
}

void MainWindow::translateCurrentText() {
    const std::string &translated = TranslateTool::translate(ui->edit_seleted->toPlainText().toStdString());
    auto tr = TranslateResult::fromJson(QByteArray::fromStdString(translated));
    ui->edit_translated->setText(tr.text());
}


//
// Created by Shiro on 2023/10/28.
//

#include "TranslateResult.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

TranslateResult TranslateResult::fromJson(const QByteArray &json) {
  auto document = QJsonDocument::fromJson(json);
  TranslateResult tr;
  tr.text_ = document.array().first().toObject().value("translations").toArray().first().toObject().value("text").toString();
  return tr;
}

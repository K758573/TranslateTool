#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

int main(int argc, char **argv) {
  QString jsonstr =
      R"([{"detectedLanguage":{"language":"en","score":1.0},"translations":[{"text":"选择","to":"zh-Hans"}]}])";
  QJsonDocument json_result = QJsonDocument::fromJson(jsonstr.toLocal8Bit());

  qDebug() << json_result.array().first().toObject().value("translations").toArray().first().toObject().value("text").toString();

  return 0;
}
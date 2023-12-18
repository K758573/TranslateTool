//
// Created by Shiro on 2023/10/28.
//

#ifndef TRANSLATETOOL_UI_TRANSLATERESULT_H_
#define TRANSLATETOOL_UI_TRANSLATERESULT_H_
#include <QString>

//[{"detectedLanguage":{"language":"en","score":1.0},"translations":[{"text":"选择","to":"zh-Hans"}]}]
class TranslateResult {
 private:
  QString text_;

 public:
  QString text(){
    return text_;
  }
  static TranslateResult fromJson(const QByteArray &json);
};

#endif //TRANSLATETOOL_UI_TRANSLATERESULT_H_

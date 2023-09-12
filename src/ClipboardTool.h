//
// Created by Shiro on 2023/9/5.
//

#ifndef TRANSLATETOOL_CLIPBOARDTOOL_H
#define TRANSLATETOOL_CLIPBOARDTOOL_H

#include <string>
typedef struct tagMSG MSG;

class ClipboardTool
{
private:
  //上次读取时的格式
  int format_{};
  //申请的全局内存的位置
  void* data_{};
  //当前读取到的内容，保存的是原始数据
  std::string buffer_;
  //global key id
  int id_;
public:
  ClipboardTool();
  
  virtual ~ClipboardTool();
  
  static bool isHotkeyTriggered(MSG *msg);
  
public:
  std::string getSelect();

private:
  /**
   * 读取，读完把读到的数据保存到buffer，读取的格式保存到format
   * 读取之后会清空剪贴板
   * @return 转换成U8的字符串或者原始数据
   */
  std::string read();
  
  /**
   * 申请内存，把buffer写入全局内存中，用data保存
   */
  void backup();
  //恢复剪贴板
  /**
   * 根据上次读到的格式，全局内存，把全局内存写入剪贴板
   */
  void restore();
  
  /**
   * 模拟 Ctrl C ，等待剪贴板内容变化后返回
   * @return 复制成功返回true，复制失败返回false
   */
  static bool waitCopy();
};


#endif //TRANSLATETOOL_CLIPBOARDTOOL_H

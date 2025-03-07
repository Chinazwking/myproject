// exception.h - 异常
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020  8 12
// License: GPL.v3

#ifndef __EASY_ENGINE_INCLUDE_COMMON_EXCEPTION_H__
#define __EASY_ENGINE_INCLUDE_COMMON_EXCEPTION_H__

#include <exception>
#include <string>

namespace easy_engine {

class Exception : public std::exception {
 public:
  explicit Exception(const std::string& msg = "") noexcept
      : _error_message(msg) {}
  virtual ~Exception() noexcept {}
  const char* what() const noexcept override { return _error_message.c_str(); }

 private:
  std::string _error_message;
};

// 带有位置的异常抛出
#define THROW_WITH_LOC(exc_class, msg)                     \
  {                                                        \
    std::string line(std::to_string(__LINE__));            \
    std::string file(__FILE__);                            \
    throw exc_class("[" + file + "][" + line + "]" + msg); \
  }

}  // namespace easy_engine

#endif  // __EASY_ENGINE_INCLUDE_COMMON_EXCEPTION_H__

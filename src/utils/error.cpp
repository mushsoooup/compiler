//
// Created by zc175 on 2023/9/29.
//
#include "utils/error.h"
#include <cstdarg>
#include <iostream>

// Access illegal address to trigger SIGSEGV for better debugging
void critical_error(Token &token, const char *fmt...) {
  char buf[64];
  va_list list;
  va_start(list, fmt);
  vsnprintf(buf, 64, fmt, list);
  va_end(list);
  buf[63] = '\0';
  std::cout << "Error at line " << token.line << " column " << token.column
            << "; " << buf << std::endl;
  volatile int a = *(int *)0x1;
}

void critical_error(const char *fmt...) {
  char buf[64];
  va_list list;
  va_start(list, fmt);
  vsnprintf(buf, 64, fmt, list);
  va_end(list);
  buf[63] = '\0';
  std::cout << buf << std::endl;
  volatile int a = *(int *)0x1;
}

#ifndef ASSERT_H
#define ASSERT_H
#include <stdarg.h>
#include <ExportHeader.h>
#include <Logger.h>

#pragma warning ( disable : 4127)

#define ASSERT(expr, ...)  do { const char* msg = #expr##" "##__VA_ARGS__; if (!(expr)) {LOG(Severe, msg ); END_LOG exit(1);}}while(0);

//#pragma warning ( default : 4127)
#endif

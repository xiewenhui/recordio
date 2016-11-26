#ifndef COMMONLIB_STRINGPRINTF_H_
#define COMMONLIB_STRINGPRINTF_H_

#include <string>

#include "recordio/stringpiece.h"

namespace commonlib {
namespace util {
std::string StringPrintf(const char* const format, ...);
void SStringPrintf(std::string* const dst, const char* const format, ...);
void StringAppendF(std::string* const dst, const char* const format, ...);
}  // namespace util
}  // namespace commonlib
#endif  // COMMONLIB_STRINGPRINTF_H_

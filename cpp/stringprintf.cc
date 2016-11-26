#include "recordio/stringprintf.h"

#include <cstdarg>
#include <cstdio>

#include <string>

#if !defined(_MSC_VER)
#ifndef va_copy
# ifdef __va_copy
# define va_copy(d, s) __va_copy((d), (s))
# else
# define va_copy(d, s) memcpy(&(d), &(s), sizeof(va_list))
# endif
#endif
#endif

namespace commonlib {
namespace util {

void StringAppendV(std::string* const dst, const char* const format, va_list ap) {
  // First try with a small fixed size buffer
  char space[1024];

  // It's possible for methods that use a va_list to invalidate
  // the data in it upon use.  The fix is to make a copy
  // of the structure before using it and use that copy instead.
  va_list backup_ap;
#if defined(_MSC_VER)
  backup_ap = ap;
#else
  va_copy(backup_ap, ap);
#endif
  int result = vsnprintf(space, sizeof(space), format, backup_ap);
  va_end(backup_ap);

  if ((result >= 0) && (result < sizeof(space))) {
    // It fit
    dst->append(space, result);
    return;
  }

  // Repeatedly increase buffer size until it fits
  int length = sizeof(space);
  while (true) {
    if (result < 0) {
      // Older behavior: just try doubling the buffer size
      length *= 2;
    } else {
      // We need exactly "result+1" characters
      length = result + 1;
    }
    char* const buf = new char[length];

// Restore the va_list before we use it again
#if defined(_MSC_VER)
    backup_ap = ap;
#else
    va_copy(backup_ap, ap);
#endif
    result = vsnprintf(buf, length, format, backup_ap);
    va_end(backup_ap);

    if ((result >= 0) && (result < length)) {
      // It fit
      dst->append(buf, result);
      delete[] buf;
      return;
    }
    delete[] buf;
  }
}

std::string StringPrintf(const char* const format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string result;
  StringAppendV(&result, format, ap);
  va_end(ap);
  return result;
}

void SStringPrintf(std::string* const dst, const char* const format, ...) {
  va_list ap;
  va_start(ap, format);
  dst->clear();
  StringAppendV(dst, format, ap);
  va_end(ap);
}

void StringAppendF(std::string* const dst, const char* const format, ...) {
  va_list ap;
  va_start(ap, format);
  StringAppendV(dst, format, ap);
  va_end(ap);
}

}  // namespace util
}  // namespace commonlib

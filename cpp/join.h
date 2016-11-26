#ifndef COMMONLIB_JOIN_H_
#define COMMONLIB_JOIN_H_

#include <string>
#include <sstream>

#include "google/protobuf/stubs/common.h"
#include "recordio/stringpiece.h"

namespace commonlib {
namespace util {

// A buffer size large enough for all FastToBuffer functions.
const int kFastToBufferSize = 32;

// Writes output to the beginning of the given buffer. Returns a pointer to the
// end of the std::string (i.e. to the NUL char). Buffer must be at least 12 bytes.
// Not actually fast, but maybe someday!
template <class T>
char* NumToBuffer(T i, char* buffer) {
  std::stringstream ss;
  ss << i;
  const std::string s = ss.str();
  strcpy(buffer, s.c_str());  // NOLINT
  return buffer + s.size();
}

struct AlphaNum {
  StringPiece piece;
  char digits[kFastToBufferSize];

  // No bool ctor -- bools convert to an integral type.
  // A bool ctor would also convert incoming pointers (bletch).

  AlphaNum(::google::protobuf::int32 i32)  // NOLINT(runtime/explicit)
      : piece(digits, NumToBuffer(i32, digits) - &digits[0]) {}
  AlphaNum(::google::protobuf::uint32 u32)  // NOLINT(runtime/explicit)
      : piece(digits, NumToBuffer(u32, digits) - &digits[0]) {}
  AlphaNum(::google::protobuf::int64 i64)  // NOLINT(runtime/explicit)
      : piece(digits, NumToBuffer(i64, digits) - &digits[0]) {}
  AlphaNum(::google::protobuf::uint64 u64)  // NOLINT(runtime/explicit)
      : piece(digits, NumToBuffer(u64, digits) - &digits[0]) {}
  AlphaNum(float f)  // NOLINT(runtime/explicit)
      : piece(digits, strlen(NumToBuffer(f, digits))) {}
  AlphaNum(double f)  // NOLINT(runtime/explicit)
      : piece(digits, strlen(NumToBuffer(f, digits))) {}
  AlphaNum(const char* c_str) : piece(c_str) {}   // NOLINT(runtime/explicit)
  AlphaNum(const StringPiece& pc) : piece(pc) {}  // NOLINT(runtime/explicit)
  AlphaNum(const std::string& s) : piece(s) {}         // NOLINT(runtime/explicit)

  StringPiece::size_type size() const { return piece.size(); }
  const char* data() const { return piece.data(); }

 private:
  // Use ":" not ':'
  AlphaNum(char c);  // NOLINT(runtime/explicit)
};

extern AlphaNum gEmptyAlphaNum;

std::string StrCat(const AlphaNum& a);
std::string StrCat(const AlphaNum& a, const AlphaNum& b);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d, const AlphaNum& e);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d, const AlphaNum& e, const AlphaNum& f);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d, const AlphaNum& e, const AlphaNum& f,
              const AlphaNum& g);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d, const AlphaNum& e, const AlphaNum& f,
              const AlphaNum& g, const AlphaNum& h);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d, const AlphaNum& e, const AlphaNum& f,
              const AlphaNum& g, const AlphaNum& h, const AlphaNum& i);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d, const AlphaNum& e, const AlphaNum& f,
              const AlphaNum& g, const AlphaNum& h, const AlphaNum& i,
              const AlphaNum& j);
std::string StrCat(const AlphaNum& a, const AlphaNum& b, const AlphaNum& c,
              const AlphaNum& d, const AlphaNum& e, const AlphaNum& f,
              const AlphaNum& g, const AlphaNum& h, const AlphaNum& i,
              const AlphaNum& j, const AlphaNum& k);

void StrAppend(std::string* s, const AlphaNum& a);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d, const AlphaNum& e);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d, const AlphaNum& e,
               const AlphaNum& f);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d, const AlphaNum& e,
               const AlphaNum& f, const AlphaNum& g);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d, const AlphaNum& e,
               const AlphaNum& f, const AlphaNum& g, const AlphaNum& h);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d, const AlphaNum& e,
               const AlphaNum& f, const AlphaNum& g, const AlphaNum& h,
               const AlphaNum& i);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d, const AlphaNum& e,
               const AlphaNum& f, const AlphaNum& g, const AlphaNum& h,
               const AlphaNum& i, const AlphaNum& j);
void StrAppend(std::string* s, const AlphaNum& a, const AlphaNum& b,
               const AlphaNum& c, const AlphaNum& d, const AlphaNum& e,
               const AlphaNum& f, const AlphaNum& g, const AlphaNum& h,
               const AlphaNum& i, const AlphaNum& j, const AlphaNum& k);

}  // namespace util
}  // namespace commonlib

#endif  // COMMONLIB_JOIN_H_

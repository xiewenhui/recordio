#include "recordio/recordio.h"

#include <memory>
#include <string>
#include "glog/logging.h"


namespace commonlib {
namespace util {
const ::google::protobuf::int32  RecordWriter::kMagicNumber = 0x3ed7230a;

RecordWriter::RecordWriter(File* const file)
    : file_(file), use_compression_(true) {}

bool RecordWriter::Close() { return file_->Close(); }

void RecordWriter::set_use_compression(bool use_compression) {
  use_compression_ = use_compression;
}

RecordReader::RecordReader(File* const file) : file_(file) {}

bool RecordReader::Close() { return file_->Close(); }

}  // namespace util
}  // namespace commonlib

#ifndef COMMONLIB_RECORDIO_H_
#define COMMONLIB_RECORDIO_H_

#include <memory>
#include <string>
#include <boost/scoped_array.hpp>
#include "snappy.h"
#include "recordio/file.h"

namespace commonlib {
namespace util {
// This class appends a protocol buffer to a file in a binary format.
// The data written in the file follows the following format (sequentially):
// - MagicNumber (32 bits) to recognize this format.
// - Uncompressed data payload size (32 bits)
// - Compressed data payload size (32 bits), or 0 if the
//   data is not compressed.
// - Payload, possibly compressed. use snappy
class RecordWriter {
 public:
  // Magic number when writing and reading protocol buffers.
  static const ::google::protobuf::int32 kMagicNumber;

  explicit RecordWriter(File* const file);

  template <class P>
  bool WriteProtocolMessage(const P& proto) {
    std::string uncompressed_buffer;
    proto.SerializeToString(&uncompressed_buffer);
    const ::google::protobuf::uint32 uncompressed_size = uncompressed_buffer.size();

    std::string compressed_buffer("");
    if (use_compression_) {
      compressed_buffer.reserve(uncompressed_size + 128);
      snappy::Compress(uncompressed_buffer.c_str(), uncompressed_size, &compressed_buffer);
    }
    const ::google::protobuf::uint32 compressed_size = compressed_buffer.size();

    if (file_->Write(&kMagicNumber, sizeof(kMagicNumber)) !=
        sizeof(kMagicNumber)) {
      return false;
    }
    if (file_->Write(&uncompressed_size, sizeof(uncompressed_size)) !=
        sizeof(uncompressed_size)) {
      return false;
    }
    if (file_->Write(&compressed_size, sizeof(compressed_size)) !=
        sizeof(compressed_size)) {
      return false;
    }
    if (use_compression_) {
      if (file_->Write(compressed_buffer.c_str(), compressed_size) !=
          compressed_size) {
        return false;
      }
    } else {
      if (file_->Write(uncompressed_buffer.c_str(), uncompressed_size) !=
          uncompressed_size) {
        return false;
      }
    }
    return true;
  }
  // Closes the underlying file.
  bool Close();

  void set_use_compression(bool use_compression);

 private:
  File* const file_;
  bool use_compression_;
};

// This class reads a protocol buffer from a file.
// The format must be the one described in RecordWriter, above.
class RecordReader {
 public:
  explicit RecordReader(File* const file);

  template <class P>
  bool ReadProtocolMessage(P* const proto) {
    ::google::protobuf::uint32 usize = 0;
    ::google::protobuf::uint32 csize = 0;
    int magic_number = 0;
    if (file_->Read(&magic_number, sizeof(magic_number)) !=
        sizeof(magic_number)) {
      return false;
    }
    if (magic_number != RecordWriter::kMagicNumber) {
      return false;
    }
    if (file_->Read(&usize, sizeof(usize)) != sizeof(usize)) {
      return false;
    }
    if (file_->Read(&csize, sizeof(csize)) != sizeof(csize)) {
      return false;
    }

    if (csize != 0) {  // The data is compressed.
      boost::scoped_array<char> compressed_buffer(new char[csize + 1]);
      if (file_->Read(compressed_buffer.get(), csize) != csize) {
        return false;
      }
      compressed_buffer[csize] = '\0';

      std::string buffer;
      buffer.reserve(usize + 1);
      if (!snappy::Uncompress(compressed_buffer.get(), csize, &buffer)) {
        return false;
      }
      return proto->ParseFromString(buffer);
    } else {
      boost::scoped_array<char> buffer(new char[usize + 1]);
      if (file_->Read(buffer.get(), usize) != usize) {
        return false;
      }
      return proto->ParseFromArray(buffer.get(), usize);
    }
  }

  // Closes the underlying file.
  bool Close();

 private:
  File* const file_;
};

}  // namespace util
}  // namespace commonlib

#endif  // COMMONLIB_RECORDIO_H_

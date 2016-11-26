package recordio

import (
	"bytes"
	"encoding/binary"
	"errors"
	"github.com/golang/protobuf/proto"
	"github.com/golang/snappy"
	"os"
)

// Magic number when writing and reading protocol buffers.
const kMagicNumber uint32 = 0x3ed7230a
const kUInt32Len = 4

// This class appends a protocol buffer to a file in a binary format.
// The data written in the file follows the following format (sequentially):
// - MagicNumber (32 bits) to recognize this format.
// - Uncompressed data payload size (32 bits)
// - Compressed data payload size (32 bits), or 0 if the
//   data is not compressed.
// - Payload, possibly compressed. use snapp
type RecordWriter struct {
	file     *os.File
	compress bool
}

// constructor
func NewRecordWriter(filepath string) (*RecordWriter, error) {
	rw := new(RecordWriter)
	f, err := os.OpenFile(filepath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0755)
	if err != nil {
		return nil, err
	}
	rw.file = f
	rw.compress = true
	return rw, nil
}

func (rw *RecordWriter) WriteProtocolMessage(pb proto.Message) (bool, error) {
	bytes, err := proto.Marshal(pb)
	if err != nil {
		return false, err
	}
	uncompressedSize := uint32(len(bytes))
	compressedBuffer := make([]byte, 0, uncompressedSize)
	if rw.compress {
		compressedBuffer = snappy.Encode(compressedBuffer, bytes)
	}
	compressedSize := uint32(len(compressedBuffer))

	var file *os.File = rw.file
	if _, err := file.Write(UInt2Bytes(kMagicNumber)); err != nil {
		return false, err
	}
	if _, err := file.Write(UInt2Bytes(uncompressedSize)); err != nil {
		return false, err
	}
	if _, err := file.Write(UInt2Bytes(compressedSize)); err != nil {
		return false, err
	}

	if rw.compress {
		if _, err := file.Write(compressedBuffer); err != nil {
			return false, err
		}
	} else {
		if _, err := file.Write(bytes); err != nil {
			return false, err
		}
	}
	return true, nil
}

func (rw *RecordWriter) Finish() {
	rw.file.Sync()
	rw.file.Close()
}

// This class reads a protocol buffer from a file.
// The format must be the one described in RecordWriter, above
type RecordReader struct {
	file *os.File
}

// constructor
func NewRecordReader(filepath string) (*RecordReader, error) {
	rr := new(RecordReader)
	f, err := os.Open(filepath)
	if err != nil {
		return nil, err
	}
	rr.file = f
	return rr, nil
}

func (rr *RecordReader) ReadProtocolMessage(pb proto.Message) (bool, error) {
	var file *os.File = rr.file
	uint32buf := make([]byte, kUInt32Len)
	if _, err := file.Read(uint32buf); err != nil {
		return false, err
	}

	if kMagicNumber != Bytes2Uint32(uint32buf) {
		return false, errors.New("err: MagicNumber dismatch")
	}

	if _, err := file.Read(uint32buf); err != nil {
		return false, err
	}
	usize := Bytes2Uint32(uint32buf)

	if _, err := file.Read(uint32buf); err != nil {
		return false, err
	}
	csize := Bytes2Uint32(uint32buf)

	if csize != 0 {
		compressedBuffer := make([]byte, csize)
		if _, err := file.Read(compressedBuffer); err != nil {
			return false, err
		}
		data, err := snappy.Decode(nil, compressedBuffer)
		if err != nil {
			return false, err
		}
		err = proto.Unmarshal(data, pb)
		if err != nil {
			return false, err
		}
		return true, nil
	} else {
		unCompressedBuffer := make([]byte, usize)
		if _, err := file.Read(unCompressedBuffer); err != nil {
			return false, err
		}
		err := proto.Unmarshal(unCompressedBuffer, pb)
		if err != nil {
			return false, err
		}
		return true, nil
	}
}

func (rr *RecordReader) Finish() {
	rr.file.Close()
}

func Bytes2Uint32(data []byte) (ret uint32) {
	buf := bytes.NewBuffer(data)
	binary.Read(buf, binary.LittleEndian, &ret)
	return
}

func UInt2Bytes(d uint32) []byte {
	buf := make([]byte, kUInt32Len)
	binary.LittleEndian.PutUint32(buf, d)
	return buf

}

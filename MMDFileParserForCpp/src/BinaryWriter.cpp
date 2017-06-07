#include <MMDFileParser/ParserHelper.hpp>

namespace MMDFileParser
{

  BinaryWriter::BinaryWriter(const FilePath& path) : stream_(std::make_shared<std::ofstream>(path, std::ios::binary)) {}

  inline bool BinaryWriter::write(const void* buf, int size)
  {
    stream_->write((const char*)buf, size);
    return !stream_->fail();
  }

  size_t BinaryWriter::getPos() const { return stream_->tellp(); }
}

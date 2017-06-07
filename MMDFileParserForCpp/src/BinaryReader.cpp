#include <MMDFileParser/reader_helper.h>

namespace MMDFileParser
{
  BinaryReader::BinaryReader(const FilePath& path) : stream_(std::make_shared<std::ifstream>(path, std::ios::binary)) {}

  size_t BinaryReader::read(void* buf, int size)
  {
    stream_->read(static_cast<char*>(buf), size);
    return static_cast<size_t>(stream_->gcount());
  }

  size_t BinaryReader::getPos() const
  {
    return static_cast<size_t>(stream_->tellg());
  }

}
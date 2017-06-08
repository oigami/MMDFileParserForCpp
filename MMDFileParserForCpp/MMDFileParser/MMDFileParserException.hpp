#pragma once
#include <stdexcept>

namespace MMDFileParser
{
  class MMDFileParserException :public std::runtime_error
  {
    size_t m_pos;
  public:

    explicit MMDFileParserException(const std::string& message, size_t pos)
      : runtime_error(message), m_pos(pos)
    {
    }

    explicit MMDFileParserException(const char* message, size_t pos)
      : runtime_error(message), m_pos(pos)
    {
    }


    size_t ErrorPos()const { return m_pos; }
  };
}

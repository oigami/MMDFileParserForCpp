#pragma once
#include <stdexcept>

namespace MMDFileParser
{
  class MMDFileParserException :public std::runtime_error
  {
    int m_pos;
  public:

    explicit MMDFileParserException(const std::string& message, int pos)
      : runtime_error(message), m_pos(pos)
    {
    }

    explicit MMDFileParserException(const char* message, int pos)
      : runtime_error(message), m_pos(pos)
    {
    }


    int ErrorPos()const { return m_pos; }
  };
}

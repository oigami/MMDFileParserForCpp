#pragma once
#include <MMDFileParser/ParserHelper.hpp>
#include <MMDFileParser/VMDStruct.hpp>

namespace MMDFileParser
{
  class VMDWriter
  {
  public:

    static void Save(const FilePath& outFileName, const VMDStruct& vmd);
  };
}


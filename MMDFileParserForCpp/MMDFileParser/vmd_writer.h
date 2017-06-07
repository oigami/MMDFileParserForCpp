#pragma once
#include <MMDFileParser/reader_helper.h>
#include <MMDFileParser/vmd_struct.h>

namespace MMDFileParser
{
  class VMDWriter
  {
  public:

    static void Save(const FilePath& outFileName, const VMDStruct& vmd);
  };
}


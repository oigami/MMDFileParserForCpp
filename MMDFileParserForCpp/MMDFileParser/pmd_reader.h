#pragma once
#include <MMDFileParser/pmd_struct.h>
#include <MMDFileParser/reader_helper.h>

namespace MMDFileParser
{
  class PMDReader : public pmd_struct::PMDStruct
  {

    bool has_englishName;

    bool m_isLoaded = false;

  public:

    void open(IReader& reader);
    void open(const FilePath& path);

    explicit PMDReader(IReader& reader);
    explicit PMDReader(const FilePath& path);
    ~PMDReader();

    bool isOpened() const;

    explicit operator bool() const { return m_isLoaded; }


    static constexpr char HeaderFormat[] = "pmd";

    static bool MagicNumberEqual(const char* buf);

  };
}

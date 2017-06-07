#pragma once
#include <MMDFileParser/reader_helper.h>
#include <MMDFileParser/vmd_struct.h>
#include <memory>

namespace MMDFileParser
{
  class VMDReader : public VMDStruct
  {
    bool is_opened_;
    void openImpl(IReader& reader);
  public:

    VMDReader();

    /// <summary>VMDファイルからデータを取り出す</summary>
    /// <param name="file_name"></param>
    explicit VMDReader(const FilePath& file_name);

    /// <summary>VMDファイルからデータを取り出す</summary>
    /// <param name="file_name"></param>
    explicit VMDReader(IReader& reader);

    void open(const FilePath& file_name);

    void open(IReader& reader);

    void close();

    bool isOpened() const;

    explicit operator bool() const { return isOpened(); }

    String getModelName() const;

    static constexpr char HeaderFormat2[] = "Vocaloid Motion Data 0002";
  };
}

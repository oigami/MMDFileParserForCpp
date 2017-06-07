#include <MMDFileParser/VMDReader.hpp>
#include <MMDFileParser/ParserHelper.hpp>
#include <memory>
#include "MMDFileParser/MMDFileParserException.hpp"
#include "src/Detail/Helper.h"

namespace MMDFileParser
{
  namespace
  {
    template<class Type>
    bool ReadSizeAndArray(IReader& reader, std::vector<Type>& arr)
    {
      return detail::ReadSizeAndArray<typename Type::CountType>(reader, arr);
    }
  }

  bool VMDReader::isOpened() const
  {
    return is_opened_;
  }

  void VMDReader::openImpl(IReader& reader)
  {
    using namespace detail;
    if ( !Read(reader, header) )
    {
      throw MMDFileParserException("Read VMD header read error.", reader.getPos());
    }
    if ( strncmp(HeaderFormat2, header.header, sizeof(HeaderFormat2) - 1) != 0 )
    {
      throw MMDFileParserException("VMD header Mismatch error.\nAcutual : " + std::string(header.header, sizeof(header.header)), reader.getPos());
    }

    if ( !ReadSizeAndArray(reader, keyFrames) )
    {
      throw MMDFileParserException("VMD key frames read error.", reader.getPos());
    }

    if ( !ReadSizeAndArray(reader, morphFrames) )
    {
      throw MMDFileParserException("VMD morph frames read error.", reader.getPos());
    }
    if ( !ReadSizeAndArray(reader, cameraFrames) )
    {
      throw MMDFileParserException("VMD camera frames read error.", reader.getPos());
    }

    if ( !ReadSizeAndArray(reader, lightFrames) )
    {
      throw MMDFileParserException("VMD light frames read error.", reader.getPos());
    }

    // MMDv6.19以前で保存されたVMDはここまで

    if ( !ReadSizeAndArray(reader, selfShadowFrames) ) return;

    // MMDv7.39.x64以前で保存されたVMDはここまで

    vmd_struct::ShowIkWithoutArray::CountType showIKCount;
    if ( !Read(reader, showIKCount) ) return;

    showIKs.resize(showIKCount);
    for ( auto& i : showIKs )
    {
      if ( !Read(reader, static_cast<vmd_struct::ShowIkWithoutArray&>(i)) )
      {
        throw MMDFileParserException("VMD ik read error.", reader.getPos());
      }
      if ( !ReadSizeAndArray(reader, i.ik) )
      {
        throw MMDFileParserException("VMD ik read error.", reader.getPos());
      }
    }
  }

  VMDReader::VMDReader() :is_opened_(false) {}

  /// <summary>VMDファイルからデータを取り出す</summary>
  /// <param name="file_name"></param>
  VMDReader::VMDReader(const FilePath& file_name)
  {
    open(file_name);
  }

  VMDReader::VMDReader(IReader& reader) { open(reader); }

  void VMDReader::open(const FilePath& file_name)
  {
    BinaryReader reader(file_name);
    open(reader);
  }

  void VMDReader::open(IReader& reader)
  {
    try
    {
      openImpl(reader);
      is_opened_ = true;
    }
    catch ( ... )
    {
      close();
      throw;
    }
  }

  void VMDReader::close()
  {
    is_opened_ = false;
    morphFrames.clear();
    keyFrames.clear();
  }

  String VMDReader::getModelName() const
  {
    int endPos = sizeof(header.modelName);
    for ( int i = 0; i < sizeof(header.modelName); i++ )
    {
      if ( header.modelName[i] == '\0' )
      {
        endPos = i;
        break;
      }
    }
    return (std::string(header.modelName, endPos));
  }
}

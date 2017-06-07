#include <MMDFileParser/VMDWriter.hpp>
#include <MMDFileParser/ParserHelper.hpp>
#include <algorithm>
#include "src/Detail/Helper.h"
#include <MMDFileParser/MMDFileParserException.hpp>
#include <MMDFileParser/VMDStruct.hpp>

namespace MMDFileParser
{
  namespace
  {
    template<class Type>
    bool WriteSizeAndArray(IWriter& writer, const Array<Type>& arr)
    {
      return MMDFileParser::detail::WriteSizeAndArray<typename Type::CountType>(writer, arr);
    }
  }

  void VMDWriter::Save(const FilePath& outFileName, const VMDStruct& vmd)
  {
    BinaryWriter writer_(outFileName);
    using namespace detail;
    // ヘッダの書き込み
    Write(writer_, vmd.header);

    // ボーンフレームの書き込み
    if ( !WriteSizeAndArray(writer_, vmd.keyFrames) )
    {
      throw MMDFileParserException("VMD key frames write error", writer_.getPos());
    }

    // モーフフレームの書き込み
    if ( !WriteSizeAndArray(writer_, vmd.morphFrames) )
    {
      throw MMDFileParserException("VMD morph frames write error", writer_.getPos());
    }

    // カメラフレームの書き込み
    if ( !WriteSizeAndArray(writer_, vmd.cameraFrames) )
    {
      throw MMDFileParserException("VMD camera frames write error", writer_.getPos());
    }

    // ライトフレームの書き込み
    if ( !WriteSizeAndArray(writer_, vmd.lightFrames) )
    {
      throw MMDFileParserException("VMD light frames write error", writer_.getPos());
    }

    // セルフシャドウフレームの書き込み
    if ( !WriteSizeAndArray(writer_, vmd.selfShadowFrames) )
    {
      throw MMDFileParserException("VMD selfshadow frames write error", writer_.getPos());
    }

    // Ikフレームの書き込み
    if ( !Write(writer_, static_cast<vmd_struct::ShowIkWithoutArray::CountType>(vmd.showIKs.size())) )
    {
      throw MMDFileParserException("VMD ik size write error", writer_.getPos());
    }

    for ( auto& i : vmd.showIKs )
    {
      if ( !Write(writer_, static_cast<const vmd_struct::ShowIkWithoutArray&>(i)) )
      {
        throw MMDFileParserException("VMD ik write error", writer_.getPos());
      }
      if ( !WriteSizeAndArray(writer_, i.ik) )
      {
        throw MMDFileParserException("VMD ik array write error", writer_.getPos());
      }
    }
  }
}

#include <MMDFileParser/PMDReader.hpp>
#include <MMDFileParser/ParserHelper.hpp>
#include "src/Detail/Helper.h"
#include <MMDFileParser/MMDFileParserException.hpp>

namespace MMDFileParser
{
  namespace
  {
    template<class T>
    void ReadOrThrow(IReader& reader, T& t, const std::string err_msg)
    {
      if ( !detail::Read(reader, t) )
        throw MMDFileParserException(err_msg, reader.getPos());
    }
  }
  PMDReader::PMDReader(const FilePath& path)
  {
    open(path);
  }

  PMDReader::PMDReader(IReader& reader)
  {
    open(reader);
  }

  bool PMDReader::isOpened() const { return m_isLoaded; }

  bool PMDReader::MagicNumberEqual(const char* buf)
  {
    return strncmp(buf, HeaderFormat, sizeof(HeaderFormat) - 1) == 0;
  }

  void PMDReader::open(IReader& reader)
  {
    using namespace detail;

    { // ヘッダデータ
      if ( !Read(reader, m_header_) )
      {
        throw MMDFileParserException("PMD header read error.", reader.getPos());
      }

      auto magic = reinterpret_cast<char*>(m_header_.magic);
      if ( !MagicNumberEqual(magic) )
      {
        throw MMDFileParserException("PMD header magic mismatch error.\n  Actual : " + std::string(magic, sizeof(m_header_.magic)), reader.getPos());
      }
    }

    // 頂点データ
    if ( !ReadSizeAndArray<std::uint32_t>(reader, m_vertices) )
    {
      throw MMDFileParserException("PMD vertices read error.", reader.getPos());
    }
    // ポリゴンデータ
    if ( !ReadSizeAndArray<std::uint32_t>(reader, m_faces) )
    {
      throw MMDFileParserException("PMD faces read error.", reader.getPos());
    }
    // 材料データ
    if ( !ReadSizeAndArray<std::uint32_t>(reader, m_materials) )
    {
      throw MMDFileParserException("PMD materials read error.", reader.getPos());
    }
    // ボーンデータ
    if ( !ReadSizeAndArray<std::uint16_t>(reader, m_bones) )
    {
      throw MMDFileParserException("PMD bones read error.", reader.getPos());
    }

    // IKデータ
    std::uint16_t numPmdIkData;
    if ( !Read(reader, numPmdIkData) )
    {
      throw MMDFileParserException("PMD ik count read error.", reader.getPos());
    }
    m_ikData.resize(numPmdIkData);
    for ( auto& pmdIkData : m_ikData )
    {
      ReadOrThrow(reader, pmdIkData.ik_bone_index, "PMD ik bone index read error.");
      ReadOrThrow(reader, pmdIkData.ik_target_bone_index, "PMD ik target bone read error.");
      std::uint8_t ik_child_bone_length;
      ReadOrThrow(reader, ik_child_bone_length, "PMD ik child bone length read error.");
      ReadOrThrow(reader, pmdIkData.iterations, "PMD ik iterations read error.");
      ReadOrThrow(reader, pmdIkData.control_weight, "PMD ik control weight read error.");
      if ( !ReadArray(reader, ik_child_bone_length, pmdIkData.ik_child_bone_index) )
      {
        throw MMDFileParserException("PMD ik chid bone index read error.", reader.getPos());
      }
    }

    // 表情データ
    std::uint16_t numPmdSkin;
    ReadOrThrow(reader, numPmdSkin, "PMD num skin read error.");
    m_skinData.resize(numPmdSkin);
    for ( auto& skinData : m_skinData )
    {
      pmd_struct::SkinDataWithoutArray* pmdSkinDataWithoutArray = &skinData;
      ReadOrThrow(reader, *pmdSkinDataWithoutArray, "PMD skin data read error.");
      if ( !ReadArray(reader, skinData.skin_vert_count, skinData.skin_vert_data) )
      {
        throw MMDFileParserException("PMD skin vertex read error.", reader.getPos());
      }
    }

    // 表情用枠表示リスト
    if ( !ReadSizeAndArray<std::uint8_t>(reader, m_skinIndices) )
    {
      throw MMDFileParserException("PMD skin indeices read error.", reader.getPos());
    }

    // ボーン枠用枠名リスト
    if ( !ReadSizeAndArray<std::uint8_t>(reader, m_boneDispNames) )
    {
      throw MMDFileParserException("PMD bone disp names read error.", reader.getPos());
    }
    const size_t numBoneDispName = m_boneDispNames.size();

    // ボーン枠用表示リスト
    if ( !ReadSizeAndArray<std::uint32_t>(reader, m_boneDisps) )
    {
      throw MMDFileParserException("PMD bone disps read error.", reader.getPos());
    }

    // 英名対応
    std::uint8_t english_name_compatibility = 0;
    ReadOrThrow(reader, english_name_compatibility, "PMD english name cpmpatibility read error.");

    // 各種英名
    if ( english_name_compatibility == 1 )
    {
      pmd_struct::EnglishName tmp;
      ReadOrThrow(reader, tmp.modelName, "PMD english model name read error."); // モデル名
      ReadOrThrow(reader, tmp.comment, "PMD english comment read error."); // コメント

      // ボーンリスト
      if ( !ReadArray(reader, static_cast<int>(m_bones.size()), tmp.boneName) )
      {
        throw MMDFileParserException("PMD english bone name read error.", reader.getPos());
      }

      if ( numPmdSkin )
      {
        // 表情リスト
        if ( !ReadArray(reader, numPmdSkin - 1, tmp.skinName) )
        {
          throw MMDFileParserException("PMD english skin name read error.", reader.getPos());
        }
      }

      // ボーン枠用枠名リスト
      if ( !ReadArray(reader, static_cast<int>(numBoneDispName), tmp.boneDispName) )
      {
        throw MMDFileParserException("PMD english bone disp name read error.", reader.getPos());
      }
      m_englishName = tmp;
    }

    // トゥーンテクスチャリスト
    constexpr int numToonFileName = 10;
    Array<pmd_struct::ToonTexture> toonFileName;
    if ( !ReadArray(reader, numToonFileName, toonFileName) )
    {
      throw MMDFileParserException("PMD toon filenames read error.", reader.getPos());
    }

    // 剛体データ
    if ( !ReadSizeAndArray<std::uint32_t>(reader, m_rigidBodies) )
    {
      throw MMDFileParserException("PMD rigid bodies read error.", reader.getPos());
    }

    // ジョイントデータ
    if ( !ReadSizeAndArray<std::uint32_t>(reader, m_joints) )
    {
      throw MMDFileParserException("PMD joints read error.", reader.getPos());
    }

    m_isLoaded = true;
  }

  void PMDReader::open(const FilePath& path)
  {
    BinaryReader reader(path);
    open(reader);
  }

  PMDReader::~PMDReader() {}
}

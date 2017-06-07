#pragma once
#include <array>
#include <MMDFileParser/ParserHelper.hpp>
namespace MMDFileParser
{

  namespace vmd_struct
  {
    static_assert(std::is_pod<math::Float2>::value, "");
    static_assert(std::is_pod<math::Float3>::value, "");
    static_assert(std::is_pod<math::Float4>::value, "");
    static_assert(std::is_pod<math::Vector4D<std::uint8_t>>::value, "");
    static_assert(sizeof(math::Vector4D<std::uint8_t>) == 4, "");
#pragma pack(push, 1)

    //http://harigane.at.webry.info/201103/article_1.html

    /// VMD構造体定義
    struct Header
    {
      char header[30];
      char modelName[20];
    };

    struct Bone
    {
      using CountType = std::uint32_t;
      char boneName[15];
      std::uint32_t frameNo;
      math::Float3 location; // 移動量
      math::Float4 rotation; // モデルローカル座標系
      math::Vector4D<std::uint8_t> x1; // 軸の補完パラメータ (x,y,z,r)
      math::Vector4D<std::uint8_t> y1; // 軸の補完パラメータ (x,y,z,r)
      math::Vector4D<std::uint8_t> x2; // 軸の補完パラメータ (x,y,z,r)
      math::Vector4D<std::uint8_t> y2; // 軸の補完パラメータ (x,y,z,r)

      std::array<std::uint8_t, 16> _interpolation1;
      std::array<std::uint8_t, 16> _interpolation2;
      std::array<std::uint8_t, 16> _interpolation3;
    };

    struct Morph
    {
      using CountType = std::uint32_t;
      char name[15];
      std::uint32_t frameNo;
      float m_weight;

      bool operator<(const Morph& m) const { return frameNo < m.frameNo; }
    };

    struct Camera
    {
      using CountType = std::uint32_t;
      std::uint32_t frameNo; // フレーム番号
      float distance; // 目標点とカメラの距離(目標点がカメラ前面でマイナス)
      math::Float3 lookAtPos; // 目標点
      math::Float3 rotation; // カメラの回転(rad)(x軸はMMD数値入力のマイナス値)
      std::uint8_t bezier[24]; // 補間パラメータ
      std::uint32_t viewAngle; // 視野角(deg)
      std::uint8_t parth; // パースペクティブ, 0:ON, 1:OFF
    };

    struct Light
    {
      using CountType = std::uint32_t;
      std::uint32_t frameNo; // フレーム番号
      float r; // 照明色赤(MMD入力値を256で割った値)
      float g; // 照明色緑(MMD入力値を256で割った値)
      float b; // 照明色青(MMD入力値を256で割った値)
      float x; // 照明x位置(MMD入力値)
      float y; // 照明y位置(MMD入力値)
      float z; // 照明z位置(MMD入力値)
    };

    struct SelfShadow
    {
      using CountType = std::uint32_t;
      std::uint32_t frameNo; // フレーム番号
      std::uint8_t type; // セルフシャドウ種類, 0:OFF, 1:mode1, 2:mode2
      float distance; // シャドウ距離(MMD入力値Lを(10000-L)/100000とした値)
    };

    struct InfoIk
    {
      using CountType = std::uint32_t;
      char name[20]; // "右足ＩＫ\0"などのIKボーン名の文字列 20byte
      std::uint8_t on_off; // IKのon/off, 0:OFF, 1:ON
    };

    struct ShowIkWithoutArray
    {
      using CountType = std::uint32_t;
      std::uint32_t frameNo; // フレーム番号
      std::uint8_t show; // モデル表示, 0:OFF, 1:ON

      //InfoIK ik[ik_count];  // IK on/off情報配列
    };

#pragma pack(pop)

    struct ShowIk : ShowIkWithoutArray
    {
      Array<InfoIk> ik;
    };
  }

  struct VMDStruct
  {
    vmd_struct::Header header;
    Array<vmd_struct::Bone> keyFrames;
    Array<vmd_struct::Morph> morphFrames;
    Array<vmd_struct::Camera> cameraFrames;
    Array<vmd_struct::Light> lightFrames;
    Array<vmd_struct::SelfShadow> selfShadowFrames;
    Array<vmd_struct::ShowIk> showIKs;
  };
}

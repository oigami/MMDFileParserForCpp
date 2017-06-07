#include "pch.h"
#include <MMDFileParser/VMDReader.hpp>
#include <experimental/filesystem>
#include "MMDFileParser/VMDWriter.hpp"
namespace filesystem = std::experimental::filesystem;

TEST(VMDReader, HeaderCheck)
{
  current_path(filesystem::current_path() / ".." / "GTest1");
  MMDFileParser::VMDReader reader("kisimen.vmd");
  MMDFileParser::VMDStruct& vmd = reader;

  EXPECT_TRUE(reader.isOpened());
  EXPECT_EQ(vmd.header.header, std::string(MMDFileParser::VMDReader::HeaderFormat2));
  EXPECT_EQ(vmd.header.modelName, std::string("初音ミク(メタル服)"));

  EXPECT_EQ(vmd.keyFrames.size(), 3655);
  EXPECT_EQ(vmd.cameraFrames.size(), 0);
  EXPECT_EQ(vmd.lightFrames.size(), 0);
  EXPECT_EQ(vmd.morphFrames.size(), 256);
  EXPECT_EQ(vmd.selfShadowFrames.size(), 0);
  EXPECT_EQ(vmd.showIKs.size(), 1);

}

TEST(VMDWriter, HeaderCheck)
{
  current_path(filesystem::current_path() / ".." / "GTest1");
  MMDFileParser::VMDReader reader("kisimen.vmd");
  const MMDFileParser::VMDStruct vmd = reader;
  MMDFileParser::VMDWriter::Save("OutHeaderCheck.vmd", vmd);
  reader.open("OutHeaderCheck.vmd");

  EXPECT_TRUE(reader.isOpened());
  EXPECT_EQ(vmd.header.header, std::string(reader.header.header));
  EXPECT_EQ(vmd.header.modelName, std::string(reader.header.modelName));

  EXPECT_EQ(vmd.keyFrames.size(), reader.keyFrames.size());
  EXPECT_EQ(vmd.cameraFrames.size(), reader.cameraFrames.size());
  EXPECT_EQ(vmd.lightFrames.size(), reader.lightFrames.size());
  EXPECT_EQ(vmd.morphFrames.size(), reader.morphFrames.size());
  EXPECT_EQ(vmd.selfShadowFrames.size(), reader.selfShadowFrames.size());
  EXPECT_EQ(vmd.showIKs.size(), reader.showIKs.size());

}
""" Create Nuget Tool """
# pylint: disable=C0103
# pylint: disable=C0325

import subprocess
import os
import shutil

msbuildPath = "\"C:/Program Files (x86)/Microsoft Visual Studio/2017/"\
    "Community/MSBuild/15.0/Bin/msbuild\" /m "


def Compile(command):
    print(command)
    subprocess.call(command, shell=True)


def MakeCommand(vsVersion, platform, configuration, runtimeLibraryType):
    cmd = msbuildPath +\
        " ../../MMDFileParserForCpp/MMDFileParserForCpp.vcxproj /p:configuration={1} "\
        "/p:OutDir=../nuget/work/lib/native/{0}/lib/ /p:Platform=\"{0}\" "\
        "/p:Targetname={2} /p:PlatformToolset={3} "

    targetName = "MMDFileParserForCpp-{}-{}{}".format(vsVersion, runtimeLibraryType,
                                                      "-d" if configuration == "Debug" else "")
    if runtimeLibraryType == "MD":
        configuration += runtimeLibraryType
    cmd = cmd.format(platform, configuration, targetName, vsVersion)
    return cmd


outputDir = "Output/"
if os.path.exists(outputDir):
    shutil.rmtree(outputDir)

os.mkdir(outputDir)

os.chdir("Output")
os.makedirs("build/native/include/MMDFileParser/MMDFileParser")

# nugetヘッダのコピー
shutil.copy("../MMDFileParserForCpp.nuspec", "./")
shutil.copy("../MMDFileParserForCpp.targets", "./build/native")

# ヘッダファイルのコピー
shutil.copytree("../../MMdFileParserForCpp/MMDFileParser/",
                "./lib/native/include/MMDFileParser/MMDFileParser")
shutil.copy("../../MMdFileParserForCpp/MMDFileParser.hpp", "./lib/native/include/MMDFileParser")

# プロジェクトのビルド
print("echo Compile VS15")

Compile(MakeCommand("v141", "Win32", "Debug", "MT"))
Compile(MakeCommand("v141", "Win32", "Release", "MT"))

Compile(MakeCommand("v141", "x64", "Debug", "MT"))
Compile(MakeCommand("v141", "x64", "Release", "MT"))

Compile(MakeCommand("v141", "Win32", "Debug", "MD"))
Compile(MakeCommand("v141", "Win32", "Release", "MD"))

Compile(MakeCommand("v141", "x64", "Debug", "MD"))
Compile(MakeCommand("v141", "x64", "Release", "MD"))

# nuget packageの作成
subprocess.call("nuget pack MMDFileParserForCpp.nuspec", shell=True)

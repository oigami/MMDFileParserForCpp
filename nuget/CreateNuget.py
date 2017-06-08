""" Create Nuget Tool """
# pylint: disable=C0103

import subprocess
import os
import shutil

print("echo Compile VS15")
os.chdir("work")
subprocess.call("dir", shell=True)
shutil.rmtree("./lib/native/include")
shutil.copytree("../../MMdFileParserForCpp/MMDFileParser/", "./lib/native/include")

msbuildPath = "\"C:/Program Files (x86)/Microsoft Visual Studio/2017/"\
    "Community/MSBuild/15.0/Bin/msbuild\" "

cmd = msbuildPath + " ../../MMDFileParserForCpp.sln /p:configuration={1} "\
    "/p:OutputPath=./lib/native/{0}/lib  /p:Platform=\"{0}\""

# Debug
c = cmd.format("x64", "Debug")
print(c)
subprocess.call(c, shell=True)

# Release
c = cmd.format("x64", "Release")
print(c)
subprocess.call(c, shell=True)

subprocess.call("cd ..")

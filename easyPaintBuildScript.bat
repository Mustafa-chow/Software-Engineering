@rem setup path.. change these according to your computer settings

SET PATH=C:\Users\Home_College\Documents\Dattathreya\cmake-3.12.0-win64-x64\bin;C:\Qt\Qt5.9.1\5.9.1\msvc2017_64;%PATH%

@ECHO \nBuilding easyPaintApplication

rem define variables for each of the variables used
rem replace these values with your own values if the directories are different

SET GEN_INSTANCE="C:/Program Files (x86)/Microsoft Visual Studio/2017/Community"
SET QT_PREFIX=C:\Qt\Qt5.9.1\5.9.1\msvc2017_64
SET MACHINE_TYPE="Visual Studio 15 2017 Win64"

rem Here .. refers to the two layers above build directory
SET PROJECT_FILE=../CSC4111W19GRP2/Project/
SET BUILD_DIR=bin
SET PROJECT_NAME=easypaintGrp2.sln

If NOT EXIST ..\%BUILD_DIR% MKDIR ..\%BUILD_DIR%
cd ../%BUILD_DIR%
cmake %PROJECT_FILE% -DCMAKE_PREFIX_PATH=%QT_PREFIX% -G %MACHINE_TYPE% -DCMAKE_GENERATOR_INSTANCE=%GEN_INSTANCE%

IF %MACHINE_TYPE%=="Visual Studio 15 2017 Win64" CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
msbuild.exe %PROJECT_NAME% /clp:ErrorsOnly
@ECHO \nCompleted building the project 

rem execute the application
SET EXE_DIR=../bin/Debug
SET EXE_FIL_NAME=easypaintGrp2.exe
cd %EXE_DIR%
%EXE_FIL_NAME%
pause




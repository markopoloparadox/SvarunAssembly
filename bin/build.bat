@echo off
cd ..

rem if exist build (
rem 	rd /s /q  build
rem )
mkdir build
cd build
cmake ../src
cmake --build .
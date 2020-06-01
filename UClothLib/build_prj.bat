mkdir build
cd build
cmake .. -G"Visual Studio 16 2019"
"msbuild.exe" ucloth.sln /target:ALL_BUILD
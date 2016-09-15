pushd 3rdparty
call openssl_download_build64.bat
popd
pushd premake5
premake5.exe vs2015
popd
set MSBUILD=C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe
"%MSBUILD%" _build_premake\crtmpserver.sln /p:Conifguration=Release /p:Platform=Win64


call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
msbuild -verbosity:minimal -t:Rebuild -p:Configuration="Debug" "C:\work\wingstall-mod\packages\example\deflate\program\setup.sln" > "C:\work\wingstall-mod\packages\example\deflate\compile\debug_stdout.txt" 2> "C:\work\wingstall-mod\packages\example\deflate\compile\debug_stderr.txt"

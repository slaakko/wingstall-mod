call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
msbuild -verbosity:minimal -t:Rebuild -p:Configuration="Release" "C:\work\wingstall-mod\setup\deflate\program\setup.sln" > "C:\work\wingstall-mod\setup\deflate\compile\release_stdout.txt" 2> "C:\work\wingstall-mod\setup\deflate\compile\release_stderr.txt"

xcopy "..\build-Qt_5_12_6_MinGW_32\release\RegExTerm.exe" "d:\personal\dma\portable\RegExTerm\RegExTerm.exe"

xcopy /Y "c:\tools\qt\Tools\mingw730_32\bin\libgcc_s_dw2-1.dll"  "d:\personal\dma\portable\RegExTerm\libgcc_s_dw2-1.dll"
xcopy /Y "c:\tools\qt\Tools\mingw730_32\bin\libstdc++-6.dll"     "d:\personal\dma\portable\RegExTerm\libstdc++-6.dll"
xcopy /Y "c:\tools\qt\Tools\mingw730_32\bin\libwinpthread-1.dll" "d:\personal\dma\portable\RegExTerm\libwinpthread-1.dll"

c:\tools\qt\5.12.6\mingw73_32\bin\windeployqt.exe ^
    "d:\personal\dma\portable\RegExTerm\RegExTerm.exe"

pause

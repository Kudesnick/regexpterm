xcopy "..\build-RegExTerm-Desktop_Qt_5_12_12_MinGW_32_bit-Release\release\RegExTerm.exe" "d:\personal\dma\portable\RegExTerm\RegExTerm.exe"

xcopy /Y "c:\tools\Qt5.12.12\Tools\mingw730_32\\bin\libgcc_s_dw2-1.dll"  "d:\personal\dma\portable\RegExTerm\libgcc_s_dw2-1.dll"
xcopy /Y "c:\tools\Qt5.12.12\Tools\mingw730_32\\bin\libstdc++-6.dll"     "d:\personal\dma\portable\RegExTerm\libstdc++-6.dll"
xcopy /Y "c:\tools\Qt5.12.12\Tools\mingw730_32\\bin\libwinpthread-1.dll" "d:\personal\dma\portable\RegExTerm\libwinpthread-1.dll"

c:\tools\Qt5.12.12\5.12.12\mingw73_32\bin\windeployqt.exe ^
    "d:\personal\dma\portable\RegExTerm\RegExTerm.exe"

pause

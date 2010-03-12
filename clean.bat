REM erase the .ncb
erase eternity*.ncb

REM remove the Build folder (has compiled .exe files in it)
rd .\Build /s /q
rd .\Release /s /q

REM remove all vcproj.details.user files
erase *.vcproj.*.user /s

REM delete all "lastRunLog.txt" 's
erase lastRunLog.txt /s


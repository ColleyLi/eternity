REM erase the intellisense files
erase *.sdf
erase *.opensdf
rd .\ipch /s /q

REM remove the Build folder (has compiled .exe files in it)
rd .\Build /s /q
rd .\Debug /s /q
rd .\Release /s /q

REM delete all "lastRunLog.txt" 's
erase lastRunLog.txt /s


REM erase the .ncb
erase eternity.ncb

REM remove the debug and release folders
rd .\Debug /s /q
rd .\Release /s /q

rd .\eternity\Debug /s /q
rd .\eternity\Release /s /q

rd .\game\Debug /s /q
rd .\game\Release /s /q

REM remove the vcproj.details.user files
erase .\eternity\eternity.vcproj.*.user
erase .\game\game.vcproj.*.user


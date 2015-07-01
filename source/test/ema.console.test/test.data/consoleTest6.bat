@copy /Y consoleTest6.data consoleTest6.data1 > nul
@if not "%1"=="" powershell -NoLogo -Noninteractive -Command echo %1 ^| Out-File consoleTest6.data1 unicode -Append ^; < nul
@type consoleTest6.data1  
@rem @del consoleTest6.data1  


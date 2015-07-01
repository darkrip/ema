@copy /Y consoleTest3.data consoleTest3.data1 > nul
@if not "%1"=="" powershell echo %1 ^| Out-File consoleTest3.data1 unicode -Append < nul
@type consoleTest3.data1  
@del consoleTest3.data1  


@copy /Y consoleTest3.data consoleTest3.data1 > nul
@if not "%1"=="" echo %1>> consoleTest3.data1
@type consoleTest3.data1  
@del consoleTest3.data1  


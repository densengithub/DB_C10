mkdir Release
pushd %1 & for %%i in (.) do set curr=%%~ni
copy .\project\%curr%\*.PKG	.\Release\
copy .\project\%curr%\*.ROM	.\Release\
copy .\project\bootloader\kproc.sys	.\Release\
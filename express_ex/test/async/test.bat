
IF EXIST out.dat  DEL /f out.dat

exho %0
set Path=C:\Qt\5.12.8\msvc2017_64\bin;%Path%

IF "%1"=="" (
    set PATH_TO_EXE=.
    set EX_ARG=--opt
)

IF NOT "%1"=="" (
    set PATH_TO_EXE=%1
    set EX_ARG=%2
)



set EXPRESS_FILE="calc.express"
::@echo off
::%PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i -untypedFSR --allFSR --tableSSR --outputPrm  --llvmIRcode --runJit %EX_ARG%
::for %%i in ("double_async_f4src.kex" "double_async_f8src.kex" "single_async_f8src.kex" ) do (
for %%i in ("double_async_f8src.kex" "single_async_f8src.kex" ) do (
    %PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i --llvmIRcode  %EX_ARG% >  output.ll
    %PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i -untypedFSR --allFSR --tableSSR --outputPrm  --llvmIRcode --runJit %EX_ARG%
    ..\ex_test.py   --db=%EXPRESS_FILE% -i=%%i
    IF EXIST out.dat  DEL /f out.dat
)

for %%i in ("double_async_f4src.kex" ) do (
    %PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i --llvmIRcode  %EX_ARG% >  output.ll
    %PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i -untypedFSR --allFSR --tableSSR --outputPrm  --llvmIRcode --runJit %EX_ARG%
    ..\ex_test.py   --db=%EXPRESS_FILE% -i=%%i --otype=f4
    IF EXIST out.dat  DEL /f out.dat
)


::%PATH_TO_EXE%/express_ex_app.exe   --db=calc.express -i=double_async_f4src.kex -untypedFSR --allFSR --tableSSR --outputPrm --llvmIRcode --runJit
:: --nameList       - names list
::      --untypedFSR     - FSR(first stage representation) without type calculation
::      --activeNameList - Procedure Integration
::      --allFSR         - full FSR(first stage representation)
::      --redusedFSR     - redused FSR code
::      --tableSSR       - table (second stage) representation
::      --outputPrm      - output parameters
::      --llvmIRcode     - llvm IR
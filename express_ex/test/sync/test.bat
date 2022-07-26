
IF EXIST out.dat  DEL /f out.dat

exho %0

set path=C:\Qt\5.12.8\msvc2017_64\bin;C:\Qt\5.14.2\msvc2017_64\bin;%path%

IF "%1"=="" (
    set PATH_TO_EXE=.
    set EX_ARG=--opt
)

IF NOT "%1"=="" (
    set PATH_TO_EXE=%1
    set EX_ARG=%2
)


set EXPRESS_FILE="sync_base.express"
::--db="sync_base.express" -i="single_arg_convolve.kex"  -untypedFSR --allFSR --tableSSR --outputPrm  --llvmIRcode --runJit
::for %%i in ("single_arg_convolve.kex") do (

::%PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i --llvmIRcode  %EX_ARG% >  output.ll
for %%i in ( "recurrence_relation.kex" "integrate.kex" "ternary.kex" "multi_arg.kex" "multi_arg_cos.kex" "single_arg_convolve.kex") do (
    %PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i -untypedFSR --allFSR --tableSSR --outputPrm  --llvmIRcode --runJit %EX_ARG%
    ..\ex_test.py   --db=%EXPRESS_FILE% -i=%%i
    IF EXIST out.dat  DEL /f out.dat
)



::..\ex_test.py   --db=%EXPRESS_FILE% -i=%%i
::IF EXIST out_0.dat  DEL /f out_0.dat 
::%PATH_TO_EXE%/express_ex_app.exe   --db=calc.express -i=double_async_f4src.kex -untypedFSR --allFSR --tableSSR --outputPrm --llvmIRcode --runJit
:: --nameList       - names list
::      --untypedFSR     - FSR(first stage representation) without type calculation
::      --activeNameList - Procedure Integration
::      --allFSR         - full FSR(first stage representation)
::      --redusedFSR     - redused FSR code
::      --tableSSR       - table (second stage) representation
::      --outputPrm      - output parameters
::      --llvmIRcode     - llvm IR
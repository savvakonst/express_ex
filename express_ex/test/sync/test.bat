
IF EXIST out_0.dat  DEL /f out_0.dat 

exho %0

IF "%1"=="" (
    set PATH_TO_EXE=.
    set EX_ARG=--opt
)

IF NOT "%1"=="" (
    set PATH_TO_EXE=%1
    set EX_ARG=%2
)


set EXPRESS_FILE="sync_base.express"

::for %%i in ("multi_arg.kex" "multi_arg_cos.kex" "single_arg_convolve.kex") do (
for %%i in ("single_arg_convolve.kex") do (
    %PATH_TO_EXE%/express_ex_app.exe   --db=%EXPRESS_FILE% -i=%%i  -untypedFSR --allFSR --tableSSR --outputPrm  --llvmIRcode --runJit %EX_ARG%
    ..\ex_test.py   --db=%EXPRESS_FILE% -i=%%i
    IF EXIST out_0.dat  DEL /f out.dat
)



::..\ex_test.py   --db=%EXPRESS_FILE% -i=%%i
::IF EXIST out_0.dat  DEL /f out_0.dat 
::C:\Express_expr_compiler\express\express_ex\x86\Debug\express_ex   --db=calc.express -i=double_async_f4src.kex -untypedFSR --allFSR --tableSSR --outputPrm --llvmIRcode --runJit
:: --nameList       - names list
::      --untypedFSR     - FSR(first stage representation) without type calculation
::      --activeNameList - Procedure Integration
::      --allFSR         - full FSR(first stage representation)
::      --redusedFSR     - redused FSR code
::      --tableSSR       - table (second stage) representation
::      --outputPrm      - output parameters
::      --llvmIRcode     - llvm IR
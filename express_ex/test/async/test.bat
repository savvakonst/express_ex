
IF EXIST out_0.dat  DEL /f out_0.dat 

exho %0

IF "%1"=="" (
    set CONF=Debug
    set ARCH=x64
)

IF NOT "%1"=="" (
    set CONF=%1
    set ARCH==%2
)



set EXPRESS_FILE="calc.express"


::@echo off

for %%i in ("double_async_f4src.kex" "double_async_f8src.kex" "single_async_f8src.kex" ) do (
    C:\Express_expr_compiler\express\express_ex\%ARCH%\%CONF%\express_ex   --db=%EXPRESS_FILE% -i=%%i --llvmIRcode --runJit
    ..\ex_test.py   --db=%EXPRESS_FILE% -i=%%i
    IF EXIST out_0.dat  DEL /f out_0.dat 
)



::C:\Express_expr_compiler\express\express_ex\x86\Debug\express_ex   --db=calc.express -i=double_async_f4src.kex -untypedFSR --allFSR --tableSSR --outputPrm --llvmIRcode --runJit
:: --nameList       - names list
::      --untypedFSR     - FSR(first stage representation) without type calculation
::      --activeNameList - Procedure Integration
::      --allFSR         - full FSR(first stage representation)
::      --redusedFSR     - redused FSR code
::      --tableSSR       - table (second stage) representation
::      --outputPrm      - output parameters
::      --llvmIRcode     - llvm IR
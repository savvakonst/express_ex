# Процесс сборки и конфигурирования

стандартная конфигурация
`cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER="путь к C компилятору" -DCMAKE_CXX_COMPILER="путь к C++ компилятору"
-DCMAKE_PREFIX_PATH="директория Qt целевой платформы" -G Ninja -DCMAKE_VERBOSE_MAKEFILE=1 -DENABLE_TESTING=ON "директория проекта"`

пример:
`cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx64/x64/cl.exe" "-DCMAKE_CXX_COMPILER=C:/Program Files (x86)
/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx64/x64/cl.exe" -DCMAKE_PREFIX_PATH=C:
/Qt/5.12.8/msvc2017_64 -G Ninja -DCMAKE_VERBOSE_MAKEFILE=1 -DENABLE_TESTING=ON C:\Express_expr_compiler\express`

## Зависимости

### опционально используемые программы

* при наличии python3 и можно провести тестирование. Установить необходимые модули для python можно следующим образом
  ```
  pip install numpy  
  pip install matplotlib
  ```

* при наличии [Doxygen](https://doxygen.nl/download.html) и [Graphviz](https://graphviz.org/download/)
  можно построить документацию (на данный момент она бедна).

### используемые проекты/библиотеки

Вообще говоря Cmake может сам подтянуть все зависимости к проекту кроме Qt во время генерации, для этого не нужны
никакие "дополнительные" аргументы. Cmake разместит эти "подпроекты"
в каталоге "external_projects", при желании можно изменить каталог с промощью аргумента cmake
`-DEXTERNAL_PROJECTS_DIRECTORY= "некий каталог"`.
> Внимание! при автоматической загрузке зависимостей и использовании в качестве генератора Ninja Multi-Config
> перед сборкой цели express_ex возможно понадобится сначала собрать отдельно цель build_llvm

Далее идут инструкции для ручного подключения зависимостей:

* [llvm](https://llvm.org/docs/GettingStarted.html).  
  Примеры конфигурирования проекта с ручным подключением llvm:
    * с
      ninja `cmake -DCMAKE_PREFIX_PATH=C:/Qt/5.12.8/msvc2017_64;путь к папке где был собран llvm"  -G "Visual Studio 16 2019" -A x64`
    * с Visual
      Studio  `make -DCMAKE_PREFIX_PATH=C:/Qt/5.12.8/msvc2017_64;путь к llvm-build" -G "Visual Studio 16 2019" -A x64`

  Как установить и сконфигурировать llvm не думая):
    * извлечение LLVM:
      необходимо скачать версию ниже 14, т.к. в ней начинается переход к "Opaque Pointers" и, как следствие, большие
      изменения в API.
        * для Windows: `git clone --depth 1 --config core.autocrlf=false --branch llvmorg-12.0.1
          https://github.com/llvm/llvm-project.git`
        * для Linux: `git clone --depth 1 --branch llvmorg-12.0.1  https://github.com/llvm/llvm-project.git`
    * конфигурировани LLVM
      `cmake -G "Ninja"
      -DCMAKE_C_COMPILER="путь к C компилятору"
      -DCMAKE_CXX_COMPILER="путь к C++ компилятору"
      -DLLVM_INCLUDE_TESTS=OFF  
      -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_TOOLS=OFF  
      -DLLVM_INCLUDE_DOCS=OFF -DLLVM_TARGETS_TO_BUILD=X86`
    * сборка LLVM `cmake --build "путь к папке"
      --target LLVMCore LLVMExecutionEngine LLVMInterpreter LLVMMC LLVMMCJIT LLVMSupport LLVMX86CodeGen LLVMX86Desc LLVMX86Info`
* [antlr 4](https://www.antlr.org/).  
  Если у вас нет внешнего проекта antlr 4 nj проще всего не делать ничего, во время генерации проекта cmake сам загрузит
  c помощью git проект antlr 4 нужной версии в папку external_projects и подключит к проекту.

  Подключение внешнего проекта. Совместимость данного проекта проверена с версией antlr 4.10.1. Для уже имеющегося
  пакета (cmake-package)  antlr 4, местоположение которого можно задать с помощью аргумента командной строки:  
  `-DCMAKE_PREFIX_PATH="директория пакета"`.  
  Если скомпилированного пакета у вас нет, то сначала вам нужно собрать antlr 4 в нужной для вас концигурации,
  > подсказка: при сборке antlr 4 используйте следующие аргументы cmake:  
  `-DANTLR_BUILD_CPP_TESTS=OFF`, `-DANTLR4_INSTALL=ON`, `-DWITH_STATIC_CRT=OFF`

  а затем установить пакет в подходящую для вас директорию:  
  `cmake --install "директория сборки" --prefix "директория пакета" `

* QT.  
  При генерации проекта укажите путь к пакету QT. c помощью аргумента `CMAKE_PREFIX_PATH`

`git filter-branch --force --index-filter "git rm --cached --ignore-unmatch *.exe" --prune-empty --tag-name-filter cat -- --all`
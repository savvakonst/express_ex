# Процесс сборки

## Зависимости

* llvm.  
  * извлечение LLVM:
    необходимо скачать версию ниже 14, т.к. в ней начинается переход к "Opaque Pointers" и,
    как следствие, большие изменения в API.
    * для Windows: `git clone --depth 1 --config core.autocrlf=false --branch llvmorg-12.0.1  https://github.com/llvm/llvm-project.git`
    * для Linux: `git clone --depth 1  --branch llvmorg-12.0.1  https://github.com/llvm/llvm-project.git`
  * build LLVM
  `cmake -G "Ninja" 
  -DLLVM_INCLUDE_TESTS=OFF  
  -DLLVM_INCLUDE_BENCHMARKS=OFF 
  -DLLVM_INCLUDE_TOOLS=OFF  
  -DLLVM_INCLUDE_DOCS=OFF 
  -DLLVM_TARGETS_TO_BUILD=X86`  
  


* antlr 4.  
  Если у вас нет внешнего проекта antlr 4 nj проще всего 
  не делать ничего, во время генерации проекта cmake сам загрузит c помощью git проект antlr 4 
  нужной версии в папку external_projects и подключит к проекту.  

  Подключение внешнего проекта. Совместимость данного проекта проверена с версией antlr 4.10.1.
  Для уже имеющегося пакета (cmake-package)  antlr 4,
  местоположение которого можно задать с помощью аргумента командной строки:  
  `-DCMAKE_PREFIX_PATH="директория пакета"`.  
  Если скомпилированного пакета у вас нет, 
  то сначала вам нужно собрать antlr 4 в нужной для вас концигурации,
  >подсказка: при сборке antlr 4 используйте следующие аргументы cmake:  
  `-DANTLR_BUILD_CPP_TESTS=OFF`, `-DANTLR4_INSTALL=ON`, `-DWITH_STATIC_CRT=OFF`
  
   а затем установить пакет в подходящую для вас директорию:  
  `cmake --install "директория сборки" --prefix "директория пакета" `

* QT.  
  При генерации проекта укажите путь к пакету QT. 
  >пример: `-DCMAKE_PREFIX_PATH=C:/Qt/5.12.8/msvc2017_64`


#ifndef __linux__
#    ifdef DLL_express_ex_EXPORT
#        define DLL_EXPORT __declspec(dllexport)
#    else
#        define DLL_EXPORT
#    endif
#else
#    define DLL_EXPORT_
#endif



#ifdef EX_DEBUG
#    include <iostream>
#    define DEFINE_DCOUNTER(CLASSNAME) inline static int object__counter = 0;
#    define INCREMENT_DCOUNTER(CLASSNAME) \
        { object__counter++; }
#    define DECREMENT_DCOUNTER(CLASSNAME)                                               \
        {                                                                               \
            object__counter--;                                                          \
            std::cout << #CLASSNAME << ": " << object__counter << "\n";                 \
            if (object__counter == 0) std::cout << "all " << #CLASSNAME << "deleted\n"; \
        }
#else
#    define DEFINE_DCOUNTER(CLASSNAME)
#    define INCREMENT_DCOUNTER(CLASSNAME)
#    define DECREMENT_DCOUNTER(CLASSNAME)
#endif

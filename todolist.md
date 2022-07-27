# To do list

* add existence check for input files (filenames, data storage, etc.)
  and output check: filenames, data storage, etc.. Place this check before execution (maybe earlier).

## block structure

* intermediate_block
    * load_block
    * calc_block
    * store_block -> load_block | (next block)

## code simplification

Remove visitEnterStackUpdate from classes inherited from Operation_ifs and add corresponding directive to

## optimization

```
function rms(arg):
    function recurrence_relation( a , cond):
       increment = ( a * a )
       ## if increment has type less than double it will be transformed to target type twice? fnd both of them will be buffered
       c = recurrence_relation[-1] + increment
       res = cond ? increment : c
       return double( res )
    end

    cond = (arg[ 0] < 0) * (arg[-1] > 0)
    
    intermediate = recurrence_relation(arg, cond)**0.5
    
    return double(intermediate)
end
```

## issue

```
function rms(arg, delay):

    function recurrence_relation( a , cond):
       increment = ( a * a ) # if ( a[0] * a[0] ) replace with ( a* a) issue occur
       c = recurrence_relation[-1] + increment
       res = cond ? increment : c
       return double( res )
    end


    function get_num( a , cond, delay):
       c = get_num[-1] + 0 * a + 1
       res = (cond * (c<delay )) ? 1 : c # if you repalece this line  with  "res = cond * (c<delay ) ? 1 : c"  issue occur
       return double( res )
    end

    cond = (arg < 0) * (arg[-1] > 0)

    num = get_num(arg, cond, delay )

    intermediate = recurrence_relation(arg, num == 1)

    res = cond *(num[-1]) ? (intermediate[-1] / num[-1] )** 0.5 : rms[-1]

    return double(res)
end
```
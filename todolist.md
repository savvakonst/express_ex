# To do list

* add existence check for input files (filenames, data storage, etc.)
and output check: filenames, data storage, etc.. Place this check before execution (maybe earlier).

## block structure

* intermediate_block
    * load_block
    * calc_block
    * store_block -> load_block | (next block)

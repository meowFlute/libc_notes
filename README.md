# GNU `libc` Reference Manual Notes

For fun, I'm reading through the libc reference material in info and man. For retention I'll be making some demos of things as I learn about them.

## Source Files

The files in the src directory map as follows to the sections of the reference manual

Section 2: Error Reporting -> `src/libc_error_reporting_notes`
Section 3: Memory -> `src/libc_virtual_memory_allocation_and_paging_notes`

## Building Examples

On a machine with gcc and glib6 installed it should be as simple as navigating to the root directory of the repo and running `make`, though for extra reference I'm on Ubuntu 22.04 right now with the apt packages installed for those two. 

### NOTE: ctags / compiledb could kill your build
I use ctags as well as a json database of build dependencies and stuff for links within vim. If you don't, just remove the `post_build` stuff from the makefile and any reference to it or it'll likely make it ***think*** the build failed when in reality it just couldn't find executables it doesn't even need.

### Additional notes
Any extra notes are in the code comments, for example running the executable using

```shell
$ LD_PRELOAD=/lib/x86_64-linux-gnu/libc_malloc_debug.so MALLOC_TRACE=$HOME/libc_notes/malloc.trace ./libc_notes 
```

will activate and run the mtrace demo code, otherwise that will remain dormant. You'll find comments documenting this right above the mtrace call in `src/libc_virtual_memory_allocation_and_paging_notes.c`

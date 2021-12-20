#if [ "$(ldd HeborisC7EX-SDL2.exe | sed -ne 's/^.*\(\/mingw64\/bin\/.*\) (0x.*$/\1/p')" = '' ] ; then echo 'empty' ; else echo 'nonempty' ; fi

#if [ "$(ldd HeborisCE.exe | sed -ne 's/^.*\(\/mingw64\/bin\/.*\) (0x.*$/\1/p')" = '' ] ; then echo 'empty' ; else echo 'nonempty' ; fi

# TODO: Finish this for installation of DLLs of MinGW builds.
#install(CODE "
#message(STATUS \"Target file: $<TARGET_FILE:${EXE}>\")
#execute_process(COMMAND sh -c \"ldd $<TARGET_FILE:${EXE}> | sed -ne 's#^[ \\t][ \\t]*[^\\\\/][^\\\\/]*\\\\.[dD][lL][lL] => \\\\(..*\\\\.[dD][lL][lL]\\\\) (0x[0-9a-f][0-9a-f]*)$#\\\\1#p'\" OUTPUT_VARIABLE LIBRARIES_LINES)
#string(REPLACE \"\\n\" \";\" DEPENDENCIES \${LIBRARIES_LINES})
#foreach(DEPENDENCY IN LISTS DEPENDENCIES)
#	message(STATUS \${DEPENDENCY})
#endforeach()
#
#set_property(GLOBAL PROPERTY DEPTH 0)
#function(test_recurse)
#	get_property(DEPTH_LAST GLOBAL PROPERTY DEPTH)
#	math(EXPR DEPTH_CURRENT \"\${DEPTH_LAST} + 1\")
#	set_property(GLOBAL PROPERTY DEPTH \${DEPTH_CURRENT})
#	message(STATUS \"Depth at entry: \${DEPTH_CURRENT}\")
#	if(\${DEPTH_CURRENT} EQUAL 3)
#		return()
#	else()
#		test_recurse()
#		get_property(DEPTH_AFTER_RECURSE GLOBAL PROPERTY DEPTH)
#		message(STATUS \"Depth after recurse: \${DEPTH_AFTER_RECURSE}\")
#	endif()
#endfunction()
#
#test_recurse()
#message(STATUS \"Done\")
#")

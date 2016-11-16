# NAME

bin2c - convert file data to C byte array

# SYNOPSIS

bin2c [IN_FILE [OUT_FILE [VAR_NAME]]]

# DESCRIPTION

Writes out the C source code for a byte array representing the data in the
input file.

By default reads from STDIN, writes to STDOUT and names the variable
`resource_data`.

If IN_FILE is given but VAR_NAME is not, will use the name of IN_FILE converted
to a valid identifier.

# USING WITH CMAKE

You can use this repo as a submodule or just include the `bin2c.c` file in your sources.

Here is how you would add a rule to create an `.h` file from e.g. a compiled
resource file:

```cmake
# include the compiled header in your source list
SET(SOURCES program.cpp resources.h)

ADD_EXECUTABLE(program ${SOURCES})

# add the bin2c intermediate executable
ADD_EXECUTABLE(bin2c bin2c.c)
# if you use SDL you may need this
#SET_TARGET_PROPERTIES(bin2c PROPERTIES COMPILE_FLAGS "-Dmain=main")

# step one, compile your resources
ADD_CUSTOM_COMMAND(OUTPUT resources.xrs
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND wxrc ${CMAKE_CURRENT_SOURCE_DIR}/xrc/*.xrc -o resources.xrs
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xrc/*.xrc)

# step two, generate include file
ADD_CUSTOM_COMMAND(OUTPUT resources.h
                   COMMAND bin2c resources.xrs resources.h resource_data
                   DEPENDS bin2c resources.xrs)
```

# ACKNOWLEDGEMENTS

This program grew out of some work I am doing for the
[VisualBoyAdvance-M](http://github.com/visualboyadvance-m/visualboyadvance-m)
Game Boy Advance emulator project.

It replaces a bin2c.cmake script written in cmake that was very slow, I don't
know who the original author was.

# LICENSE

BSD 2-Clause

# CONTRIBUTING

Try to stick to C89 as much as possible please.

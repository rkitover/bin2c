#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>

void die(const char* fmt, ...) {
    char* buf = NULL;
    va_list args;
    va_start(args, fmt);
    vasprintf(&buf, fmt, args);
    va_end(args);
    perror(buf);
    exit(1);
}

void* usage(int exit_code) {
    fprintf(stderr,
"Usage: [32mbin2c [1;35mIN_FILE OUT_FILE VAR_NAME[0m\n"
"Write [1;35mIN_FILE[0m as a C array of bytes named [1;35mVAR_NAME[0m into [1;35mOUT_FILE[0m.\n"
"\n"
"All parameters are required.\n"
"\n"
"[1;35m-[0m can be used in place of [1;35mIN_FILE[0m for [1mSTDIN[0m or in place of [1;35mOUT_FILE[0m for [1mSTDOUT[0m.\n"
"\n"
"Example:\n"
"  # write data from file [1;35m./compiled-resources.xrs[0m into header file [1;35m./resources.h[0m using variable name [1;35mresource_data[0m\n"
"  [32mbin2c [1;35m./compiled-resources.xrs[0m [1;35m./resources.h[0m [1;35mresource_data[0m\n"
"\n"
"Project homepage: <[1;34mhttp://github.com/rkitover/bin2c[0m>\n"
    );

    exit(exit_code);

    return NULL;
}

int main(int argc, const char** argv) {
    const char* in_file_name  = argc == 4 ? argv[1] : usage(1);
    const char* out_file_name = argv[2];
    const char* var_name      = argv[3];
    int file_pos              = 0;
    size_t bytes_read         = 0;
    unsigned char* buf;
    FILE *in_file, *out_file;

    if (!(buf = malloc(4096)))
        die("Can't allocate memory");

    if (!(in_file  = strcmp(in_file_name, "-")  == 0 ?  stdin : fopen(in_file_name,  "rb")))
        die("Can't open input file '%s'", in_file_name);

    if (!(out_file = strcmp(out_file_name, "-") == 0 ? stdout : fopen(out_file_name, "wb")))
        die("Can't open '%s' for writing", out_file_name);

    fprintf(out_file, "/* generated from %s: do not edit */\n"
                      "const unsigned char %s[] = {\n", in_file_name, var_name);

    while ((bytes_read = fread(buf, 1, 4096, in_file))) {
        int i = 0;
        for (; i < bytes_read; i++) {
            char* comma = feof(in_file) && i == bytes_read - 1 ? "" : ",";

            fprintf(out_file, "0x%02x%s", buf[i], comma);

            if (++file_pos % 16 == 0) fputc('\n', out_file);
        }
    }

    fputs("};\n", out_file);

    free(buf);
    fclose(in_file);
    fclose(out_file);

    return 0;
}

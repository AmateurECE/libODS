///////////////////////////////////////////////////////////////////////////////
// NAME:            main.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Entrypoint for the test script.
//
// CREATED:         10/30/2021
//
// LAST EDITED:     11/02/2021
////

#include <stdio.h>

#include <libods/libods.h>

int main(int, char** argv) {
    printf("Opening %s\n", argv[1]);

    OdsFile file;
    ods_init_odsfile(&file);

    return ods_open_file(&file, argv[1]);
}

///////////////////////////////////////////////////////////////////////////////

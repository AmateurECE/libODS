///////////////////////////////////////////////////////////////////////////////
// NAME:            libods.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Primary interface to the libODS library
//
// CREATED:         10/30/2021
//
// LAST EDITED:     11/01/2021
////

#include <stdio.h>

#include <libods/libods.h>

static const char* LIBODS_LOG_PREFIX = "libods";

static void default_error_handler(const char* message, size_t) {
    fprintf(stderr, "%s: %s\n", LIBODS_LOG_PREFIX, message);
}

void ods_init_odsfile(OdsFile* file) {
    file->error = default_error_handler;
}

///////////////////////////////////////////////////////////////////////////////

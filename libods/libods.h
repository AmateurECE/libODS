///////////////////////////////////////////////////////////////////////////////
// NAME:            libods.h
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Interface for the libODS library.
//
// CREATED:         10/30/2021
//
// LAST EDITED:     11/02/2021
////

#ifndef EDT_LIBODS
#define EDT_LIBODS

#include <stddef.h>

typedef struct OdsFile {
    FILE* file;
    void (*error)(const char* message, size_t message_length);
} OdsFile;

typedef enum OdsResult {
    ODS_OK,
    ODS_ERROR,
} OdsResult;

void ods_init_odsfile(OdsFile* file);

OdsResult ods_open_file(OdsFile* file, const char* filename);

#endif // EDT_LIBODS

///////////////////////////////////////////////////////////////////////////////

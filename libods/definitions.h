///////////////////////////////////////////////////////////////////////////////
// NAME:            definitions.h
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Common definitions used library-wide (not exported)
//
// CREATED:         11/03/2021
//
// LAST EDITED:     11/03/2021
////

#ifndef EDT_DEFINITIONS
#define EDT_DEFINITIONS

#include <stddef.h>
#include <string.h>

#include <libods/libods.h>

#define ODS_ERROR_MESSAGE(error, msg) {         \
        error.file = __FILE__;                  \
        error.file_length = strlen(error.file); \
        error.line = __LINE__;                  \
        error.message = msg;                    \
        error.message_length = strlen(msg);     \
    }

#define ODS_ERRORFN(message_buffer, error_struct, file, msg) {          \
        ODS_ERROR_MESSAGE(error_struct, msg);                           \
        size_t message_length = put_error_message(message_buffer,       \
            ERROR_MESSAGE_LENGTH, &error_struct);                       \
        file->error(message_buffer, message_length);                    \
    }

static const size_t ERROR_MESSAGE_LENGTH = 256;

struct ErrorMessage {
    const char* file;
    size_t file_length;
    int line;
    const char* message;
    size_t message_length;
};

int put_error_message(char* destination, size_t destination_length,
    struct ErrorMessage* message);

OdsResult handle_archive_status(OdsFile* file, struct archive* stream,
    int result);

#endif // EDT_DEFINITIONS

///////////////////////////////////////////////////////////////////////////////

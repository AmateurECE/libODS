///////////////////////////////////////////////////////////////////////////////
// NAME:            mimetype.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Logic to parse mimetype entry
//
// CREATED:         11/03/2021
//
// LAST EDITED:     11/03/2021
////

#include <libods/compression/parsers.h>

OdsResult parse_mimetype(OdsFile* file, struct archive* reader,
    struct archive_entry*)
{
    char* buffer = NULL;
    size_t length = 0;
    off_t offset = 0;
    int result = archive_read_data_block(reader, (const void**)&buffer,
        &length, &offset);
    OdsResult status = handle_archive_status(file, reader, result);
    if (status != ODS_OK) {
        return status;
    }

    static const char* MIMETYPE =
        "application/vnd.oasis.opendocument.spreadsheet";
    if (strncmp(MIMETYPE, (const char*)&buffer[offset], length)) {
        char message_buffer[ERROR_MESSAGE_LENGTH];
        struct ErrorMessage error_message = {0};
        ODS_ERRORFN(message_buffer, error_message, file,
            "Incorrect mimetype!");
    }

    return ODS_OK;
}

///////////////////////////////////////////////////////////////////////////////

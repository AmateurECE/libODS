///////////////////////////////////////////////////////////////////////////////
// NAME:            compression.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Handles compression of the actual .ods
//
// CREATED:         11/01/2021
//
// LAST EDITED:     11/03/2021
////

#include <stdbool.h>
#include <string.h>

#include <archive.h>
#include <archive_entry.h>

#include <libods/libods.h>
#include <libods/definitions.h>
#include <libods/compression/parsers.h>

static const size_t CHUNK_SIZE = 10240;

int put_error_message(char* destination, size_t destination_length,
    struct ErrorMessage* message)
{
    size_t total_length = message->file_length
        + 1 // ":"
        + 5 // "99999" is the max
        + 2 // ": "
        + message->message_length
        + 2; // newline,NUL

    if (destination_length < total_length) {
        total_length = destination_length;
    }

    return snprintf(destination, total_length, "%s:%d: %s\n", message->file,
        message->line, message->message);
}

OdsResult handle_archive_status(OdsFile* file, struct archive* stream,
    int result)
{
    struct ErrorMessage error_message = {0};
    char message_buffer[ERROR_MESSAGE_LENGTH];

    if (result < ARCHIVE_OK) {
        const char* error_string = archive_error_string(stream);
        ODS_ERRORFN(message_buffer, error_message, file, error_string);
    }

    if (result < ARCHIVE_WARN) {
        return ODS_ERROR;
    }

    return ODS_OK;
}

static OdsResult parse_archive_entries(OdsFile* file, struct archive* reader,
    const struct EntryParser* entries)
{
    struct archive_entry* entry;
    bool consumed = false;
    while (archive_read_next_header(reader, &entry) == ARCHIVE_OK) {
        int index = 0;
        while (NULL != entries[index].filename) {
            if (0 == strcmp(entries[index].filename,
                    archive_entry_pathname(entry))) {
                consumed = true;
                OdsResult status = entries[index].parse_entry(file, reader,
                    entry);
                if (ODS_OK != status) {
                    return status;
                }
            }
            ++index;
        }

        if (!consumed) {
            archive_read_data_skip(reader);
        }
    }

    return ODS_OK;
}

static const struct EntryParser parsers[] = {
    { "mimetype", parse_mimetype },
    { 0 /* sentinel */ },
};

OdsResult ods_open_file(OdsFile* file, const char* filename) {
    struct archive* reader = archive_read_new();
    archive_read_support_filter_all(reader);
    archive_read_support_format_zip(reader);
    int result = archive_read_open_filename(reader, filename, CHUNK_SIZE);
    int status = handle_archive_status(file, reader, result);
    if (ODS_OK != status) {
        return status;
    }

    parse_archive_entries(file, reader, parsers);
    archive_read_close(reader);
    archive_read_free(reader);
    return ODS_OK;
}

///////////////////////////////////////////////////////////////////////////////

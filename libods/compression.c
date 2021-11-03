///////////////////////////////////////////////////////////////////////////////
// NAME:            compression.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Handles compression of the actual .ods
//
// CREATED:         11/01/2021
//
// LAST EDITED:     11/02/2021
////

#include <stdbool.h>
#include <string.h>

#include <archive.h>
#include <archive_entry.h>

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
static const size_t CHUNK_SIZE = 10240;

struct ErrorMessage {
    const char* file;
    size_t file_length;
    int line;
    const char* message;
    size_t message_length;
};

static int put_error_message(char* destination, size_t destination_length,
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

static OdsResult handle_archive_status(OdsFile* file, struct archive* stream,
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

struct EntryParser {
    const char* filename;
    OdsResult (*parse_entry)(OdsFile* file, struct archive* reader,
        struct archive_entry*);
};

static OdsResult parse_archive_entries(OdsFile* file, struct archive* reader,
    const struct EntryParser* entries, size_t number_entries)
{
    struct archive_entry* entry;
    bool consumed = false;
    while (archive_read_next_header(reader, &entry) == ARCHIVE_OK) {
        for (size_t index = 0; index < number_entries; ++index) {
            if (0 == strcmp(entries[index].filename,
                    archive_entry_pathname(entry))) {
                consumed = true;
                OdsResult status = entries[index].parse_entry(file, reader,
                    entry);
                if (ODS_OK != status) {
                    return status;
                }
            }
        }

        if (!consumed) {
            archive_read_data_skip(reader);
        }
    }

    return ODS_OK;
}

static OdsResult parse_mimetype(OdsFile* file, struct archive* reader,
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

static const struct EntryParser parsers[] = {
    { "mimetype", parse_mimetype },
};
static const size_t NUMBER_ENTRY_PARSERS = sizeof(parsers)
    / sizeof(struct EntryParser);

OdsResult ods_open_file(OdsFile* file, const char* filename) {
    struct archive* reader = archive_read_new();
    archive_read_support_filter_all(reader);
    archive_read_support_format_zip(reader);
    int result = archive_read_open_filename(reader, filename, CHUNK_SIZE);
    int status = handle_archive_status(file, reader, result);
    if (ODS_OK != status) {
        return status;
    }

    parse_archive_entries(file, reader, parsers, NUMBER_ENTRY_PARSERS);
    archive_read_close(reader);
    archive_read_free(reader);
    return ODS_OK;
}

///////////////////////////////////////////////////////////////////////////////

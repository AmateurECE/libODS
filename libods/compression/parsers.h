///////////////////////////////////////////////////////////////////////////////
// NAME:            parsers.h
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Archive entry parsers
//
// CREATED:         11/03/2021
//
// LAST EDITED:     11/03/2021
////

#ifndef EDT_PARSERS
#define EDT_PARSERS

#include <archive.h>
#include <archive_entry.h>

#include <libods/definitions.h>
#include <libods/libods.h>

struct EntryParser {
    const char* filename;
    OdsResult (*parse_entry)(OdsFile* file, struct archive* reader,
        struct archive_entry*);
};

OdsResult parse_mimetype(OdsFile* file, struct archive* reader,
    struct archive_entry*);

#endif // EDT_PARSERS

///////////////////////////////////////////////////////////////////////////////

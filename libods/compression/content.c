///////////////////////////////////////////////////////////////////////////////
// NAME:            content.c
//
// AUTHOR:          Ethan D. Twardy <ethan.twardy@gmail.com>
//
// DESCRIPTION:     Parse the "content.xml" entry of the .ods file.
//
// CREATED:         11/03/2021
//
// LAST EDITED:     11/03/2021
////

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <libods/compression/parsers.h>

// Bridge between libarchive and libxml2.
static int ods_xml_input_read_callback(void* context, char* buffer, int len) {
    struct archive* reader = (struct archive*)context;
    la_ssize_t bytes_read = archive_read_data(reader, buffer, len);
    return bytes_read < 0 ? -1 : bytes_read;
}

// No close actions required for libarchive
static int ods_xml_input_close_callback(void*) { return 0; }

static void print_element_names(xmlNode* node) {
    xmlNode* current = NULL;
    for (current = node; current; current = current->next) {
        if (current->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", current->name);
        }

        print_element_names(current->children);
    }
}

// TODO: Better error handling in here.
OdsResult parse_content(OdsFile* file, struct archive* reader,
    struct archive_entry* entry)
{
    LIBXML_TEST_VERSION
    xmlDoc* document = xmlReadIO(ods_xml_input_read_callback,
        ods_xml_input_close_callback, reader, NULL, NULL, 0);
    if (NULL == document) {
        return ODS_ERROR;
    }

    xmlNode* root_element = xmlDocGetRootElement(document);
    print_element_names(root_element);
    xmlFreeDoc(document);
    xmlCleanupParser();

    return ODS_OK;
}

///////////////////////////////////////////////////////////////////////////////

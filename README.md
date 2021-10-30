# libODS

A library for reading and writing OpenDocument Spreadsheet files.

# OpenDocument Spreadsheet Format

The `.ods` file is a zip archive, with the following interesting entries:

* `META-INF/manifest.xml` contains a list of the entries in the file
* `mimetype` contains the mimetype of the ods
  (application/vnd.oasis.opendocument.spreadsheet)
* `content.xml` contains the actual content of the document
* `styles.xml` contains information about cell styles
* `meta.xml` contains some statistics about the document

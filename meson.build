###############################################################################
# NAME:             meson.build
#
# AUTHOR:           Ethan D. Twardy <ethan.twardy@gmail.com>
#
# DESCRIPTION:      Build script for the project.
#
# CREATED:          10/30/2021
#
# LAST EDITED:      11/03/2021
###

project('libods', 'c')
libarchive = dependency('libarchive', version: '>=3.5.2')
libxml2 = dependency('libxml2')
ods = library(
  'ods',
  'libods/libods.c',
  'libods/compression.c',
  'libods/compression/mimetype.c',
  'libods/compression/content.c',
  dependencies: [libarchive, libxml2],
  c_args: ['-Wall', '-Wextra', '-O0', '-std=c17'],
  version: '0.1.0', soversion: '0',
)

executable(
  'testods',
  'test/main.c',
  c_args: ['-I.', '-Wall', '-Wextra', '-O0', '-g', '-std=c17'],
  link_with: [ods],
)

pkgconfig = import('pkgconfig')
pkgconfig.generate(ods, filebase: 'libods')

###############################################################################

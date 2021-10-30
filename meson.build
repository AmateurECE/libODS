###############################################################################
# NAME:             meson.build
#
# AUTHOR:           Ethan D. Twardy <ethan.twardy@gmail.com>
#
# DESCRIPTION:      Build script for the project.
#
# CREATED:          10/30/2021
#
# LAST EDITED:      10/30/2021
###

project('libods', 'c')
ods = library(
  'ods',
  'libods/libods.c',
  version: '0.1.0', soversion: '0',
)

executable(
  'testods',
  'test/main.c',
  c_args: ['-I.', '-Wall', '-Wextra', '-O0', '-g'],
  link_with: ods,
)

###############################################################################

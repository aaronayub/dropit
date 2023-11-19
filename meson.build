# Copyright (c) 2023 Aaron Ayub
# SPDX-License-Identifier: GPL-3.0-only

project('dropit', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

subdir('src')

exe = executable('dropit',
  sources,
  resources,
  dependencies : gtkdep,
  install : true)

test('basic', exe)

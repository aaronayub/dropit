# Copyright (c) 2023 Aaron Ayub
# SPDX-License-Identifier: GPL-3.0-only

project('gdrag', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

gtkdep = dependency('gtk4')
exe = executable('gdrag', 'gdrag.c',
  dependencies : gtkdep,
  install : true)

test('basic', exe)

# Copyright (c) 2023 Aaron Ayub
# SPDX-License-Identifier: GPL-3.0-only

project('gdrag', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

gtkdep = dependency('gtk4')

gnome = import('gnome')
resources = gnome.compile_resources('gdrag-resources','gdrag.gresource.xml')

exe = executable('gdrag', 'gdrag.c',
  resources,
  dependencies : gtkdep,
  install : true)

test('basic', exe)

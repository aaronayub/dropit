# Copyright (c) 2023 Aaron Ayub
# SPDX-License-Identifier: GPL-3.0-only

project('dropit', 'c',
  version : '1.0.0',
  license: 'GPL-3.0-only',
  default_options : ['warning_level=3'])

gtkdep = dependency('gtk4', version: '>=4.8.0')
gnome = import('gnome')

subdir('src')

executable('dropit',
  sources,
  resources,
  dependencies : gtkdep,
  install : true)

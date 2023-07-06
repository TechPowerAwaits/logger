project('logger', 'c', default_options: ['c_std=c89'], license: '0BSD', license_files: 'LICENSE.txt',
		meson_version: '>=1.1.0', version: '1.0.0')

sources = files([
	'logger.c',
])

header_dir = include_directories('include')

headers = files([
	'include/logger.h'
])

install_headers(headers)

logger_lib = library('logger', sources, include_directories: header_dir, install: true)

logger_dep = declare_dependency(include_directories: header_dir, link_with: logger_lib)
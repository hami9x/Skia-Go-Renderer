{
  'targets': [
    {
      'configurations': {
        'Debug': { },
        'Release': { }
      },
      'target_name': 'skia_go_renderer',
      'type': 'executable',
      'dependencies': [
        'third_party/skia/gyp/skia_lib.gyp:skia_lib'
      ],
      'include_dirs': [
        'third_party/skia/include/config',
        'third_party/skia/include/core',
      ],
      'sources': [
        'app/main.cpp'
      ],
      'ldflags': [
        '-std=c++11'
      ],
      'cflags': [
        '-Werror', '-W', '-Wall', '-Wextra', '-Wno-unused-parameter', '-g', '-O0'
      ]
    }
  ]
}
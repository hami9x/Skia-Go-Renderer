{
  'targets': [
    {
      'configurations': {
        'Debug': { },
        'Release': { }
      },
      'target_name': 'skia_go_renderer',
      'type': 'shared_library',
      'dependencies': [
        'third_party/skia/gyp/skia_lib.gyp:skia_lib'
      ],
      'include_dirs': [
        'third_party/skia/include/config',
        'third_party/skia/include/core',
      ],
      'sources': [
        'app/renderer.cpp'
      ],
      'ldflags': [
        '-std=c++11'
      ],
      'cflags': [
        '-Werror', '-W', '-Wall', '-Wextra', '-Wno-unused-parameter', '-g', '-O0'
      ]
    },
    {
      'configurations': {
        'Debug': { },
        'Release': { }
      },
      'target_name': 'test',
      'type': 'executable',
      'dependencies': [
        'third_party/skia/gyp/skia_lib.gyp:skia_lib'
      ],
      'include_dirs': [
        'third_party/skia/include/config',
        'third_party/skia/include/core',
      ],
      'sources': [
        'app/glfwSkia_test.cpp'
      ],
      'ldflags': [
        '-std=c++11'
      ],
      'cflags': [
        '-Werror', '-W', '-Wall', '-Wextra', '-Wno-unused-parameter', '-g', '-O0'
      ],
      'link_settings': {
        'libraries': [
          '-lglfw',
        ],
      },
    }
  ]
}
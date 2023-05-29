project('speaker_array_setup', 'cpp')

build_machine.system()
meson.get_compiler('cpp')

dep_yaml = dependency('yaml-cpp')

src = ['test_load_config.cpp', '../../modules/speakerarray.cpp']

speaker_array_h = include_directories('../../modules')

executable('run_test',
    sources: src,
    include_directories : speaker_array_h,
    dependencies: dep_yaml,
    override_options : ['cpp_std=c++11']
)


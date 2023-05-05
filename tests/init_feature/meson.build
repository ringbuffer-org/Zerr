project('init_feature', 'cpp')

build_machine.system()
meson.get_compiler('cpp')

test_src = ['test_init_feature.cpp', '../../modules/featureextractor.cpp']

modules_h = include_directories('../../modules')

subdir('features')

executable('run_test',
    sources: test_src + features_src,
    include_directories : [modules_h, features_h],
    override_options : ['cpp_std=c++11']
)

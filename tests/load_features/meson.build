project('load_features', 'cpp')

build_machine.system()
meson.get_compiler('cpp')

test_src = ['test_feature_bank.cpp', '../../modules/featurebank.cpp']

modules_h = include_directories('modules')
subdir('modules')
subdir('features')

executable('run_test',
    sources: test_src + features_src,
    include_directories : [modules_h, features_h],
    override_options : ['cpp_std=c++11']
)

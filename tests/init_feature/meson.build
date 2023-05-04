project('init_feature', 'cpp')

build_machine.system()
meson.get_compiler('cpp')

src = ['test_init_feature.cpp']

feature_bank_h = include_directories('../../modules')
features_h = include_directories('../../features')

executable('run_test',
    sources: src,
    include_directories : [feature_bank_h, features_h],
    override_options : ['cpp_std=c++11']
)

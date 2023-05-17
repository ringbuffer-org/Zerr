project('zerr', 'cpp')

test_src = ['main.cpp']
build_machine.system()

subdir('modules')
subdir('features')

dep_yaml = dependency('yaml-cpp')

executable('run_test',
    sources: test_src + features_src + modules_src,
    include_directories : [modules_h, features_h],
    dependencies: dep_yaml,
    override_options : ['cpp_std=c++11']
)

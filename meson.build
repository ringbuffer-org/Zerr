project('zerr', 'cpp')

main_src = ['zerr.cpp']
build_machine.system()

subdir('modules')
subdir('features')
subdir('utils')


dep_yaml = dependency('yaml-cpp')

executable('run_test',
    sources: main_src + modules_src + features_src + utils_src,
    include_directories : [modules_h, features_h, utils_h],
    dependencies: dep_yaml,
    override_options : ['cpp_std=c++11']
)

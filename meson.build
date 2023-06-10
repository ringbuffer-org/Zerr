project('zerr', 'cpp')
# build_machine.system()
main_src = [
  'main.cpp',
  'src/zerr.cpp',
]

utils_src = [
  'src/utils/ringbuffer.cpp',
  'src/utils/utils.cpp',
]

modules_src = [
  'src/modules/mapper.cpp',
  'src/modules/audiorouter.cpp',
  'src/modules/featurebank.cpp',
  'src/modules/speakerarray.cpp',
  'src/modules/featureextractor.cpp',
  'src/modules/trajectorygenerator.cpp'
]

features_src = [
  'src/features/centroid.cpp',
  'src/features/flux.cpp',
  'src/features/onset.cpp',
  'src/features/zerocrossing.cpp'
]

main_h     = include_directories('include')
utils_h    = include_directories('include/utils')
modules_h  = include_directories('include/modules')
features_h = include_directories('include/features')

dep_jack = dependency('jack')
dep_fftw = dependency('fftw3')
dep_yaml = dependency('yaml-cpp')

executable('run_zerr',
    sources: main_src + modules_src + features_src + utils_src,
    include_directories : [main_h, modules_h, features_h, utils_h],
    dependencies: [dep_jack, dep_fftw, dep_yaml],
    override_options : ['cpp_std=c++11']
)

project('SimpleCppCoroutines', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])



#incdir = include_directories('include')


# Jumps into the src/ subdir and executes the meson.build there
# after execution of meson.build is done, the line returns here
# with all the variables defined hoisted to here

subdir('src')




executable(
    'main1', 
    source_files1, # this variable is not defined in this meson.build but its hoisted from the subdir call above
    #include_directories: incdir,
    dependencies: dependency('threads')
)


executable(
    'main2', 
    source_files2, # this variable is not defined in this meson.build but its hoisted from the subdir call above
    #include_directories: incdir,
    dependencies: dependency('threads')
)
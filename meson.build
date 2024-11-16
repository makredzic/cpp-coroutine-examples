project('MesonExampleProject', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])



incdir = include_directories('include')


# Jumps into the src/ subdir and executes the meson.build there
# after execution of meson.build is done, the line returns here
# with all the variables defined hoisted to here

subdir('src')


exe = executable(
    'main', 
    source_files, # this variable is not defined in this meson.build but its hoisted from the subdir call above
    install : true, 
    include_directories: incdir
)


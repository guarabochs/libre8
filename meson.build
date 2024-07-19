project('8bitdo', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++26']
)

add_project_link_arguments('-lstdc++exp', '-lstdc++exp', language: 'cpp')

app_deps = [
  # app dependencies
  # <print> got implemented on GCC 14 so fmt is out
  dependency('hidapi-hidraw')
]

app_includes = [
  include_directories('include')
]

app_sources = [
  'src/app/app.cc',
  'src/libre8/device_manager_impl.cc',
  'src/libre8/pro2_controller_impl.cc'
]

app = library('app',
  app_sources,
  install: true,
  include_directories: [app_includes],
  dependencies: [app_deps]
)

exe = executable(
  'app',
  'src/main.cc',
  include_directories: [app_includes],
  dependencies: [app_deps],
  install: true,
  link_with: [app]
)

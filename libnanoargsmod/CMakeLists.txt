add_library(nanoargsmod)
add_library(nanoargsmod::nanoargsmod ALIAS nanoargsmod)

target_sources(nanoargsmod PUBLIC FILE_SET CXX_MODULES FILES nanoargsmod.cppm)

target_compile_features(nanoargsmod PUBLIC cxx_std_23)

target_compile_options(nanoargsmod PRIVATE -Wdocumentation)

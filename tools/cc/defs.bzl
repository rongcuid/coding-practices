def _with_standard_flag(copts, flag):
    if copts == None:
        copts = []
    return [flag] + copts

def c23_library(name, copts = None, **kwargs):
    native.cc_library(
        name = name,
        copts = _with_standard_flag(copts, "-std=c23"),
        **kwargs
    )

def c23_binary(name, copts = None, **kwargs):
    native.cc_binary(
        name = name,
        copts = _with_standard_flag(copts, "-std=c23"),
        **kwargs
    )

def cxx20_library(name, copts = None, **kwargs):
    native.cc_library(
        name = name,
        copts = _with_standard_flag(copts, "-std=c++20"),
        **kwargs
    )

def cxx20_binary(name, copts = None, **kwargs):
    native.cc_binary(
        name = name,
        copts = _with_standard_flag(copts, "-std=c++20"),
        **kwargs
    )

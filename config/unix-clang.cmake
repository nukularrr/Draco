# -------------------------------------------*-cmake-*-------------------------------------------- #
# file   config/unix-clang.cmake
# brief  Establish flags for Unix clang
# note   Copyright (C) 2015-2023 Triad National Security, LLC., All rights reserved.
# ------------------------------------------------------------------------------------------------ #

include_guard(GLOBAL)

# Note: In config/compilerEnv.cmake, the build system sets flags for
#
# 1. the language standard (C++14, C99, etc)
# 2. interprocedural optimization.

# Suggested flags:
#
# * http://clang.llvm.org/docs/UsersManual.html#options-to-control-error-and-warning-messages
#   -fdiagnostics-show-hotness
# * https://lefticus.gitbooks.io/cpp-best-practices/content/02-Use_the_Tools_Available.html
#
# valgrind like options, https://clang.llvm.org/docs/AddressSanitizer.html
#
# * '-g -fsanitize=address -fno-omit-frame-pointer'
# * must use clang++ for linking
# * suppressions: LSAN_OPTIONS=suppressions=MyLSan.supp
# * human readable: ASAN_SYMBOLIZER_PATH=/usr/local/bin/llvm-symbolizer ./a.out
#
# Sanitizers were attempted in Dec 2022 but none work correctly with our link time dependencies.
#
# * See https://re-git.lanl.gov/draco/devops/-/issues/107
# * These failed with undefined symbol errors: `-fsanitize=address`, `-fsanitize=thread`,
#   `-fsanitize=memory`, `-fsanitize=undefined`, and `-fsanitize=dataflow`.

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0 AND NOT MSVC)
  message(FATAL_ERROR "Draco requires LLVM clang version >= 6.0.")
endif()

#
# Compiler Flags
#
if(NOT CXX_FLAGS_INITIALIZED)
  set(CXX_FLAGS_INITIALIZED
      "yes"
      CACHE INTERNAL "using draco settings.")

  string(APPEND CMAKE_C_FLAGS " -g -Weverything")
  # now turn off some flags that produce too many warnings (we should work on these eventually!)
  string(
    APPEND
    CMAKE_C_FLAGS
    " -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation-unknown-command"
    " -Wno-exit-time-destructors -Wno-global-constructors -Wno-weak-vtables -Wno-old-style-cast"
    " -Wno-sign-conversion -Wno-padded -Wno-extra-semi-stmt -Wno-unreachable-code-break"
    " -Wno-unreachable-code-return -Wno-missing-prototypes -Wno-disabled-macro-expansion"
    " -Wno-switch-enum -Wno-deprecated-declarations -Wno-missing-noreturn -Wno-unreachable-code"
    " -Wno-documentation-deprecated-sync -Wno-documentation -Wno-undefined-func-template"
    " -Wno-weak-template-vtables -Wno-comma")

  if((NOT CMAKE_CXX_COMPILER_WRAPPER STREQUAL CrayPrgEnv)
     AND (NOT ${CMAKE_GENERATOR} MATCHES Xcode)
     AND HAS_MARCH_NATIVE)
    string(APPEND CMAKE_C_FLAGS " -march=native")
  endif()

  set(CMAKE_C_FLAGS_DEBUG "-fno-inline -O0 -DDEBUG")
  set(CMAKE_C_FLAGS_RELEASE "-O3 -funroll-loops -DNDEBUG")
  set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_RELEASE}")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -funroll-loops")

  # OneAPI on trinitite reports itself as "LLVM" and parses this file.  The Intel optimizer needs
  # these options to maintain IEEE 754 compliance.
  if(DEFINED CMAKE_CXX_COMPILER_WRAPPER
     AND CMAKE_CXX_COMPILER_WRAPPER STREQUAL CrayPrgEnv
     AND DEFINED ENV{INTEL_COMPILER_TYPE}
     AND "$ENV{INTEL_COMPILER_TYPE}" STREQUAL "ONEAPI")
    string(APPEND CMAKE_C_FLAGS " -fp-model=precise")
  endif()

  # Suppress warnings about typeid() called with function as an argument. In this case, the function
  # might not be called if the type can be deduced.
  string(APPEND CMAKE_CXX_FLAGS " ${CMAKE_C_FLAGS} -Wno-undefined-var-template"
         " -Wno-potentially-evaluated-expression")

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Woverloaded-virtual")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")

  if(DEFINED CMAKE_CXX_COMPILER_WRAPPER AND "${CMAKE_CXX_COMPILER_WRAPPER}" STREQUAL "CrayPrgEnv")
    string(APPEND CMAKE_CXX_FLAGS " -stdlib=libstdc++")
    # Work around for broken ftn + CC linking (Redmine #1323) that results in
    #
    # ld.lld: error: corrupt input file: version definition index 0 for symbol mpiprivc_ is out of
    # bounds
    #
    # As of 2021-08-10, this is required on Capulin/Thunder/Trinitite, but must be ommitted on
    # rznevada when using cce@12.
    if(EXISTS /usr/gapps)
      # ATS-2: no-op
    else()
      # trinitite/trinity/capulin/thunder
      string(APPEND CMAKE_EXE_LINKER_FLAGS " -fuse-ld=bfd")
    endif()
  elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.0.0)
    # \note When building with LLVM-13 on WSL2 or Linux, I don't need the '-stdlib' flag.  I'm not
    # sure if the difference is WSL2 or newer llvm or something else.  For now, assume that we only
    # need this flag for older versions of llvm.
    string(APPEND CMAKE_CXX_FLAGS " -stdlib=libc++")
  else()
    string(APPEND CMAKE_CXX_FLAGS " -pthread")
  endif()

endif()

# ------------------------------------------------------------------------------------------------ #
# Toggle for OpenMP support
if(OpenMP_C_FLAGS)
  toggle_compiler_flag(OpenMP_FOUND "${OpenMP_C_FLAGS}" "C" "")
endif()
if(OpenMP_CXX_FLAGS)
  toggle_compiler_flag(OpenMP_FOUND "${OpenMP_CXX_FLAGS}" "CXX" "")
endif()
# Note: adding openmp option to EXE_LINKER will break MPI detection for gfortran when running with
# clang++/clang/gfortran.

# ------------------------------------------------------------------------------------------------ #
# Ensure cache values always match current selection
deduplicate_flags(CMAKE_C_FLAGS)
deduplicate_flags(CMAKE_CXX_FLAGS)
force_compiler_flags_to_cache("C;CXX;EXE_LINKER")

# ------------------------------------------------------------------------------------------------ #
# End config/unix-clang.cmake
# ------------------------------------------------------------------------------------------------ #

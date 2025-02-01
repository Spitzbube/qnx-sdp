#
# Copyright 2017-2019, QNX Software Systems.  All Rights Reserved.
#
# You must obtain a written license from and pay applicable license fees to QNX
# Software Systems before you may reproduce, modify or distribute this software,
# or any work that includes all or part of this software.  Free development
# licenses are available for evaluation and non-commercial purposes.  For more
# information visit http://licensing.qnx.com or email licensing@qnx.com.
#
# This file may contain contributions from others.  Please review this entire
# file for other proprietary rights or license notices, as well as the QNX
# Development Suite License Guide at http://licensing.qnx.com/license-guide/
# for other information.
#

#
# This is a QNX helper makefile that deals with:
# - some optimization flags
# - protection flags
# - warnings and errors
# - linked libraries
#
# Projects making use of this makefile define the libraries to be included and get a default set of
# options that apply to their project type (archive, shared object, dll, executable).  Variables can
# be defined in order to opt out or opt in of certain aspects of the makefile.  Note that prior to
# including qh_makeflags.mk, $(MKFILES_ROOT)/qmacros.mk must first be included.
#
# The set of controlling variables and their effect follows.
#
# QH_MF_STATIC_LIBS (space separated list of static library names):
#     The defined list of static libraries is automatically controlled with static and dynamic
#     linker flags.  Note that the non-S variant needs to be used when linking into executables and
#     non-S static libraries, and the S variant needs to be used otherwise.
#     These are added to the LIBS variable.
#     Note that Darwin's linker doesn't support the -Bstatic and -Bdynamic options, and so any
#     library in this list will simply be silently rolled in the QH_MF_LIBS list.
#
# QH_MF_LIBS (space separated list of non-static library names):
#     The defined list of non-static libraries to be linked with.  These are added to the LIBS
#     variable, prior to the QH_MF_STATIC_LIBS list.
#
# QH_MF_OPTIMIZE (0, 1, 2, 3, s or f):
#     Enables the -Ox flag, where x is as defined by QH_MF_OPTIMIZE.  When not specified, 's' is
#     assumed.
#
# QH_MF_NO_OPTIMIZE:
#     Disables setting of optimization flags (default or explicit).
#
# QH_MF_NO_OPTIMIZE_LTO:
#     Disables setting of link time optimization flags implicitely used when optimizations are used.
#
# QH_MF_NO_PROTECT:
#     Disables setting of stack protection and FORTIFY_SOURCE flags.
#
# QH_MF_NO_EXE_PIE:
#     Disables setting of PIE flags for an executable.  This is meant to be used for executables
#     only, and is ignored otherwise.
#
# QH_MF_EXE_PIE:
#     Enables setting of PIE flags for an executable.  This flag is automatic when building for NTO,
#     and is superceded by QH_MF_NO_EXE_PIE (when defined).  This is meant to be used for
#     executables only, and is ignored otherwise.
#
# QH_MF_NO_RELRO:
#     Disables setting of read-only relocation flags (full, combined with "now").
#
# QH_MF_WARNINGS_AS_ERRORS:
#     Enables setting of flag that treats all compile warnings as errors.
#
# QH_MF_WARNINGS_NO_DEFAULTS:
#     Disables setting of all default warning flags.
#
# QH_MF_NO_LIB_UNDEFINED_SYMBOLS:
#     Disables setting of undefined symbols in object files linker warning flags enabled in the V1
#     set of warnings.  This is meant to be used for libraries only (archive, shared object, dll).
#     Superseded by QH_MF_NO_WARNINGS and QH_MF_WARNINGS_UPTO_BASIC.
#
# QH_MF_WARNINGS_UNEXPECTED_CONDITIONS:
#     Enables the set of warnings related to possible unexpected conditions.
#
# QH_MF_WARNINGS_STRICT:
#     Enables the set of warnings related to strict code writing rules.
#
# QH_MF_WARNINGS_API_IMPACTING:
#     Enables a set of warnings that can possibly have an impact on public APIs.  Developers using
#     these warning flags need to be aware of the possible consequences of making suggested changes
#     in public APIs.  If the implications of making code changes in public APIs based on warnings
#     resulting from these flags are not very well understood by the developer, it is strongly
#     recommended the changes not be made given the possible negative side effects of making the
#     changes.  Under these conditions, it is better to simply silence the warnings instead of
#     making public API code changes.
#     As an example, if the __const__ attribute is added to a function of a public API as might be
#     suggested by -Wsuggest-attribute=const, but that the attribute is later removed because of a
#     change in implementation (without changing the rest of the API), then there might be cases
#     where the function may end up not always being called if the client code is not recompiled,
#     thus breaking backwards compatibility.  In this case, the attribute should not have been added
#     in the first place.
#
# Regarding warnings and errors, projects that want most but not all warnings to be used can still
# use this makefile, and once they include it, they can then specifically add CCFLAGS that remove
# the warning (their -Wno- counterpart).
#
# Here is a description of what the makefile provides by default:
#
# Optimization flag, controlled by QH_MF_NO_OPTIMIZE and QH_MF_OPTIMIZE:
# CCFLAGS += -Os
#
# Link time optimization flags, controlled by QH_MF_NO_OPTIMIZE, QH_MF_OPTIMIZE and
# QH_MF_NO_OPTIMIZE_LTO:
# CCFLAGS += -flto -ffat-lto-objects
# LDFLAGS += -flto -Os
#
# Stack protection flags, controlled by QH_MF_NO_PROTECT:
# CCFLAGS += -D_FORTIFY_SOURCE=2
# CCFLAGS += -fstack-protector-strong (never enabled for win64)
#
# PIE flags, controlled by QH_MF_NO_EXE_PIE and QH_MF_EXE_PIE, and only applicable for executables:
# CCFLAGS += -fpie (never enabled for win64)
# LDFLAGS += -pie -fpie (never enabled for win64)
#
# Read-only relocation (full, combined with "now"), controlled by QH_MF_NO_RELRO:
# LDFLAGS += -Wl,-z,relro (never enabled for darwin and win64)
# LDFLAGS += -Wl,-z,now (never enabled for darwin and win64)
#
# Libraries to be included, controlled by QH_MF_STATIC_LIBS and QH_MF_LIBS:
# LIBS += $(QH_MF_LIBS) $(QH_MF_STATIC_LIBS)
# ...where QH_MF_STATIC_LIBS are automatically properly set as static libraries in the link options.
# See the note above rerarding the lack of support for QH_MF_STATIC_LIBS on Darwin.
#
# Default warning flags, controlled by QH_MF_WARNINGS_NO_DEFAULTS:
# CCFLAGS += -Wall
# CCFLAGS += -Wextra
#
# CCFLAGS += -Waggregate-return
# CCFLAGS += -Wcast-align
# CCFLAGS += -Wcast-qual
# CCFLAGS += -Wconversion
# CCFLAGS += -Wfloat-equal
# CCFLAGS += -Wformat-signedness
# CCFLAGS += -Wformat=2
# CCFLAGS += -Winit-self
# CCFLAGS += -Wlogical-op
# CCFLAGS += -Wmissing-declarations
# CCFLAGS += -Wmissing-prototypes
# CCFLAGS += -Wnested-externs
# CCFLAGS += -Wold-style-definition
# CCFLAGS += -Woverlength-strings
# CCFLAGS += -Wpointer-arith
# CCFLAGS += -Wshadow
# CCFLAGS += -Wstrict-prototypes
# CCFLAGS += -Wswitch-default
# CCFLAGS += -Wswitch-enum
# CCFLAGS += -Wtrampolines
# CCFLAGS += -Wunreachable-code
# CCFLAGS += -Wunsafe-loop-optimizations
# CCFLAGS += -Wwrite-strings
#
# Default warning flags specific to libraries, controlled by QH_MF_WARNINGS_NO_DEFAULTS and
# QH_MF_NO_LIB_UNDEFINED_SYMBOLS, and only applicable for libraries (archive, shared object, dll):
# LDFLAGS += -Wl,-z,defs (never enabled for darwin and win64)
#
#
# Additional sets of warnings flags can be included given the appropriate control flags, but are not
# enabled by default.
#
# Treat all warnings as errors, controlled by QH_MF_WARNINGS_AS_ERRORS:
# CCFLAGS += -Werror
#
# Warning flags related to possibly unexpected conditions, controlled by
# QH_MF_WARNINGS_UNEXPECTED_CONDITIONS:
# CCFLAGS += -Wdisabled-optimization
# CCFLAGS += -Wdouble-promotion
# CCFLAGS += -Winvalid-pch
# CCFLAGS += -Wjump-misses-init
# CCFLAGS += -Wmissing-include-dirs
# CCFLAGS += -Wstack-protector
# CCFLAGS += -Wundef
#
# Warning flags related to strict code writing rules, controlled by QH_MF_WARNINGS_STRICT:
# CCFLAGS += -Warray-bounds=2
# CCFLAGS += -Wbad-function-cast
# CCFLAGS += -Wdeclaration-after-statement
# CCFLAGS += -Wpedantic
# CCFLAGS += -pedantic-errors
# CCFLAGS += -Wredundant-decls
# CCFLAGS += -Wstrict-overflow=5
# CCFLAGS += -Wsuggest-attribute=format
# CCFLAGS += -Wunsuffixed-float-constants
# CCFLAGS += -Wvariadic-macros
# CCFLAGS += -Wvla
#
# Warning flags that can potentially impact public APIs (see earlier description), controlled by
# QH_MF_WARNINGS_API_IMPACTING:
# CCFLAGS += -Wsuggest-attribute=const
# CCFLAGS += -Wsuggest-attribute=noreturn
# CCFLAGS += -Wsuggest-attribute=pure
#
#
# Note that the -Winline flag is not being used in any context in qh_makeflags.  This warning merely
# mentions a function was not inlined even if the __inline__ function attribute was specified, but
# this is just fine given it's really only a hint for the compiler to use.  If the compiler
# determines it's more efficient not to inline the function, it is still not a problem.  Turning on
# this warning flag would simply encourage developers to use __always_inline__ instead, which would
# lead to suboptimal code in some cases.  If developers really need to use the -Winline warning
# flag, they can specify it directly in their makefile.
#
#
# Example usage for a binary executable where lto and pie are not desired and where only basic
# warnings are desired:
#
#    include $(MKFILES_ROOT)/qmacros.mk
#    ...
#    QH_MF_NO_OPTIMIZE_LTO = 1
#    QH_MF_NO_EXE_PIE = 1
#    QH_MF_WARNINGS_AS_ERRORS = 1
#
#    QH_MF_STATIC_LIBS = my_staticS
#    QH_MF_LIBS += my_non_static
#
#    include $(MKFILES_ROOT)/qh_makeflags.mk
#
#    # if the project doesn't want the old-style-definition warnings
#    CCFLAGS += -Wno-old-style-definition
#
# Example usage for a library where all warning sets are desired, and where all warnings would be
# treated as errors:
#
#    include $(MKFILES_ROOT)/qmacros.mk
#    ...
#    QH_MF_WARNINGS_UNEXPECTED_CONDITIONS = 1
#    QH_MF_WARNINGS_STRICT = 1
#    QH_MF_WARNINGS_API_IMPACTING = 1
#    QH_MF_WARNINGS_AS_ERRORS = 1
#
#    QH_MF_STATIC_LIBS = my_staticS
#    QH_MF_LIBS += my_non_static
#
#    include $(MKFILES_ROOT)/qh_makeflags.mk
#
#

ifndef _MK_QH_MAKEFLAGS_MK_INCLUDED
_MK_QH_MAKEFLAGS_MK_INCLUDED = 1

################## BUILD_TYPE start

ifeq ($(BUILD_TYPE),)
$(error $$(MKFILES_ROOT)/qmacros.mk must be included before qh_makeflags.mk)
endif

ifeq ($(BUILD_TYPE), SO)
QH_MF_TYPE_LIB_ = 1
else
ifeq ($(BUILD_TYPE), DLL)
QH_MF_TYPE_DLL_ = 1
else
ifeq ($(BUILD_TYPE), EX)
QH_MF_TYPE_EXE_ = 1
else
ifeq ($(BUILD_TYPE), AR)
QH_MF_TYPE_AR_ = 1
endif # ar
endif # exe
endif # dll
endif # lib

ifeq ($(QH_MF_TYPE_LIB_)$(QH_MF_TYPE_DLL_)$(QH_MF_TYPE_AR_)$(QH_MF_TYPE_EXE_),)
$(warning BUILD_TYPE is not one of 'AR', 'SO', 'DLL' or 'EX')
endif

################## BUILD_TYPE end

################## QH_MF_NO_OPTIMIZE start

ifndef QH_MF_NO_OPTIMIZE

ifndef QH_MF_OPTIMIZE
QH_MF_OPTIMIZE_ = s
else
$(eval QH_MF_EXTRAS_ := $$$(QH_MF_OPTIMIZE))
ifneq ($(QH_MF_EXTRAS_),)
$(error QH_MF_OPTIMIZE must be defined as 0, 1, 2, 3, s or f, and was '$(QH_MF_OPTIMIZE)' (too long))
endif
ifeq ($(findstring $(QH_MF_OPTIMIZE), 0123sf),)
$(error QH_MF_OPTIMIZE must be defined as 0, 1, 2, 3, s or f, and was '$(QH_MF_OPTIMIZE)')
endif
QH_MF_OPTIMIZE_ = $(QH_MF_OPTIMIZE)
endif # QH_MF_OPTIMIZE

# Optimization flags
CCFLAGS += -O$(QH_MF_OPTIMIZE_)
ifndef QH_MF_NO_OPTIMIZE_LTO
QH_MF_CCFLAGS_gcc_ += -flto -ffat-lto-objects
QH_MF_LDFLAGS_gcc_ += -flto -O$(QH_MF_OPTIMIZE_)
CCFLAGS += $(QH_MF_CCFLAGS_$(COMPILER_TYPE)_)
LDFLAGS += $(QH_MF_LDFLAGS_$(COMPILER_TYPE)_)
QH_MF_CCFLAGS_gcc_ =
QH_MF_LDFLAGS_gcc_ =
endif # QH_MF_NO_OPTIMIZE_LTO

endif # QH_MF_NO_OPTIMIZE

################## QH_MF_NO_OPTIMIZE end

################## QH_MF_NO_PROTECT start

ifndef QH_MF_NO_PROTECT
# Protection and security flags
CCFLAGS += -D_FORTIFY_SOURCE=2
# win64 does not support stack protectors
ifeq ($(filter win64, $(OS)),)
CCFLAGS += -fstack-protector-strong
endif # OS check
endif # QH_MF_NO_PROTECT

################## QH_MF_NO_PROTECT end

################## type specific start

ifdef QH_MF_TYPE_EXE_
ifndef QH_MF_NO_EXE_PIE
# prevent pie options for win64
ifeq ($(filter win64, $(OS)), win64)
QH_MF_NO_EXE_PIE := 1
endif
# turn on automatic PIE for executables on NTO
ifeq ($(filter nto, $(OS)), nto)
QH_MF_EXE_PIE := 1
endif # nto automatic PIE

ifdef QH_MF_EXE_PIE
CCFLAGS += -fpie
LDFLAGS += -pie -fpie
endif # QH_MF_EXE_PIE
endif # QH_MF_NO_EXE_PIE
endif # QH_MF_TYPE_EXE_

################## type specific end

################## QH_MF_NO_RELRO start

ifndef QH_MF_NO_RELRO
# darwin & win64 do not support -z
ifeq ($(filter darwin win64, $(OS)),)
# Read-only relocation (full, combined with "now")
LDFLAGS += -Wl,-z,relro
LDFLAGS += -Wl,-z,now
endif # OS check
endif # QH_MF_NO_RELRO

################## QH_MF_NO_RELRO end

################## QH_MF_WARNINGS_AS_ERRORS start

ifdef QH_MF_WARNINGS_AS_ERRORS
# Don't set -Werror when it's disallowed.
ifneq ($(QNX_ALLOW_WERROR), 0)
CCFLAGS += -Werror
endif
endif

################## QH_MF_WARNINGS_AS_ERRORS end

################## QH_MF_WARNINGS_NO_DEFAULTS start

ifndef QH_MF_WARNINGS_NO_DEFAULTS

ifneq ($(QH_MF_TYPE_LIB_)$(QH_MF_TYPE_DLL_)$(QH_MF_TYPE_AR_),)
ifndef QH_MF_NO_LIB_UNDEFINED_SYMBOLS
# darwin & win64 do not support -z
ifeq ($(filter darwin win64, $(OS)),)
# Prevent undefined symbols in object files
LDFLAGS += -Wl,-z,defs
endif # OS check
endif # !QH_MF_NO_LIB_UNDEFINED_SYMBOLS
endif # LIB & DLL

CCFLAGS += -Wall
CCFLAGS += -Wextra

CCFLAGS += -Waggregate-return
CCFLAGS += -Wcast-align
CCFLAGS += -Wcast-qual
CCFLAGS += -Wconversion
CCFLAGS += -Wfloat-equal
CCFLAGS += -Wformat-signedness
CCFLAGS += -Wformat=2
CCFLAGS += -Winit-self
CCFLAGS += -Wlogical-op
CCFLAGS += -Wmissing-declarations
CCFLAGS += -Wmissing-prototypes
CCFLAGS += -Wnested-externs
CCFLAGS += -Wold-style-definition
CCFLAGS += -Woverlength-strings
CCFLAGS += -Wpointer-arith
CCFLAGS += -Wshadow
CCFLAGS += -Wstrict-prototypes
CCFLAGS += -Wswitch-default
CCFLAGS += -Wswitch-enum
CCFLAGS += -Wtrampolines
CCFLAGS += -Wunreachable-code
CCFLAGS += -Wunsafe-loop-optimizations
CCFLAGS += -Wwrite-strings

endif # !QH_MF_WARNINGS_NO_DEFAULTS

################## QH_MF_WARNINGS_UNEXPECTED_CONDITIONS start

ifdef QH_MF_WARNINGS_UNEXPECTED_CONDITIONS
CCFLAGS += -Wdisabled-optimization
CCFLAGS += -Wdouble-promotion
CCFLAGS += -Winvalid-pch
CCFLAGS += -Wjump-misses-init
CCFLAGS += -Wstack-protector
CCFLAGS += -Wundef

ifdef QH_MF_WARNINGS_AS_ERRORS
ifeq ($(filter iclean clean spotless uninstall qinstall quninstall hinstall huninstall showvars pinfo_failure, $(MAKECMDGOALS)),)
ifneq ($(INSTALL_ROOT_$(OS)), $(USE_ROOT_$(OS)))
ifeq ("$(wildcard $(INSTALL_ROOT_HDR)/.*)","")
QH_MF_NO_STAGE_ = 1
endif
endif
endif
endif

ifeq ($(QH_MF_NO_STAGE_),)
CCFLAGS += -Wmissing-include-dirs
else
$(warning warning: Stage folder ($(INSTALL_ROOT_HDR)) does not exist.  Bypassing -Wmissing-include-folders.)
endif
endif

################## QH_MF_WARNINGS_UNEXPECTED_CONDITIONS end

################## QH_MF_WARNINGS_STRICT start

ifdef QH_MF_WARNINGS_STRICT
CCFLAGS += -Warray-bounds=2
CCFLAGS += -Wbad-function-cast
CCFLAGS += -Wdeclaration-after-statement
CCFLAGS += -Wpedantic
CCFLAGS += -pedantic-errors
CCFLAGS += -Wredundant-decls
CCFLAGS += -Wstrict-overflow=5
CCFLAGS += -Wsuggest-attribute=format
CCFLAGS += -Wunsuffixed-float-constants
CCFLAGS += -Wvariadic-macros
CCFLAGS += -Wvla
endif

################## QH_MF_WARNINGS_STRICT end

################## QH_MF_WARNINGS_API_IMPACTING start

ifdef QH_MF_WARNINGS_API_IMPACTING
CCFLAGS += -Wsuggest-attribute=const
CCFLAGS += -Wsuggest-attribute=noreturn
CCFLAGS += -Wsuggest-attribute=pure
endif

################## QH_MF_WARNINGS_API_IMPACTING end

################## QH_MF_WARNINGS_NO_DEFAULTS end

################## STATIC_LIBS & LIBS start

ifneq ($(QH_MF_STATIC_LIBS)$(QH_MF_LIBS),)

ifdef QH_MF_STATIC_LIBS
ifeq ($(filter darwin, $(OS)),)
$(foreach lib, $(QH_MF_STATIC_LIBS), $(eval LIBPREF_$(lib) = -Wl,-Bstatic))
$(foreach lib, $(QH_MF_STATIC_LIBS), $(eval LIBPOST_$(lib) = -Wl,-Bdynamic))
endif # OS check
endif # QH_MF_STATIC_LIBS

LIBS += $(QH_MF_LIBS) $(QH_MF_STATIC_LIBS)

endif # QH_MF_STATIC_LIBS && QH_MF_LIBS

################## STATIC_LIBS & LIBS end

endif # _MK_QH_MAKEFLAGS_MK_INCLUDED

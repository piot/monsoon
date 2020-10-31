# First found opus include directories
find_path(OPUS_INCLUDE_DIRS
	NAMES opus/opus.h
	PATH_SUFFIXES include
)
if(OPUS_INCLUDE_DIRS)
	set(FOUND_OPUS_INCLUDE_DIRS 1)
endif()


# Found Library
find_library(OPUS_LIBRARIES NAMES opus)
if(OPUS_LIBRARIES)
	find_library(FOUND_LIBRARY NAMES m)
	if(FOUND_LIBRARY)
		list(APPEND OPUS_LIBRARIES ${FOUND_LIBRARY})
	endif()
endif()

# Look for the opusfile header file.
find_path(OPUSFILE_INCLUDE_DIR
  NAMES opus/opusfile.h
  DOC "Opus file include directory")
mark_as_advanced( OPUSFILE_INCLUDE_DIR )
if(OPUSFILE_INCLUDE_DIR)
	set(FOUND_OPUSFILE_INCLUDE_DIRS 1)
endif()

find_library(OPUSFILE_LIBRARY
  NAMES opusfile
  DOC "Opus file library")
mark_as_advanced( OPUSFILE_LIBRARY )

list(APPEND OPUS_LIBRARIES ${OPUSFILE_LIBRARY})
list(APPEND FOUND_OPUS_INCLUDE_DIRS ${OPUSFILE_INCLUDE_DIR})

# Do a normal package find and setting variables
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Opus
	DEFAULT_MSG
	OPUS_INCLUDE_DIRS OPUS_LIBRARIES FOUND_OPUS_INCLUDE_DIRS
)

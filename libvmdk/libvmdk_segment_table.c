/*
 * Segment table functions
 *
 * Copyright (c) 2009-2012 Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libvmdk_definitions.h"
#include "libvmdk_libcdata.h"
#include "libvmdk_libcerror.h"
#include "libvmdk_libcnotify.h"
#include "libvmdk_libcstring.h"
#include "libvmdk_libuna.h"
#include "libvmdk_segment_file_handle.h"
#include "libvmdk_segment_table.h"

/* Initialize the segment table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_initialize(
     libvmdk_segment_table_t **segment_table,
     int number_of_handles,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_initialize";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( *segment_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid segment table value already set.",
		 function );

		return( -1 );
	}
	if( number_of_handles <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of handles value cannot be zero or less.",
		 function );

		return( -1 );
	}
	*segment_table = memory_allocate_structure(
	                  libvmdk_segment_table_t );

	if( *segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create segment table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *segment_table,
	     0,
	     sizeof( libvmdk_segment_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear segment table.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *segment_table )->segment_file_handle_array ),
	     number_of_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment file handle array.",
		 function );

		goto on_error;
	}
	( *segment_table )->maximum_segment_size = maximum_segment_size;

	return( 1 );

on_error:
	if( *segment_table != NULL )
	{
		memory_free(
		 *segment_table );

		*segment_table = NULL;
	}
	return( -1 );
}

/* Frees the segment table including elements
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_free(
     libvmdk_segment_table_t **segment_table,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_free";
	int result            = 1;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( *segment_table != NULL )
	{
		if( ( *segment_table )->basename != NULL )
		{
			memory_free(
			 ( *segment_table )->basename );
		}
		if( libcdata_array_free(
		     &( ( *segment_table )->segment_file_handle_array ),
		     &libvmdk_segment_file_handle_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the table index array.",
			 function );

			result = -1;
		}
		memory_free(
		 *segment_table );

		*segment_table = NULL;
	}
	return( result );
}

/* Resizes the segment table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_resize(
     libvmdk_segment_table_t *segment_table,
     int number_of_handles,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_resize";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( number_of_handles <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of handles value cannot be zero or less.",
		 function );

		return( -1 );
	}
	if( libcdata_array_resize(
	     segment_table->segment_file_handle_array,
	     number_of_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize segment file handle array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_segment_table_get_basename_size(
     libvmdk_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_get_basename_size";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#else
	*basename_size = segment_table->basename_size;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

/* Retrieves the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_segment_table_get_basename(
     libvmdk_segment_table_t *segment_table,
     char *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function       = "libvmdk_segment_table_get_basename";
	size_t narrow_basename_size = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                  = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          narrow_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          narrow_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          narrow_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          narrow_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow basename size.",
		 function );

		return( -1 );
	}
#else
	narrow_basename_size = segment_table->basename_size;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( basename_size < narrow_basename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) basename,
		          basename_size,
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) basename,
		          basename_size,
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) basename,
		          basename_size,
		          libcstring_narrow_system_string_codepage,
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) basename,
		          basename_size,
		          libcstring_narrow_system_string_codepage,
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#else
	if( libcstring_system_string_copy(
	     basename,
	     segment_table->basename,
	     segment_table->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ segment_table->basename_size - 1 ] = 0;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

/* Sets the basename in the segment table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_set_basename(
     libvmdk_segment_table_t *segment_table,
     const char *basename,
     size_t basename_length,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_set_basename";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) basename,
		          basename_length + 1,
		          libcstring_narrow_system_string_codepage,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) basename,
		          basename_length + 1,
		          libcstring_narrow_system_string_codepage,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		goto on_error;
	}
#else
	segment_table->basename_size = basename_length + 1;
#endif
	segment_table->basename = libcstring_system_string_allocate(
	                           segment_table->basename_size );

	if( segment_table->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf8_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (uint8_t *) basename,
		          basename_length + 1,
		          libcstring_narrow_system_string_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (uint8_t *) basename,
		          basename_length + 1,
		          libcstring_narrow_system_string_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		goto on_error;
	}
#else
	if( libcstring_system_string_copy(
	     segment_table->basename,
	     basename,
	     basename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		goto on_error;
	}
	segment_table->basename[ basename_length ] = 0;
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );

on_error:
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename = NULL;
	}
	segment_table->basename_size = 0;

	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )
/* Retrieves the size of the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_segment_table_get_basename_size_wide(
     libvmdk_segment_table_t *segment_table,
     size_t *basename_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_get_basename_size_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename size.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_WIDE_SYSTEM_CHARACTER_TYPE )
	*basename_size = segment_table->basename_size;
#else
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}

/* Retrieves the basename in the segment table
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libvmdk_segment_table_get_basename_wide(
     libvmdk_segment_table_t *segment_table,
     wchar_t *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function     = "libvmdk_segment_table_get_basename_wide";
	size_t wide_basename_size = 0;

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename == NULL )
	{
		return( 0 );
	}
#if defined( LIBCSTRING_WIDE_SYSTEM_CHARACTER_TYPE )
	wide_basename_size = segment_table->basename_size;
#else
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          &wide_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          &wide_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          &wide_basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          &wide_basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	if( basename_size < wide_basename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: basename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libcstring_system_string_copy(
	     basename,
	     segment_table->basename,
	     segment_table->basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
	basename[ segment_table->basename_size - 1 ] = 0;
#else
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) basename,
		          basename_size,
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) basename,
		          basename_size,
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) basename,
		          basename_size,
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) basename,
		          basename_size,
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );
}

/* Sets the basename in the segment table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_set_basename_wide(
     libvmdk_segment_table_t *segment_table,
     const wchar_t *basename,
     size_t basename_length,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_set_basename_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result            = 0;
#endif

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename      = NULL;
		segment_table->basename_size = 0;
	}
#if defined( LIBCSTRING_WIDE_SYSTEM_CHARACTER_TYPE )
	segment_table->basename_size = basename_length + 1;
#else
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          libcstring_narrow_system_string_codepage,
		          &( segment_table->basename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          libcstring_narrow_system_string_codepage,
		          &( segment_table->basename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine basename size.",
		 function );

		goto on_error;
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	segment_table->basename = libcstring_system_string_allocate(
	                           segment_table->basename_size );

	if( segment_table->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_WIDE_SYSTEM_CHARACTER_TYPE )
	if( libcstring_system_string_copy(
	     segment_table->basename,
	     basename,
	     basename_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set basename.",
		 function );

		goto on_error;
	}
	segment_table->basename[ basename_length ] = 0;
#else
	if( libcstring_narrow_system_string_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) segment_table->basename,
		          segment_table->basename_size,
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          (libuna_utf32_character_t *) basename,
		          basename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) segment_table->basename,
		          segment_table->basename_size,
		          libcstring_narrow_system_string_codepage,
		          (libuna_utf16_character_t *) basename,
		          basename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set basename.",
		 function );

		goto on_error;
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */
	return( 1 );

on_error:
	if( segment_table->basename != NULL )
	{
		memory_free(
		 segment_table->basename );

		segment_table->basename = NULL;
	}
	segment_table->basename_size = 0;

	return( -1 );
}
#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Retrieves the number of handles in the segment table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_get_number_of_handles(
     libvmdk_segment_table_t *segment_table,
     int *number_of_handles,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_get_number_of_handles";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     segment_table->segment_file_handle_array,
	     number_of_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in segment file handle array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a segment file handle from the segment table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_get_handle(
     libvmdk_segment_table_t *segment_table,
     int handle_index,
     libvmdk_segment_file_handle_t **handle,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_get_handle";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry(
	     segment_table->segment_file_handle_array,
	     handle_index,
	     (intptr_t **) handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from segment file handle array.",
		 function,
		 handle_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets a segment file handle in the segment table
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_set_handle(
     libvmdk_segment_table_t *segment_table,
     int handle_index,
     libvmdk_segment_file_handle_t *handle,
     libcerror_error_t **error )
{
	libvmdk_segment_file_handle_t *segment_file_handle = NULL;
	static char *function                              = "libvmdk_segment_table_set_handle";
	int number_of_handles                              = 0;

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     segment_table->segment_file_handle_array,
	     &number_of_handles,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in segment file handle array.",
		 function );

		return( -1 );
	}
	if( handle_index >= number_of_handles )
	{
		if( libvmdk_segment_table_resize(
		     segment_table,
		     handle_index + 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize the segment table.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libcdata_array_get_entry(
		     segment_table->segment_file_handle_array,
		     handle_index,
		     (intptr_t **) &segment_file_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d from segment file handle array.",
			 function,
			 handle_index );

			return( -1 );
		}
		if( segment_file_handle != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: segment file handle entry: %d value already set.",
			 function,
			 handle_index );

			return( -1 );
		}
	}
	if( libcdata_array_set_entry(
	     segment_table->segment_file_handle_array,
	     handle_index,
	     (intptr_t *) handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set entry: %d in segment file handle array.",
		 function,
		 handle_index );

		return( -1 );
	}
	return( 1 );
}

/* Sets the maximum segment size
 * Returns 1 if successful or -1 on error
 */
int libvmdk_segment_table_set_maximum_segment_size(
     libvmdk_segment_table_t *segment_table,
     size64_t maximum_segment_size,
     libcerror_error_t **error )
{
	static char *function = "libvmdk_segment_table_set_maximum_segment_size";

	if( segment_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment table.",
		 function );

		return( -1 );
	}
	segment_table->maximum_segment_size = maximum_segment_size;

	return( 1 );
}

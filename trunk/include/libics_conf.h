/*
 * libics: Image Cytometry Standard file reading and writing.
 *
 * Copyright (C) 2000-2006 Cris Luengo and others
 * email: clluengo@users.sourceforge.net
 *
 * Large chunks of this library written by
 *    Bert Gijsbers
 *    Dr. Hans T.M. van der Voort
 * And also Damir Sudar, Geert van Kempen, Jan Jitze Krol,
 * Chiel Baarslag and Fons Laan.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*
 * FILE : libics_conf.h
 *
 * This file allows you to configure libics. Only needed to build the
 * library
 *
 */

#ifndef LIBICS_CONF_H
#define LIBICS_CONF_H

/*
 * Doubles larger than ICS_MAX_DOUBLE or smaller than ICS_MIN_DOUBLE
 * are written in scientific notation.
 */
#define ICS_MAX_DOUBLE 10000000
#define ICS_MIN_DOUBLE 0.001

/*
 * ICS_HISTARRAY_INCREMENT sets the increment for the array allocated
 * to contain the history strings.
 */
#define ICS_HISTARRAY_INCREMENT 1024

/*
 * ICS_BUF_SIZE is the size of the buffer allocated to:
 * - Do the compression. This is independend from the memory allocated by
 * zlib for the dictionary.
 * - Decompress stuff into when skipping a data block (IcsSetIdsBlock() for
 * compressed files).
 * - Copy data over from one IDS file to another when the ICS file is
 * opened for updating.
 */
#define ICS_BUF_SIZE 16384


/*
 * If ICS_FORCE_C_LOCALE is set, the locale is set to "C" before each read
 * or write operation. This ensures that the ICS header file is formatted
 * properly. If your program does not modify the locale, you can safely
 * comment out is line: the "C" locale is the default. If this constant is
 * not defined and your program changes the locale, the resulting ICS
 * header file might not be readable by other applications: it will only
 * be readable if the reading application is set to the same locale as the
 * writing application.
 * The ICS standard calls for the locale to be set to "C".
 */
#define ICS_FORCE_C_LOCALE

/*********************************************************************
 *** If we are not using the configure script:                     ***
 *** (currently configured for 32-bit Windows - edit as necessary) ***
 *********************************************************************/
#undef ICS_USING_CONFIGURE
#if !defined(ICS_USING_CONFIGURE)

/*
 * If ICS_DO_GZEXT is defined, the code will search for IDS files
 * which have the .ids.gz or .ids.Z filename extension.
 */
#define ICS_DO_GZEXT

/*
 * If ICS_ZLIB is defined, the zlib dependancy is included, and the
 * library will be able to read GZIP compressed files.
 * This variable is set by the makefile -- enable ZLIB support there.
 */
/*#define ICS_ZLIB*/

/*
 * These are used internally when the precise length of a variable is needed.
 * These are OK for 32-bit systems, but they might be different for 64-bit
 * systems.
 * We also use size_t for a variable that is as wide as a pointer (i.e. can
 * hold the size of any data block).
 */
typedef unsigned char ics_t_uint8;
typedef signed char ics_t_sint8;
typedef unsigned short ics_t_uint16;
typedef signed short ics_t_sint16;
typedef unsigned int ics_t_uint32;
typedef signed int ics_t_sint32;
typedef float ics_t_real32;
typedef double ics_t_real64;

/*********************************************************************
 *** If we are using the configure script:
 *********************************************************************/
#else

/* Define if your system has strcasecmp() in strings.h */
#undef HAVE_STRINGS_STRCASECMP
/* Define if your system has strcasecmp() in string.h */
#undef HAVE_STRING_STRCASECMP

/* Location of the . */
#undef ICS_DO_GZEXT

/* Whether to search for IDS files with .ids.gz or .ids.Z extension. */
#undef ICS_DO_GZEXT

/* Whether to use zlib compression. */
#undef ICS_ZLIB

/* The number of bytes in a char.  */
#undef SIZEOF_CHAR

/* The number of bytes in a short.  */
#undef SIZEOF_SHORT

/* The number of bytes in a int.  */
#undef SIZEOF_INT

/* The number of bytes in a long.  */
#undef SIZEOF_LONG

/* The number of bytes in a unsigned char.  */
#undef SIZEOF_UNSIGNED_CHAR

/* The number of bytes in a unsigned short.  */
#undef SIZEOF_UNSIGNED_SHORT

/* The number of bytes in a unsigned int.  */
#undef SIZEOF_UNSIGNED_INT

/* The number of bytes in a unsigned long.  */
#undef SIZEOF_UNSIGNED_LONG

/* The number of bytes in a float.  */
#undef SIZEOF_FLOAT

/* The number of bytes in a double.  */
#undef SIZEOF_DOUBLE

/*
 * These are used internally when the precise length of a variable is needed.
 * We also use size_t for a variable that is as wide as a pointer (i.e. can
 * hold the size of any data block).
 */

#if SIZEOF_CHAR == 1
typedef signed char ics_t_sint8;
#elif SIZEOF_SHORT == 1
typedef signed short ics_t_sint8;
#elif SIZEOF_INT == 1
typedef signed int ics_t_sint8;
#elif SIZEOF_LONG == 1
typedef signed long ics_t_sint8;
#endif

#if SIZEOF_UNSIGNED_CHAR == 1
typedef unsigned char ics_t_uint8;
#elif SIZEOF_UNSIGNED_SHORT == 1
typedef unsigned short ics_t_uint8;
#elif SIZEOF_UNSIGNED_INT == 1
typedef unsigned int ics_t_uint8;
#elif SIZEOF_UNSIGNED_LONG == 1
typedef unsigned long ics_t_uint8;
#endif

#if SIZEOF_CHAR == 2
typedef signed char ics_t_sint16;
#elif SIZEOF_SHORT == 2
typedef signed short ics_t_sint16;
#elif SIZEOF_INT == 2
typedef signed int ics_t_sint16;
#elif SIZEOF_LONG == 2
typedef signed long ics_t_sint16;
#endif

#if SIZEOF_UNSIGNED_CHAR == 2
typedef unsigned char ics_t_uint16;
#elif SIZEOF_UNSIGNED_SHORT == 2
typedef unsigned short ics_t_uint16;
#elif SIZEOF_UNSIGNED_INT == 2
typedef unsigned int ics_t_uint16;
#elif SIZEOF_UNSIGNED_LONG == 2
typedef unsigned long ics_t_uint16;
#endif

#if SIZEOF_CHAR == 4
typedef signed char ics_t_sint32;
#elif SIZEOF_SHORT == 4
typedef signed short ics_t_sint32;
#elif SIZEOF_INT == 4
typedef signed int ics_t_sint32;
#elif SIZEOF_LONG == 4
typedef signed long ics_t_sint32;
#endif

#if SIZEOF_UNSIGNED_CHAR == 4
typedef unsigned char ics_t_uint32;
#elif SIZEOF_UNSIGNED_SHORT == 4
typedef unsigned short ics_t_uint32;
#elif SIZEOF_UNSIGNED_INT == 4
typedef unsigned int ics_t_uint32;
#elif SIZEOF_UNSIGNED_LONG == 4
typedef unsigned long ics_t_uint32;
#endif

#if SIZEOF_FLOAT == 4
typedef float ics_t_real32;
#elif SIZEOF_DOUBLE == 4
typedef double ics_t_real32;
#endif

#if SIZEOF_FLOAT == 8
typedef float ics_t_real64;
#elif SIZEOF_DOUBLE == 8
typedef double ics_t_real64;
#endif

#endif /* not using configure script */

#endif /* LIBICS_CONF_H */

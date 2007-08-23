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
 * FILE : libics_data.c
 *
 * This file declares some data structures used when reading and
 * writing the ICS headers.
 */

#include "libics_intern.h"

Ics_Symbol G_CatSymbols[] =
{
   { "source",           ICSTOK_SOURCE },
   { "layout",           ICSTOK_LAYOUT },
   { "representation",   ICSTOK_REPRES },
   { "parameter",        ICSTOK_PARAM },
   { ICS_HISTORY,        ICSTOK_HISTORY }, /* Don't want duplicate strings... */
   { "sensor",           ICSTOK_SENSOR },
   { "end",              ICSTOK_END }
};

Ics_Symbol G_SubCatSymbols[] =
{
   {"file",               ICSTOK_FILE },
   {"offset",             ICSTOK_OFFSET },
   {"parameters",         ICSTOK_PARAMS },
   {"order",              ICSTOK_ORDER },
   {"sizes",              ICSTOK_SIZES },
   {"coordinates",        ICSTOK_COORD },
   {"significant_bits",   ICSTOK_SIGBIT },
   {"format",             ICSTOK_FORMAT },
   {"sign",               ICSTOK_SIGN },
   {"compression",        ICSTOK_COMPR },
   {"byte_order",         ICSTOK_BYTEO },
   {"origin",             ICSTOK_ORIGIN },
   {"scale",              ICSTOK_SCALE },
   {"units",              ICSTOK_UNITS },
   {"labels",             ICSTOK_LABELS },
   {"SCIL_TYPE",          ICSTOK_SCILT },
   {"type",               ICSTOK_TYPE },
   {"model",              ICSTOK_MODEL },
   {"s_params",           ICSTOK_SPARAMS }
};

Ics_Symbol G_SubSubCatSymbols[] =
{
   {"Channels",          ICSTOK_CHANS },
   {"PinholeRadius",     ICSTOK_PINHRAD },
   {"LambdaEx",          ICSTOK_LAMBDEX },
   {"LambdaEm",          ICSTOK_LAMBDEM },
   {"ExPhotonCnt",       ICSTOK_PHOTCNT },
   {"RefrInxMedium",     ICSTOK_REFRIME },
   {"NumAperture",       ICSTOK_NUMAPER },
   {"RefrInxLensMedium", ICSTOK_REFRILM },
   {"PinholeSpacing",    ICSTOK_PINHSPA }
};

Ics_Symbol G_ValueSymbols[] =
{
   {"uncompressed",      ICSTOK_COMPR_UNCOMPRESSED },
   {"compress",          ICSTOK_COMPR_COMPRESS },
   {"gzip",              ICSTOK_COMPR_GZIP },
   {"integer",           ICSTOK_FORMAT_INTEGER },
   {"real",              ICSTOK_FORMAT_REAL },
   {"float",             ICSTOK_FORMAT_REAL }, /* CAUTION: this makes this list one longer than you'd expect */
   {"complex",           ICSTOK_FORMAT_COMPLEX },
   {"signed",            ICSTOK_SIGN_SIGNED },
   {"unsigned",          ICSTOK_SIGN_UNSIGNED }
};

Ics_SymbolList G_Categories =
{
   ICSTOK_LASTMAIN,
   G_CatSymbols
};

Ics_SymbolList G_SubCategories =
{
   ICSTOK_LASTSUB - ICSTOK_FIRSTSUB - 1,
   G_SubCatSymbols
};

Ics_SymbolList G_SubSubCategories =
{
   ICSTOK_LASTSUBSUB - ICSTOK_FIRSTSUBSUB - 1,
   G_SubSubCatSymbols
};

Ics_SymbolList G_Values =
{
   ICSTOK_LASTVALUE - ICSTOK_FIRSTVALUE - 1 + 1, /* See above: this list is one longer than expected. */
   G_ValueSymbols
};

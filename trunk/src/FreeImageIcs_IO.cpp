#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_Private.h"
#include "FreeImageIcs_MetaData.h"

#include "FreeImageAlgorithms_Error.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#include "libics_ll.h"


// This gets the size in bytes of all the dimensions below that of the 
// specified dimension.
// For example if will have 4 dimensional data x y z t
// GetTotalDimensionalDataSize(ics, 2, &size)
// size would return the size of the all the t dimensions below each z dimension.

static Ics_Error GetTotalDimensionalDataSize(ICS *ics, int dimension, size_t *size)
{
	Ics_DataType dataType;
	int ndims;
	size_t dims[ICS_MAXDIM];

	IcsGetLayout (ics, &dataType, &ndims, dims);

	if(dimension < 0 || dimension > ndims)
		return IcsErr_NotValidAction;

	size_t total_size = 1; 

	// Get the combined size of all the dimension after x,y.
	for (int i=0; i <= dimension; i++)
		total_size *= dims[i];

	*size = (total_size * GetIcsDataTypeBPP (dataType)) / 8;
	
	return IcsErr_Ok;
}


static void CopyBytesToFIBitmap(FIBITMAP *src, BYTE *data, int padded)
{
	register int y;

	int data_line_length, height = FreeImage_GetHeight(src);
	
	if(padded)
		data_line_length = FreeImage_GetPitch(src);
	else
		data_line_length = FreeImage_GetLine(src);
	
	BYTE *bits;
	BYTE *data_row;
			
	for( y = 0; y < height; y++) {

		bits = (BYTE *) FreeImage_GetScanLine(src, y);

		data_row = data + (height - y - 1) * data_line_length;

		memcpy( bits, data_row, data_line_length ); 
	}
}


static FIBITMAP*
FreeImageIcs_CreateFIB(BYTE *data, Ics_DataType icsType, int bpp, int width, int height, int colour, int padded)
{
	FIBITMAP 	*dib;
	BYTE 		*data_ptr;
	
	// Check the data type and convert to bpp
	if ( bpp < 0 )
		return NULL;

	data_ptr = data;

	FREE_IMAGE_TYPE fit = IcsTypeToFreeImageType (icsType);

	if(bpp == 8 && colour == 1)
		bpp = 24;

	dib = FreeImage_AllocateT(fit, width, height, bpp, 0, 0, 0);

	if(bpp == 8)
		FreeImageAlgorithms_SetGreyLevelPalette(dib);

	CopyBytesToFIBitmap(dib, data, padded);

	return dib;
}


FIBITMAP* DLL_CALLCONV
FreeImageIcs_GetIcsImageDataSlice(ICS *ics, int dimension, int slice)
{
	Ics_DataType dataType;
	int ndims;
	size_t dims[ICS_MAXDIM];

	// Reset the poiter to the beginning of file.
	if(ics->BlockRead)
		IcsSetIdsBlock (ics, 0, SEEK_SET);

	IcsGetLayout (ics, &dataType, &ndims, dims);

	// Dimension must be less than the total dimensions
	if(dimension >= ndims)
		return NULL;

	// If the slice is greater than the size for that dimension return error;
	if(slice >= (int) dims[dimension])
		return NULL;

	size_t bufsize;

	GetTotalDimensionalDataSize(ics, dimension, &bufsize);
		
	if(IcsSkipDataBlock  (ics, bufsize * slice) != IcsErr_Ok)
		return NULL;

	BYTE *buf = (BYTE *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	if(IcsGetDataBlock  (ics, buf, bufsize) != IcsErr_Ok) {
		free(buf);
		return NULL;
	}

	FIBITMAP *dib = FreeImageIcs_CreateFIB(buf, dataType, GetIcsDataTypeBPP (dataType), (int) dims[0], (int) dims[1], 0, 0);
	
	free(buf);

	return dib;
}


static FIBITMAP*
LoadFIBFromColourIcsFile (ICS *ics, int width, int height)
{
	FIBITMAP *dib = FreeImage_AllocateT(FIT_BITMAP, width, height, 24, 0, 0, 0);
	 
	FIBITMAP *red_fib = FreeImageIcs_GetIcsImageDataSlice(ics, 1, 0);
	FIBITMAP *green_fib = FreeImageIcs_GetIcsImageDataSlice(ics, 1, 1);
	FIBITMAP *blue_fib = FreeImageIcs_GetIcsImageDataSlice(ics, 1, 2);

	FreeImage_SetChannel(dib, red_fib, FICC_RED);
	FreeImage_SetChannel(dib, green_fib, FICC_GREEN);
	FreeImage_SetChannel(dib, blue_fib, FICC_BLUE);

	FreeImage_Unload(red_fib);
	FreeImage_Unload(green_fib);
	FreeImage_Unload(blue_fib);

	return dib;
}

int DLL_CALLCONV
FreeImageIcs_IsIcsFile (const char *filepath)
{
	return IcsVersion (filepath, 0) ? 1 : 0;
}


int DLL_CALLCONV
FreeImageIcs_IsIcsFileColourFile(ICS *ics)
{
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];
	char order[10], label[50];

	// GCI haven't been storing c is the labels or ordering for colour images.
	// They have left it as z. However, they have added some history with key labels 
	// that does the same thing.
	
	if(FreeImageIcs_GetFirstIcsHistoryValueWithKey(ics, "labels", label) != FREEIMAGE_ALGORITHMS_SUCCESS)
		return 0;

	if(strcmp(label, "x y c") == 0)
		return 1;

	// More tradintional layout
	IcsGetLayout (ics, &dt, &ndims, (size_t *) dims);

	// If any of the labels or orders contain a c then it is some form of colour image.
	for(int i=0; i < ndims; i++) {
		
		if(IcsGetOrder (ics, i, order, label) != IcsErr_Ok)
			return 0;

		if(strcmp(order, "c") == 0)
			return 1;
	}

	return 0;
}


int DLL_CALLCONV
FreeImageIcs_NumberOfDimensions (ICS *ics)
{
	Ics_DataType dt;
	int ndims;
	int dims[ICS_MAXDIM];
	
	IcsGetLayout (ics, &dt, &ndims, (size_t *) dims);

	return ndims;
}


int DLL_CALLCONV
FreeImageIcs_GetDimensionDetails (ICS *ics, int dimension, char* order, char *label, int* size)
{	
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];
	
	IcsGetLayout (ics, &dt, &ndims, (size_t *) dims);

	IcsGetOrder  (ics, dimension, order, label);
	FreeImageIcs_GetLabelForDimension (ics, dimension, label);

	*size = dims[dimension];

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFile (ICS *ics, int padded)
{
	FIBITMAP *dib;
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];

	IcsGetLayout (ics, &dt, &ndims, (size_t *) dims);

	if(FreeImageIcs_IsIcsFileColourFile(ics))
		dib = LoadFIBFromColourIcsFile (ics, dims[0], dims[1]);
	else
		dib = FreeImageIcs_GetIcsImageDataSlice(ics, 1, 0);
	
	return dib;
}


FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFilePath (const char* filepath, int padded)
{
	ICS *ics;

	IcsOpen(&ics, filepath, "rw");

	FIBITMAP *fib = FreeImageIcs_LoadFIBFromIcsFile (ics, padded);

	IcsClose(ics);

	return fib;
}



// Ics Saving Code

static void
GetColourImageDataInIcsFormat(FIBITMAP *dib, void *data)
{
	register int channel, x, y;  
	int i=0;
	int line = FreeImage_GetLine(dib);
	int height = FreeImage_GetHeight(dib);

	BYTE *bits = FreeImage_GetBits(dib);
	BYTE *buffer = (BYTE *) data;		
	
	for(channel = 2; channel >= 0; channel-- )
	{
		for(y = 0; y < height; y++) {

			bits = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

			for( x = channel; x < line; x+=3)
				*buffer++ = bits[x]; i++;
		}
	}
}


// This function get the bits bottom row first.
// It also ignores padding bytes as these are not stored
// in ics files.
static void
FreeImage_GetBitsVerticalFlip(FIBITMAP *dib, BYTE *bytes)
{
	register int y;
	BYTE *scanline_bytes;
	int height = FreeImage_GetHeight(dib);
	int pitch = FreeImage_GetPitch(dib);
	int line = FreeImage_GetLine(dib); 
	
	for( y = 0; y < height; y++) {

		scanline_bytes = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

		memcpy( bytes, scanline_bytes, line ); 
				
		bytes += line;
	}
}

int DLL_CALLCONV
FreeImageIcs_SaveGreyScaleImage (FIBITMAP *dib, const char *filepath)
{
	ICS* ics;
	Ics_Error err;
	int ndims;
	int dims[2];
	int bufsize;
	int bytes_per_pixel;

	if(dib == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	err = IcsOpen (&ics, filepath, "w2");

	if (err != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	bytes_per_pixel = FreeImage_GetBPP(dib) / 8;

	ndims = 2;	
	dims[0] = FreeImage_GetWidth(dib); 
    dims[1] = FreeImage_GetHeight(dib);
  
	Ics_DataType dt = FreeImageTypeToIcsType (FreeImage_GetImageType(dib));

	bufsize = dims[0] * dims[1] * bytes_per_pixel;

	IcsSetOrder  (ics, 0, "x", "x-position");
	IcsSetOrder  (ics, 1, "y", "y-position");
	IcsAddHistory  (ics, "labels", "x y");

	BYTE *bits = (BYTE*) malloc(bufsize);

	FreeImage_GetBitsVerticalFlip(dib, bits);

	if( IcsSetLayout(ics, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		goto Error;

	if( (err = IcsSetData(ics, bits, bufsize)) != IcsErr_Ok)
		goto Error;
		
	if( IcsSetCompression (ics, IcsCompr_gzip, 0) != IcsErr_Ok)
		goto Error;
	
	if( IcsClose (ics) != IcsErr_Ok)
		goto Error;

	free(bits);

	return FREEIMAGE_ALGORITHMS_SUCCESS;

	Error:

	if(bits)
		free(bits);

	return FREEIMAGE_ALGORITHMS_ERROR;
}


int DLL_CALLCONV
FreeImageIcs_SaveColourImage (FIBITMAP *dib, const char *filepath)
{
	ICS* ics;
	Ics_Error err;
	Ics_DataType dt = Ics_uint8;
	FIBITMAP *standard_dib;
	int ndims;
	int dims[3];
	int bufsize;
	
	if(dib == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImageAlgorithms_IsGreyScale(dib))
		return FREEIMAGE_ALGORITHMS_ERROR;

	err = IcsOpen (&ics, filepath, "w2");

	if (err != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if(FreeImage_GetBPP(dib) != 24)
		standard_dib = FreeImage_ConvertTo24Bits(dib);
	else
		standard_dib = FreeImage_Clone(dib);

	ndims = 3;	
	dims[0] = FreeImage_GetWidth(standard_dib); 
    dims[1] = FreeImage_GetHeight(standard_dib);
	dims[2] = 3;
  
	bufsize = dims[0] * dims[1] * dims[2];

	IcsSetOrder  (ics, 0, "x", "x-position");
	IcsSetOrder  (ics, 1, "y", "y-position");
	IcsSetOrder	 (ics, 2, "c", "colour");
	IcsAddHistory  (ics, "labels", "x y c");

	BYTE *bits = (BYTE*) malloc(bufsize);

	GetColourImageDataInIcsFormat(standard_dib, bits);

	if( IcsSetLayout(ics, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if( (err = IcsSetData(ics, bits, bufsize)) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
		
	if( IcsSetCompression (ics, IcsCompr_gzip, 0) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( IcsClose (ics) != IcsErr_Ok) {
		
		FreeImage_Unload(dib);
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	free(bits);
	FreeImage_Unload(standard_dib);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageIcs_SaveImage (FIBITMAP *dib, const char *filepath)
{
	if(FreeImageAlgorithms_IsGreyScale(dib))
		return FreeImageIcs_SaveGreyScaleImage (dib, filepath);
	else
		return FreeImageIcs_SaveColourImage (dib, filepath);
}



/*
// Save an array of fibitmaps as a multidimensal ics.
int DLL_CALLCONV
FreeImageIcs_SaveGreyScaleImages (FIBITMAP **dib, int size,
	const char *third_dimension_name, const char *third_dimension_description, const char *filepath)
{
	ICS* ics;
	Ics_Error err;
	Ics_DataType dt = Ics_uint8;
	FIBITMAP *standard_dib;
	int ndims;
	int dims[2];
	int bufsize;
	
	if(dib == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	err = IcsOpen (&ics, filepath, "w2");

	if (err != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	standard_dib = FreeImage_Clone(dib);

	ndims = 3;	
	dims[0] = FreeImage_GetWidth(standard_dib); 
    dims[1] = FreeImage_GetHeight(standard_dib);
	dims[2] = size;

	bufsize = dims[0] * dims[1] * dims[2];

	IcsSetOrder  (ics, 0, "x", "x-position");
	IcsSetOrder  (ics, 1, "y", "y-position");
	IcsSetOrder  (ics, 1, third_dimension_name, third_dimension_description);
	IcsAddHistory  (ics, "labels", "x y");

	FreeImage_FlipVertical(standard_dib);

	if( IcsSetLayout(ics, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if( (err = IcsSetData(ics, FreeImage_GetBits(standard_dib), bufsize)) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
		
	if( IcsSetCompression (ics, IcsCompr_gzip, 0) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( IcsClose (ics) != IcsErr_Ok) {
		
		FreeImage_Unload(dib);
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	FreeImage_Unload(standard_dib);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}
*/
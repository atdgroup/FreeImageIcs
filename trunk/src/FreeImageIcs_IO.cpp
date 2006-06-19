#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_MetaData.h"
#include "FreeImageIcs_Private.h"

#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"


#include <iostream>

// Old ics file save the data as all the red pixels then all the greens and then all the blues
// Yes crap - As this is slower as it requires three passes.
static FIBITMAP*
LoadICSColourImage(BYTE *data, int width, int height, int padded)
{
	FIBITMAP 	*dib; 
	BYTE *bits, *buffer;
	register int channel, x, y;
	int data_line_length;   

	if ( (dib = FreeImage_AllocateT (FIT_BITMAP, width, height, 24, 0, 0, 0)) == NULL )
		return NULL;
			
	if(padded)
		data_line_length = FreeImage_GetPitch(dib);
	else
		data_line_length = FreeImage_GetLine(dib);   
			
	bits = (BYTE *) FreeImage_GetBits(dib);
	buffer = (BYTE *) data;
			
	// blue == 0; green = 1; red == 2
	for(channel = 2; channel >= 0; channel-- )
	{
		for(y = 0; y < height; y++) {

			bits = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

			for( x = channel; x < data_line_length; x+=3) {
				
				bits[x] = buffer[0];  
		
				buffer++;
			}
		}
	 }
	  
	return dib;
}


static FREE_IMAGE_TYPE
IcsTypeToFreeImageType (Ics_DataType icsType)
{
	switch (icsType)
	{
	 case Ics_sint8:
	 case Ics_uint8:
	 
		 return FIT_BITMAP;
		 
	 case Ics_sint16:
	 
		 return FIT_INT16;
		 
	 case Ics_uint16:
	 
		 return FIT_UINT16;

	 case Ics_sint32:
	 
		 return FIT_INT32;
		 
	 case Ics_uint32:
	 
		 return FIT_UINT32;
		 
	 case Ics_real32:
	 
		 return FIT_FLOAT;
		 
	 case Ics_real64:
	 
		 return FIT_DOUBLE;
	}
	
	return FIT_UNKNOWN;
}


static Ics_DataType
FreeImageTypeToIcsType (FREE_IMAGE_TYPE fit)
{
	switch (fit)
	{
	 	case FIT_BITMAP:	
	
	 		return Ics_uint8;
	 
		case FIT_INT16:
		
			return Ics_sint16;
	
		case FIT_UINT16:
		
			return Ics_uint16;
			
		case FIT_INT32:
		
			return Ics_sint32;
			
		case FIT_UINT32:
		
			return Ics_uint32;
			
		case FIT_FLOAT:
		
			return Ics_real32;
			
		case FIT_DOUBLE:
		
			return Ics_real64;
	}		
	
	return Ics_unknown;
}

static int
GetIcsDataTypeBPP (Ics_DataType icsType)
{
	switch (icsType)
	{
	 case Ics_sint8:
		 return 8;
		 
	 case Ics_uint8:
		 return 8;
		 
	 case Ics_sint16:
		 return 16;
		 
	 case Ics_uint16:
		 return 16;
		 
	 case Ics_sint32:
		 return 32;
		 
	 case Ics_uint32:
		 return 32;
		 
	 case Ics_real32:
		 return 32;
		 
	 case Ics_real64:
		 return 64;
	}
	
	return 0;
}



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
	for (int i=0; i < dimension; i++)
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
GetIcsDimensionXYImage(FreeImageIcsPointer fip, ...)
{
	if(fip == NULL)
		return NULL;

	Ics_DataType dataType;
	int ndims;
	size_t dims[ICS_MAXDIM];

	// You have to reopen so to make the pointer that IcsSkipDataBlock uses point back to
	// zero. What can I say ICS API is crap.
	// We really should patch libics.
	ReOpenExistingIcsFileInReadMode(fip);

	IcsGetLayout (fip->ip, &dataType, &ndims, dims);

	int count = 0;
	int dimension = 2;
	int dimensions[10];
	int dimension_index;

	va_list ap;
	va_start(ap, fip);

	while((dimension_index = va_arg(ap, int)) >= 0)
	{
		// If the user has specified more params than the is dimensions above x,y return error.
		if(dimension >= ndims)
			return NULL;

		// If the dimensions index is greater than the size for that dimension return error;
		if(dimension_index >= (int) dims[dimension])
			return NULL;

		dimensions[dimension - 2] = dimension_index;
		dimension++;
	}

	va_end(ap);

	size_t data_position = 0;
	size_t size;

	ArrayReverse(dimensions, ndims - 2);
	
	GetTotalDimensionalDataSize(fip->ip, ndims - 1, &size);
	size_t bufsize = size;

	for(int i=2; i < ndims; i++) {
		
		GetTotalDimensionalDataSize(fip->ip, i, &size);

		data_position += (dimensions[i - 2] * size);

		//printf("dimension %d size %d\n", dimensions[i], size);
	}
		
	if(IcsSkipDataBlock  (fip->ip, data_position) != IcsErr_Ok)
		return NULL;

	BYTE *buf = (BYTE *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	if(IcsGetDataBlock  (fip->ip, buf, bufsize) != IcsErr_Ok) {
		free(buf);
		return NULL;
	}

	FIBITMAP *dib = FreeImageIcs_CreateFIB(buf, dataType, GetIcsDataTypeBPP (dataType), (int) dims[0], (int) dims[1], 0, 0);
	
	free(buf);

	return dib;
}


// This function gets the first xy image for the specified splice of the
// specified dimension. All other dimensions are held at zero.
DLL_API FIBITMAP* DLL_CALLCONV
GetIcsXYImageForDimensionSlice(FreeImageIcsPointer fip, int dimension, int slice)
{
	if(fip == NULL)
		return NULL;

	Ics_DataType dataType;
	int ndims;
	size_t dims[ICS_MAXDIM];

	// You have to reopen so to make the pointer that IcsSkipDataBlock uses point back to
	// zero. What can I say ICS API is crap.
	// We really should patch libics.
	ReOpenExistingIcsFileInReadMode(fip);

	IcsGetLayout (fip->ip, &dataType, &ndims, dims);

	if(ndims <= 2)
		return NULL;

	// Dimesion must be greater than 1 as this is a multidimensional image.
	if(dimension < 2 || dimension >= ndims)
		return NULL;

	// If the slice is greater than the size for that dimension return error;
	if(slice >= (int) dims[dimension])
		return NULL;

	size_t bufsize;

	GetTotalDimensionalDataSize(fip->ip, dimension, &bufsize);
		
	if(IcsSkipDataBlock  (fip->ip, bufsize * slice) != IcsErr_Ok)
		return NULL;

	BYTE *buf = (BYTE *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	if(IcsGetDataBlock  (fip->ip, buf, bufsize) != IcsErr_Ok) {
		free(buf);
		return NULL;
	}

	FIBITMAP *dib = FreeImageIcs_CreateFIB(buf, dataType, GetIcsDataTypeBPP (dataType), (int) dims[0], (int) dims[1], 0, 0);
	
	free(buf);

	return dib;
}


// Checks to see that the ics image data is padded to be byte aligned.
// GCI does not save data with padded bytes!
// Assumes colour image data is 3 dimensions (Yes is a problem 
// if we have genuine > 3 dimensional data.

int DLL_CALLCONV
IsIcsFilePaddded (char *filepath)
{
	Ics_DataType dt;
	int ndims, bytes;
	size_t dims[ICS_MAXDIM];
	size_t dataSize, bufsize;

	ICS* ip;
	Ics_Error retval;

	if((retval = IcsOpen (&ip, filepath, "r")) != IcsErr_Ok)
   		return -1;

	IcsGetLayout (ip, &dt, &ndims, dims);
	
	bytes = GetIcsDataTypeBPP (dt) / 8; 
	
	if( ndims == 2)
		dataSize = dims[0] * dims[1] * bytes;  
	else if( ndims == 3 )
		dataSize = dims[0] * dims[1] * dims[2] * bytes;
	else
		return -1;

	bufsize = IcsGetDataSize (ip);

	if((retval = IcsClose (ip)) != IcsErr_Ok)
   		return -1;

	if(dataSize > bufsize)
		return 1;

	return 0;
}

static FIBITMAP*
LoadFIBFromGreyScaleIcsFile (ICS *ip, int padded)
{
	FIBITMAP *dib;
	FREE_IMAGE_TYPE type;
	Ics_DataType dt;
	Ics_Error retval;
	size_t bufsize;
	char *buf;
	int ndims, width, height, bpp;
	int dims[ICS_MAXDIM];

	IcsGetLayout (ip, &dt, &ndims, (size_t *) dims);

	if( ndims == 3 )
		return NULL;

	type = IcsTypeToFreeImageType (dt);   
	bpp = GetIcsDataTypeBPP (dt); 
	width = dims[0];
	height = dims[1];
	
	bufsize = IcsGetDataSize (ip);
	buf = (char *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	retval = IcsGetData (ip, buf, bufsize);

	if (retval != IcsErr_Ok)
   		return NULL;
	
	if ( (dib = FreeImageAlgorithms_LoadGreyScaleFIBFromArrayData(buf, bpp, width, height, type, padded)) == NULL)
			return NULL;

	free(buf);

	return dib;
}


static FIBITMAP*
LoadFIBFromColourIcsFile (ICS *ip, int padded)
{
	FIBITMAP *dib;
	FREE_IMAGE_TYPE type;
	Ics_DataType dt;
	Ics_Error retval;
	size_t bufsize;
	BYTE *buf;
	int ndims, width, height, channels = 1, bpp;
	int dims[ICS_MAXDIM];

	IcsGetLayout (ip, &dt, &ndims, (size_t *) dims);

	if( ndims != 3 )
		return NULL;

	type = IcsTypeToFreeImageType (dt);   
	bpp = GetIcsDataTypeBPP (dt); 
	width = dims[0];
	height = dims[1]; 
	channels = dims[2];
	
	bufsize = IcsGetDataSize (ip);
	buf = (BYTE *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	retval = IcsGetData (ip, buf, bufsize);

	if (retval != IcsErr_Ok)
   		return NULL;
	
	if ( (dib = LoadICSColourImage(buf, width, height, padded)) == NULL)
		return NULL;
		
	free(buf);
	
	return dib;
}


static void
GetColourImageDataInIcsFormat(FIBITMAP *dib, void *data)
{
	register int channel, x, y;  
	int i=0, height = FreeImage_GetHeight(dib);
	int pitch = FreeImage_GetPitch(dib);

	BYTE *bits = FreeImage_GetBits(dib);
	BYTE *buffer = (BYTE *) data;		
	
	for(channel = 2; channel >= 0; channel-- )
	{
		for(y = 0; y < height; y++) {

			bits = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

			for( x = channel; x < pitch; x+=3)
				*buffer++ = bits[x]; i++;
		}
	}

	i = i;
}


static void
FreeImage_GetBitsVerticalFlip(FIBITMAP *dib, BYTE *bytes)
{
	register int y;
	BYTE *scanline_bytes;
	int height = FreeImage_GetHeight(dib);
	int pitch = FreeImage_GetPitch(dib); 
	
	for( y = 0; y < height; y++) {

		scanline_bytes = (BYTE *) FreeImage_GetScanLine(dib, height - y - 1);

		memcpy( bytes, scanline_bytes, pitch ); 
				
		bytes += pitch;
	}
}

static int
SaveFIBToIcsPointer (FIBITMAP *src, ICS* ics)
{
	Ics_Error err;
	int ndims = 2, dims[3], channels = 1;
	size_t bufsize_in_bytes;
	
	FIBITMAP *dib = FreeImage_Clone(src);

	FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);
	int width = FreeImage_GetWidth(dib); 
	int height = FreeImage_GetHeight(dib);
	int pitch = FreeImage_GetPitch(dib);
	int bpp = FreeImage_GetBPP(dib);
	int bytes_per_pixel = bpp / 8;
	Ics_DataType dt = FreeImageTypeToIcsType(type); 
	
	// ICS doesn't support 24bit images
	if(bpp == 24 && type == FIT_BITMAP) {
	
		channels = 3;	 /* number of samples per pixel */
		dt = Ics_uint8;  /* channel Size */
		ndims = 3;
		
		dims[0] = width; 
        dims[1] = height;
        dims[2] = channels; 
	
		bufsize_in_bytes = width * height * channels; 
	}
	else {

		dims[0] = pitch / bytes_per_pixel;
		dims[1] = height;
	
		bufsize_in_bytes = (height * pitch);
	}
	
	BYTE *bits =(BYTE*) malloc(bufsize_in_bytes);

	if( IcsSetLayout(ics, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(bpp == 24 && ndims == 3) {
		GetColourImageDataInIcsFormat(dib, bits);
		IcsSetOrder  (ics, 0, "x", "x-position");
		IcsSetOrder  (ics, 1, "y", "y-position");
		IcsSetOrder  (ics, 2, "c", "colour");
		IcsAddHistory  (ics, "labels", "x y c");
	}
	else
		FreeImage_GetBitsVerticalFlip(dib, bits);

	if( (err = IcsSetData(ics, bits, bufsize_in_bytes)) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
		
	if( IcsSetCompression (ics, IcsCompr_gzip, 0) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( IcsClose (ics) != IcsErr_Ok) {
		
		FreeImage_Unload(dib);
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	free(bits);
		
	FreeImage_Unload(dib);
	
	return FREEIMAGE_ALGORITHMS_SUCCESS; 
}

int DLL_CALLCONV
FreeImageIcs_IsIcsFile (const char *filepath)
{
	return IcsVersion (filepath, 0) ? 1 : 0;
}

int DLL_CALLCONV
FreeImageIcs_GetNumberOfDimensions (FreeImageIcsPointer fip)
{
	Ics_DataType dt;
	int ndims;
	int dims[ICS_MAXDIM];

	if (IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims) != IcsErr_Ok)
   		return 0;

	return ndims;
}


int
CopyHistoryStringsToArray(FreeImageIcsPointer fip, char *** history_strings, int *number_of_strings)
{
	if (IcsGetNumHistoryStrings (fip->ip, number_of_strings) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(*number_of_strings <= 0)
		return FREEIMAGE_ALGORITHMS_ERROR;

	// Save history to array location
	*history_strings = (char **) malloc(sizeof(char*) * *number_of_strings);

	for(int i=0; i < *number_of_strings; i++) {
		
		(*history_strings)[i] = (char *) malloc(sizeof(char) * 40);
		IcsGetHistoryString (fip->ip, (*history_strings)[i], (i > 0) ? IcsWhich_Next : IcsWhich_First);	
	}
		
	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int 
ReOpenExistingIcsFileInReadMode(FreeImageIcsPointer fip)
{
	if (IcsClose (fip->ip) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if(IcsOpen (&(fip->ip), fip->filepath, "r") != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


/*
int 
ReOpenExistingIcsFileInWriteMode(FreeImageIcsPointer fip, bool maintain_history)
{
	Ics_DataType dt;
	Ics_Error err;
	size_t bufsize;
	int ndims;
	int dims[ICS_MAXDIM];
	char **history_strings = NULL;
	int number_of_strings;

	if(FreeImageIcs_IsIcsFileInWriteMode(fip))
		return FREEIMAGE_ALGORITHMS_SUCCESS;  

	if (IcsClose (fip->ip) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if(IcsOpen (&(fip->ip), fip->filepath, "r") != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if (IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	bufsize = IcsGetDataSize (fip->ip);

	if((fip->buf = (BYTE *) malloc (bufsize)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if ((err = IcsGetData (fip->ip, fip->buf, bufsize)) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if (IcsGetNumHistoryStrings (fip->ip, &number_of_strings) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if(maintain_history) {

		// Save history to array location
		history_strings = (char **) malloc(sizeof(char*) * number_of_strings);

		CopyHistoryStringsToArray(fip, &history_strings, &number_of_strings);
	}

	if (IcsClose (fip->ip) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	// Write the file again
	if(IcsOpen (&(fip->ip), fip->filepath, "w2") != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if( IcsSetLayout(fip->ip, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( IcsSetData(fip->ip, fip->buf, bufsize) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if(maintain_history) {

		for(int i=0; i < number_of_strings; i++) {
			IcsAddHistory (fip->ip, NULL, history_strings[i]);	
			free(history_strings[i]);
		}

		free(history_strings);
	}

	if( IcsSetCompression (fip->ip, IcsCompr_gzip, 0) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}
*/


int 
ReOpenExistingIcsFileInWriteMode(FreeImageIcsPointer fip, bool maintain_history)
{
	Ics_DataType dt;
	Ics_Error err;
	size_t bufsize;
	int ndims;
	int dims[ICS_MAXDIM];
	char **history_strings = NULL;
	int number_of_strings;

	if(FreeImageIcs_IsIcsFileInWriteMode(fip))
		return FREEIMAGE_ALGORITHMS_SUCCESS;  

	if (IcsClose (fip->ip) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if(maintain_history) {

		// Open in rw mode
		if(IcsOpen (&(fip->ip), fip->filepath, "rw") != IcsErr_Ok)
			return FREEIMAGE_ALGORITHMS_ERROR;

		return FREEIMAGE_ALGORITHMS_SUCCESS;  
	}

	if(IcsOpen (&(fip->ip), fip->filepath, "r") != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if (IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	bufsize = IcsGetDataSize (fip->ip);

	if((fip->buf = (BYTE *) malloc (bufsize)) == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if ((err = IcsGetData (fip->ip, fip->buf, bufsize)) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	if (IcsClose (fip->ip) != IcsErr_Ok)
   		return FREEIMAGE_ALGORITHMS_ERROR;

	// Write the file again
	if(IcsOpen (&(fip->ip), fip->filepath, "w2") != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if( IcsSetLayout(fip->ip, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;
	
	if( IcsSetData(fip->ip, fip->buf, bufsize) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	if( IcsSetCompression (fip->ip, IcsCompr_gzip, 0) != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}


int DLL_CALLCONV
FreeImageIcs_IsIcsFileInWriteMode(FreeImageIcsPointer fip)
{
	if(strcmp(fip->access_mode, "w") == 0 || strcmp(fip->access_mode, "w2") == 0)
		return 1;

	return 0;
}


static bool file_exists(const char *filepath)
{
    FILE *file_p;

    if((file_p = fopen(filepath, "r")))
    {
       fclose(file_p);
       return true;
    }
    return false;
}


int DLL_CALLCONV
FreeImageIcs_OpenIcsFile(FreeImageIcsPointer *fip, const char *filepath, const char *access_mode)
{
	*fip = (FreeImageIcsPointer) malloc(sizeof(FreeImageIcs));

	(*fip)->buf = NULL;
	(*fip)->number_of_history_strings = 0;
	(*fip)->iterator = 1;

	strcpy((*fip)->filepath, filepath);
	strcpy((*fip)->access_mode, access_mode);

	if(strcmp(access_mode, "r") == 0) {

		// Justs looks at the extension
		if(IcsVersion (filepath, 1) == 0)
			return FREEIMAGE_ALGORITHMS_ERROR;

		if(IcsOpen (&((*fip)->ip), filepath, "r") != IcsErr_Ok)
			return FREEIMAGE_ALGORITHMS_ERROR;
	}
	else {

		if(file_exists(filepath)) {
		
			if(IcsVersion (filepath, 1) == 0)
				return FREEIMAGE_ALGORITHMS_ERROR;

			if(ReOpenExistingIcsFileInWriteMode(*fip, 1) == FREEIMAGE_ALGORITHMS_ERROR)
				return FREEIMAGE_ALGORITHMS_ERROR;
		}
		else {

			// No such file do a norm open.
			if(IcsOpen (&((*fip)->ip), filepath, access_mode) != IcsErr_Ok)
				return FREEIMAGE_ALGORITHMS_ERROR;
		}
	}

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}


int DLL_CALLCONV
FreeImageIcs_CloseIcsFile(FreeImageIcsPointer fip)
{
	Ics_Error err = IcsClose (fip->ip);

	if(fip->buf != NULL) {
		free(fip->buf);
		fip->buf = NULL;
	}

	free(fip);
    
	if(err != IcsErr_Ok)
		return FREEIMAGE_ALGORITHMS_ERROR;		

	return FREEIMAGE_ALGORITHMS_SUCCESS;  
}




int DLL_CALLCONV
FreeImageIcs_IsIcsFileColourFile(FreeImageIcsPointer fip)
{
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];
	char order[10], label[50];

	IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims);

	if(ndims != 3)
		return 0;

	// If any of the labels or orders contain a c then it is some form of colour image.
	for(int i=0; i < 3; i++) {
		
		if(IcsGetOrder (fip->ip, i, order, label) != IcsErr_Ok)
			return 0;

		if(strcmp(order, "c") == 0)
			return 1;
	}

	// GCI haven't been storing c is the labels or ordering for colour images.
	// They have left it as z. However, they have added some history with key labels 
	// that does the same thing.
	
	if(FreeImageIcs_GetFirstIcsHistoryValueWithKey(fip, "labels", label) != FREEIMAGE_ALGORITHMS_SUCCESS)
		return 0;

	if(strcmp(label, "x y c") == 0)
		return 1;

	return 0;
}


int DLL_CALLCONV
FreeImageIcs_NumberOfDimensions (FreeImageIcsPointer fip)
{
	Ics_DataType dt;
	int ndims;
	int dims[ICS_MAXDIM];
	
	IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims);

	return ndims;
}


int DLL_CALLCONV
FreeImageIcs_GetLabelForDimension (FreeImageIcsPointer fip, int dimension, char *label)
{
	char labels[ICS_LINE_LENGTH];

	FreeImageIcs_GetFirstIcsHistoryValueWithKey(fip, "labels", labels);

	char *result = strtok(labels, " ");

	if(dimension <= 0)
		strcpy(label, result);

	for(int i = 1; i <= dimension; i++) {
		if((result = strtok(NULL, " ")) == NULL)
			return FREEIMAGE_ALGORITHMS_ERROR;
	}

	if(result != NULL)
		strcpy(label, result);
	else
		return FREEIMAGE_ALGORITHMS_ERROR;

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


int DLL_CALLCONV
FreeImageIcs_GetDimensionDetails (FreeImageIcsPointer fip, int dimension, char* order, char *label, int* size)
{	
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];
	
	IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims);

	IcsGetOrder  (fip->ip, dimension, order, label);
	FreeImageIcs_GetLabelForDimension (fip, dimension, label);

	*size = dims[dimension];

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFile (FreeImageIcsPointer fip, int padded)
{
	FIBITMAP *dib;
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];
	
	IcsGetLayout (fip->ip, &dt, &ndims, (size_t *) dims);

	if( ndims >= 3 ) {

		if(FreeImageIcs_IsIcsFileColourFile(fip))
			dib = LoadFIBFromColourIcsFile (fip->ip, padded); 
		else {
			dib = GetIcsXYImageForDimensionSlice(fip, 2, 0);
		}
	}
	else {
		dib = LoadFIBFromGreyScaleIcsFile (fip->ip, padded); 
	}
	
	return dib;
}

FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFilePath (const char* filepath, int padded)
{
	FreeImageIcsPointer fip;

	FreeImageIcs_OpenIcsFile(&fip, filepath, "r");

	return FreeImageIcs_LoadFIBFromIcsFile (fip, padded);
}

int DLL_CALLCONV
FreeImageIcs_SaveFIBToIcsFile (FIBITMAP *dib, const char *pathname)
{
	ICS* ip;
	Ics_Error retval;
	FIBITMAP *standard_dib;

	if(dib == NULL)
		return FREEIMAGE_ALGORITHMS_ERROR;

	retval = IcsOpen (&ip, pathname, "w2");

	if (retval != IcsErr_Ok) {
   		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	if(!FreeImageAlgorithms_IsGreyScale(dib) && FreeImage_GetBPP(dib) != 24)
		standard_dib = FreeImage_ConvertTo24Bits(dib);
	else
		standard_dib = FreeImage_Clone(dib);

	if ( SaveFIBToIcsPointer (standard_dib, ip) == FREEIMAGE_ALGORITHMS_ERROR ) {
		return FREEIMAGE_ALGORITHMS_ERROR;
	}

	FreeImage_Unload(standard_dib);

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

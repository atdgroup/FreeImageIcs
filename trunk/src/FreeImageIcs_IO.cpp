#include "FreeImageIcs_IO.h"
#include "FreeImageIcs_Private.h"
#include "FreeImageIcs_MetaData.h"

#include "FreeImageAlgorithms_Error.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_Utils.h"

#include "libics_ll.h"
#include "libics.h"

#include <vector>
#include <string>
#include <algorithm>

// This gets the size in bytes between each element in a dimension
// The first dimension 0 always return the size of dataType.
static Ics_Error DLL_CALLCONV
GetSizeInBytesBetweenDimensions(size_t* dims, int ndims, Ics_DataType dataType, int dimension, size_t *size)
{
    if(dimension < 0 || dimension > ndims)
		return IcsErr_NotValidAction;

	size_t total_size = 1; 

	// Get the combined size of all the dimension after x,y.
	for (int i=1; i <= dimension; i++)
		total_size *= dims[i-1];

	*size = (total_size * GetIcsDataTypeBPP (dataType)) / 8;
	
	return IcsErr_Ok;
}

// This returns the size in bytes of one dimension slice.
static Ics_Error DLL_CALLCONV
GetSizeInBytesOfOneDimensionSlice(size_t* dims, int ndims, Ics_DataType dataType, int dimension, size_t *size)
{
    if(dimension < 0 || dimension > ndims)
		return IcsErr_NotValidAction;

	size_t total_size = dims[0]; 

	// Get the combined size of all the dimension after x,y.
	for (int i=0; i < dimension; i++)
		total_size *= dims[i];

	*size = (total_size * GetIcsDataTypeBPP (dataType)) / 8;
	
	return IcsErr_Ok;
}

static void DLL_CALLCONV
CopyBytesToFIBitmap(FIBITMAP *src, BYTE *data, int padded)
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


static FIBITMAP* DLL_CALLCONV
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


static void swap_dims(size_t *dims, size_t dim1, size_t dim2)
{
    size_t tmp;

    tmp = dims[dim1];
    dims[dim1] = dims[dim2];
    dims[dim2] = tmp;
}


/*
int DLL_CALLCONV
FreeImageIcs_CreateTestImage(const char *filepath)
{
	Ics_Error err;
    ICS *new_ics;
	int ndims;
    size_t dims[3] = {4, 3, 2};
	size_t bufsize;
    Ics_DataType dt = Ics_uint8;

    // Create new ics file that contails the swapped dimensions
	if((err = IcsOpen (&new_ics, filepath, "w2")) != IcsErr_Ok)
   		goto Error;
  
    BYTE bits[] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5};
    BYTE *tmp_bits = NULL;

    tmp_bits = bits;

    bufsize = 24 * GetIcsDataTypeBPP (dt) / 8;

    if( IcsSetLayout(new_ics, dt, 3, (size_t *) dims) != IcsErr_Ok)
		goto Error;

    if( (err = IcsSetData(new_ics, bits, bufsize)) != IcsErr_Ok)
		goto Error;
		
	//if( IcsSetCompression (new_ics, IcsCompr_gzip, 0) != IcsErr_Ok)
	//	goto Error;
	
	if( IcsClose (new_ics) != IcsErr_Ok)
		goto Error;
	
	return FREEIMAGE_ALGORITHMS_SUCCESS;

	Error:

	return FREEIMAGE_ALGORITHMS_ERROR;
}

int DLL_CALLCONV
FreeImageIcs_TestOpen(const char *filepath)
{
	Ics_Error err;
    ICS *ics;
	int ndims;
	size_t old_dims[10], dims[10];
	size_t bufsize;
    Ics_DataType dt;

    // Create new ics file that contails the swapped dimensions
	if((err = IcsOpen (&ics, filepath, "r")) != IcsErr_Ok)
   		goto Error;
  
    // Get number of dimensions in old ics file
    if((err = IcsGetLayout(ics, &dt, &ndims, dims)) != IcsErr_Ok)
   		goto Error;
	
    bufsize = IcsGetDataSize (ics);

    BYTE *old_bits = (BYTE*) malloc(bufsize);

    // Get data from old ics file and reorder into new.
    if((err = IcsGetData (ics, old_bits, bufsize)) != IcsErr_Ok)
   		goto Error;
	
	if( IcsClose (ics) != IcsErr_Ok)
		goto Error;

    free(old_bits);
    
	return FREEIMAGE_ALGORITHMS_SUCCESS;

	Error:

    if(old_bits)
		free(old_bits);

	return FREEIMAGE_ALGORITHMS_ERROR;
}
*/

inline std::vector<std::string> split( const std::string& s, const std::string& f ) {

    std::vector<std::string> temp;

    if ( f.empty() ) {
        temp.push_back( s );
        return temp;
    }
    typedef std::string::const_iterator iter;
    const iter::difference_type f_size( distance( f.begin(), f.end() ) );
    iter i( s.begin() );
    for ( iter pos; ( pos = search( i , s.end(), f.begin(), f.end() ) ) != s.end(); ) {
        temp.push_back( std::string( i, pos ) );
        advance( pos, f_size );
        i = pos;
    }
    temp.push_back( std::string( i, s.end() ) );

    return temp;
}

int DLL_CALLCONV
SwitchDimensionLabels (ICS *ics, char*out, int dim1, int dim2)
{
	char labels[ICS_LINE_LENGTH];
 
	if(FreeImageIcs_GetFirstIcsHistoryValueWithKey(ics, "Labels", labels) == FREEIMAGE_ALGORITHMS_ERROR)
        return FREEIMAGE_ALGORITHMS_ERROR;

    std::vector<std::string> words( split( labels, " " ) );

    std::string tmp = words[dim1];
    words[dim1] = words[dim2];
    words[dim2] = tmp;

    std::string output;
    std::vector<std::string>::size_type i;

    for(i=0; i < words.size(); i++)
        output += words[i] + " ";

    strcpy(out, output.c_str());

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}

int DLL_CALLCONV
FreeImageIcs_SaveIcsFileWithDimensionsSwapped(ICS *ics, const char *filepath, int dim1, int dim2)
{
	Ics_Error err;
    ICS *new_ics;
	int ndims;
	size_t old_dims[10], dims[10];
	size_t bufsize;
    Ics_DataType dt;

	if(ics == NULL)
		goto Error;
  
    // Create new ics file that contails the swapped dimensions
	 if((err = IcsOpen (&new_ics, filepath, "w2")) != IcsErr_Ok)
   		goto Error;

    // Get number of dimensions in old ics file
    if((err = IcsGetLayout(ics, &dt, &ndims, dims)) != IcsErr_Ok)
   		goto Error;
	
    if(dim1 > ndims || dim2 > ndims)
       goto Error;

    memcpy(old_dims, dims, sizeof(size_t) * 10);
    swap_dims(dims, dim1, dim2);

    bufsize = IcsGetDataSize (ics);

    BYTE *old_bits = NULL, *tmp_old_bits = NULL;
	BYTE *bits = NULL, *tmp_bits = NULL;

    old_bits = tmp_old_bits = (BYTE*) malloc(bufsize);

    // Get data from old ics file and reorder into new.
   // if((err = IcsGetData (ics, old_bits, bufsize)) != IcsErr_Ok)
   //		goto Error;
    
    size_t strides[3] = {3, 2, 1};

    err = IcsGetDataWithStrides  (ics, old_bits, bufsize, strides, ndims);

    if((bits = tmp_bits = (BYTE*) malloc(bufsize)) == NULL)
		goto Error;

    if( IcsSetLayout(new_ics, dt, ndims, (size_t *) dims) != IcsErr_Ok)
		goto Error;

    //int min_dim = MIN(dim1, dim2);
    //int max_dim = MAX(dim1, dim2);

    //size_t min_size, max_size;

    //GetSizeInBytesBetweenDimensions(old_dims, ndims, dt, min_dim, &min_size);
    //GetSizeInBytesOfOneDimensionSlice(dims, ndims, dt, min_dim, &max_size);

    

    err = IcsSetDataWithStrides  (new_ics, old_bits, bufsize, strides, ndims);
        
    if(err != IcsErr_Ok) {
        err = err;
        goto Error;
    }

    printf("\n\n");
    for(int i=0; i < bufsize; i++)
        printf("%d = %d\n", i, old_bits[i]);


    // Loop through the data 
//	size_t offset = 0;

   // while(orig_offset < bufsize) {

      //  new_offset = orig_offset;
        //tmp_old_bits = old_bits + offset; 

        // Each element of the max_dim must be iterated
/*
    for(int i=0; i < old_dims[max_dim]; i++)
        {

            offset = i;
            tmp_bits = bits + offset;

            while(offset < bufsize) {
    
                memcpy(tmp_bits, tmp_old_bits, min_size);
            
                tmp_old_bits += min_size;
                tmp_bits += max_size;
                offset += max_size;
            }
        }
*/
        // Move samller dimension size along original data
   //     orig_offset += min_size;
  //      tmp_old_bits += min_size;
  //  }

    /*
    FreeImageIcs_CopyHistoryText(ics, new_ics);

    char out[100];

    SwitchDimensionLabels (new_ics, out, dim1, dim2);

    // Change Labels History
    FreeImageIcs_ReplaceIcsHistoryValueForKey(new_ics, "Labels", out);

    // Change the order
    char order1[100], label1[100], order2[100], label2[100];
    IcsGetOrder (ics, dim1, order1, label1);
    IcsGetOrder (ics, dim2, order2, label2);
    IcsSetOrder (new_ics, dim1, order2, label2);
    IcsSetOrder (new_ics, dim2, order1, label1);

 //   if( (err = IcsSetData(new_ics, bits, bufsize)) != IcsErr_Ok)
//		goto Error;
*/

    if( (err = IcsSetCompression (new_ics, IcsCompr_gzip, 0)) != IcsErr_Ok)
	    goto Error;

    if( (err = FreeImageIcs_IcsClose (new_ics)) != IcsErr_Ok)
	    goto Error;

	free(bits);
    free(old_bits);
    
	return FREEIMAGE_ALGORITHMS_SUCCESS;

	Error:

    printf("ERROR");

	if(bits)
		free(bits);

    if(old_bits)
		free(old_bits);

	return FREEIMAGE_ALGORITHMS_ERROR;
}


FIBITMAP* DLL_CALLCONV
FreeImageIcs_GetIcsImageDimensionalDataSlice(ICS *ics, int dimension, int slice)
{
	Ics_DataType dataType;
	int ndims;
	size_t dims[ICS_MAXDIM];

    if(dimension < 2)
        return NULL;

	memset(dims, 0, sizeof(size_t) * ICS_MAXDIM);

	// Reset the poiter to the beginning of file.
	if(ics->BlockRead)
		IcsSetIdsBlock (ics, 0, SEEK_SET);

	IcsGetLayout (ics, &dataType, &ndims, dims);

	// Dimension must not be greater than the total dimensions
	if(dimension > ndims)
		return NULL;

	// If the slice is greater than the size for that dimension return error;
	if(slice != 0 && slice >= (int) dims[dimension])
		return NULL;

	size_t bufsize;

	GetSizeInBytesBetweenDimensions(dims, ndims, dataType, dimension, &bufsize);
		
	if(IcsSkipDataBlock  (ics, bufsize * slice) != IcsErr_Ok)
		return NULL;

	BYTE *buf = (BYTE *) malloc (bufsize);

	if (buf == NULL)
   		return NULL;

	if(IcsGetDataBlock  (ics, buf, bufsize) != IcsErr_Ok) {
		free(buf);
		return NULL;
	}

	FIBITMAP *dib =
        FreeImageIcs_CreateFIB(buf, dataType, GetIcsDataTypeBPP (dataType), (int) dims[0], (int) dims[1], 0, 0);
	
	free(buf);

	return dib;
}


static FIBITMAP* DLL_CALLCONV
LoadFIBFromColourIcsFile (ICS *ics, int width, int height)
{
	FIBITMAP *dib = FreeImage_AllocateT(FIT_BITMAP, width, height, 24, 0, 0, 0);
	 
	FIBITMAP *red_fib = FreeImageIcs_GetIcsImageDimensionalDataSlice(ics, 2, 0);
	FIBITMAP *green_fib = FreeImageIcs_GetIcsImageDimensionalDataSlice(ics, 2, 1);
	FIBITMAP *blue_fib = FreeImageIcs_GetIcsImageDimensionalDataSlice(ics, 2, 2);

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

    // If we have labels history then used those instead of order
    // Someone added Labels to history and we are now stuck with 
    // it for backward compatibility.
	FreeImageIcs_GetLabelForDimension (ics, dimension, label);

	*size = dims[dimension];

	return FREEIMAGE_ALGORITHMS_SUCCESS;
}


FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFile (ICS *ics)
{
	FIBITMAP *dib;
	Ics_DataType dt;
	int ndims, channels = 1;
	int dims[ICS_MAXDIM];

	IcsGetLayout (ics, &dt, &ndims, (size_t *) dims);

	if(FreeImageIcs_IsIcsFileColourFile(ics))
		dib = LoadFIBFromColourIcsFile (ics, dims[0], dims[1]);
	else
		dib = FreeImageIcs_GetIcsImageDimensionalDataSlice(ics, 2, 0);
	
	return dib;
}


FIBITMAP* DLL_CALLCONV
FreeImageIcs_LoadFIBFromIcsFilePath (const char* filepath)
{
	ICS *ics;

	IcsOpen(&ics, filepath, "r");

	FIBITMAP *fib = FreeImageIcs_LoadFIBFromIcsFile (ics);

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

int
SaveGreyScaleImage (FIBITMAP *dib, const char *filepath)
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


static int
SaveColourImage (FIBITMAP *dib, const char *filepath)
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
		return SaveGreyScaleImage (dib, filepath);
	else
		return SaveColourImage (dib, filepath);
}

Ics_Error DLL_CALLCONV
FreeImageIcs_IcsOpen (ICS* *ics, char const* filename, char const* mode)
{
    return IcsOpen (ics, filename, mode);
}

Ics_Error DLL_CALLCONV
FreeImageIcs_IcsClose (ICS* ics)
{
    return IcsClose (ics);
}

Ics_Error DLL_CALLCONV
FreeImageIcs_IcsAddHistoryString (ICS* ics, char const* key, char const* value)
{
    return IcsAddHistoryString (ics, key, value);
}

Ics_Error DLL_CALLCONV
FreeImageIcs_IcsNewHistoryIterator (ICS* ics, Ics_HistoryIterator* it, char const* key)
{
    return IcsNewHistoryIterator (ics, it, key);
}

Ics_Error DLL_CALLCONV
FreeImageIcs_IcsGetHistoryStringI (ICS* ics, Ics_HistoryIterator* it, char* string)
{
    return IcsGetHistoryStringI (ics, it, string);
}

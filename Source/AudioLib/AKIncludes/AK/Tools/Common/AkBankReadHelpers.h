/***********************************************************************
  The content of this file includes source code for the sound engine
  portion of the AUDIOKINETIC Wwise Technology and constitutes "Level
  Two Source Code" as defined in the Source Code Addendum attached
  with this file.  Any use of the Level Two Source Code shall be
  subject to the terms and conditions outlined in the Source Code
  Addendum and the End User License Agreement for Wwise(R).

  Version: <VERSION>  Build: <BUILDNUMBER>
  Copyright (c) <COPYRIGHTYEAR> Audiokinetic Inc.
 ***********************************************************************/

#ifndef _AK_BANKREADHELPERS_H_
#define _AK_BANKREADHELPERS_H_
/// Read data from bank and advance pointer.
template< typename T > 
inline T ReadBankData( 
						AkUInt8*& in_rptr 
#ifdef _DEBUG
						,AkUInt32& in_rSize
#endif
						)
{
	T l_Value;
#if defined(AK_IOS) || defined(AK_3DS) || defined(AK_ANDROID)
	typedef struct {T t;} __attribute__((__packed__)) packedStruct;
	l_Value = ((packedStruct *)in_rptr)->t;
#else
	l_Value = *( ( T* )in_rptr );
#endif

	in_rptr += sizeof( T );
#ifdef _DEBUG
	in_rSize -= sizeof( T );
#endif
	return l_Value;
}

inline char * ReadBankStringUtf8( 
						AkUInt8*& in_rptr 
#ifdef _DEBUG
						,AkUInt32& in_rSize
#endif
						,AkUInt32& out_uStringSize )
{
	out_uStringSize = ReadBankData<AkUInt32>( in_rptr 
#ifdef _DEBUG
						,in_rSize
#endif
						);

	char * pString = 0;
	if ( out_uStringSize > 0 )
	{
		pString = reinterpret_cast<char*>( in_rptr );
		in_rptr += out_uStringSize;
#ifdef _DEBUG
		in_rSize -= out_uStringSize;
#endif
	}
	return pString;
}

/// Read unaligned memory, const version
template< typename T >
inline T ReadUnaligned( const AkUInt8* in_rptr, AkUInt32 in_bytesToSkip = 0 )
{
	AkUInt32 size = sizeof(T);
	AkUInt8* ptr = const_cast<AkUInt8*>(in_rptr) + in_bytesToSkip;
	return ReadBankData<T>(ptr
#ifdef _DEBUG
	, size
#endif
	);
}


#ifdef AK_XBOX360
/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal32 AlignFloat( AkReal32* __unaligned ptr )
{
	return *ptr;
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal32 ReadBankData<AkReal32>( 
									   AkUInt8*& in_rptr
#ifdef _DEBUG
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal32 l_Value = AlignFloat( ( AkReal32* )in_rptr );
	in_rptr += sizeof( AkReal32 );
#ifdef _DEBUG
	in_rSize -= sizeof( AkReal32 );
#endif
	return l_Value;
}

/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal64 AlignFloat( AkReal64* __unaligned ptr )
{
	return *ptr;
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal64 ReadBankData<AkReal64>( 
									   AkUInt8*& in_rptr
#ifdef _DEBUG
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal64 l_Value = AlignFloat( ( AkReal64* )in_rptr );
	in_rptr += sizeof( AkReal64 );
#ifdef _DEBUG
	in_rSize -= sizeof( AkReal64 );
#endif
	return l_Value;
}

#endif //AK_XBOX360

#if defined( __SNC__ ) // Valid for both NGP and PS3 (w/SN Compiler)
	/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
	inline AkReal32 AlignFloat( AkReal32 __unaligned * ptr )
	{
		return *ptr;
	}
	
	/// Read data from bank and advance pointer.
	template<> 
	inline AkReal32 ReadBankData<AkReal32>( 
										   AkUInt8*& in_rptr
	#ifdef _DEBUG
										   ,AkUInt32& in_rSize
	#endif
										   )
	{
		AkReal32 l_Value = AlignFloat( ( AkReal32* )in_rptr );
		in_rptr += sizeof( AkReal32 );
	#ifdef _DEBUG
		in_rSize -= sizeof( AkReal32 );
	#endif
		return l_Value;
	}
	
	/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
	inline AkReal64 AlignFloat( AkReal64 __unaligned * ptr )
	{
		return *ptr;
	}
	
	/// Read data from bank and advance pointer.
	template<> 
	inline AkReal64 ReadBankData<AkReal64>( 
										   AkUInt8*& in_rptr
	#ifdef _DEBUG
										   ,AkUInt32& in_rSize
	#endif
										   )
	{
		AkReal64 l_Value = AlignFloat( ( AkReal64* )in_rptr );
		in_rptr += sizeof( AkReal64 );
	#ifdef _DEBUG
		in_rSize -= sizeof( AkReal64 );
	#endif
		return l_Value;
	}

#elif defined (AK_PS3) || defined(AK_WII) || defined (AK_WIIU) || (defined(AK_IOS) && defined(_DEBUG)) // bug with iOS SDK 4.3 in Debug only

/// Type conversion helper on some platforms.
template < typename TO, typename FROM >
inline TO union_cast( FROM value )
{
    union { FROM from; TO to; } convert;
    convert.from = value;
    return convert.to;
}

/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal32 AlignFloat( AkReal32* ptr )
{
	AkUInt32 *puint = reinterpret_cast<AkUInt32 *>( ptr );
    volatile AkUInt32 uint = *puint;
    return union_cast<AkReal32>( uint );
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal32 ReadBankData<AkReal32>( 
									   AkUInt8*& in_rptr
#ifdef _DEBUG
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal32 l_Value = AlignFloat( ( AkReal32* )in_rptr );
	in_rptr += sizeof( AkReal32 );
#ifdef _DEBUG
	in_rSize -= sizeof( AkReal32 );
#endif
	return l_Value;
}

/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal64 AlignFloat( AkReal64* ptr )
{
	AkUInt64 *puint = reinterpret_cast<AkUInt64 *>( ptr );
    volatile AkUInt64 uint = *puint;
    return union_cast<AkReal64>( uint );
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal64 ReadBankData<AkReal64>( 
									   AkUInt8*& in_rptr
#ifdef _DEBUG
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal64 l_Value = AlignFloat( ( AkReal64* )in_rptr );
	in_rptr += sizeof( AkReal64 );
#ifdef _DEBUG
	in_rSize -= sizeof( AkReal64 );
#endif
	return l_Value;
}
#endif // AK_PS3 || AK_WII

#ifdef _DEBUG

/// Read and return bank data of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define READBANKDATA( _Type, _Ptr, _Size )		\
		ReadBankData<_Type>( _Ptr, _Size )

/// Read and return non-null-terminatd UTF-8 string stored in bank, and its size.
#define READBANKSTRING_UTF8( _Ptr, _Size, _out_StringSize )		\
		ReadBankStringUtf8( _Ptr, _Size, _out_StringSize )

/// Skip over some bank data  of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKDATA( _Type, _Ptr, _Size )		\
		( _Ptr ) += sizeof( _Type );	\
		( _Size ) -= sizeof( _Type )

/// Skip over some bank data by a given size in bytes, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKBYTES( _NumBytes, _Ptr, _Size )	\
		( _Ptr ) += _NumBytes;		\
		( _Size ) -= _NumBytes

#else

/// Read and return bank data of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define READBANKDATA( _Type, _Ptr, _Size )		\
		ReadBankData<_Type>( _Ptr )

#define READBANKSTRING_UTF8( _Ptr, _Size, _out_StringSize )		\
		ReadBankStringUtf8( _Ptr, _out_StringSize )

/// Skip over some bank data  of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKDATA( _Type, _Ptr, _Size )		\
		( _Ptr ) += sizeof( _Type )

/// Skip over some bank data by a given size in bytes, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKBYTES( _NumBytes, _Ptr, _Size )	\
		( _Ptr ) += _NumBytes;

#endif

/// Helper macro to determine whether the full content of a block of memory was properly parsed

#ifdef _DEBUG
	#define CHECKBANKDATASIZE( _DATASIZE_, _ERESULT_ )	AKASSERT( _DATASIZE_ == 0 )
#else
	#define CHECKBANKDATASIZE(_DATASIZE_, _ERESULT_ )
#endif

#endif //_AK_BANKREADHELPERS_H_

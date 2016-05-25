#ifndef PACKUTIL_H_
#define PACKUTIL_H_

__declspec(align(16))
struct MainHeader
{
	char	ArchiveSignature[4];
	int		nDirectoryOffSet;
	int		nDirectoryLength;

	MainHeader()
	{
		ArchiveSignature[0] = '\0';

		nDirectoryOffSet = -1;
		nDirectoryLength = -1;
	}
};

struct FileHeader
{
	char	FileName[56];
	int		nFileOffSet;
	int		nFileLength;

	FileHeader()
	{
		FileName[0] = '\0';
		nFileOffSet = -1;
		nFileLength = -1;
	}
};


#endif
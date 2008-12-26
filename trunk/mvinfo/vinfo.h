//////////////////////////////////////////////////////////////////////////////////////////
// Version info - wrapper for version resource of a module. It allows to change 
//				  version info fields (file version, product version, comment, product 
//				  name and so on).
//
//				  Functions like SetFileVersionInfo(...) or VarSetValue(...) are absent
//				  in WinAPI, but some time you need to update version info 
//				  programatically. That is why this classes were developed.
//
//				  Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
#ifndef _H_VINFO
#define _H_VINFO

#include <map>
#include <vector>

namespace vi {

// file attributes
//
enum VIH_FILEATTRIBUTES
{
	VIH_FILEVERSION = 0x30,
	VIH_PRODUCTVERSION = 0x38,
	VIH_FILEFLAGSMASK = 0x40,
	VIH_FILEFLAGS = 0x44,
	VIH_FILEOS = 0x48,
	VIH_FILETYPE = 0x4C
};

// block header fields
//
#define VI_COMMENTS				_L("Comments")
#define VI_COMPANYNAME			_L("CompanyName")
#define VI_FILEDESCRIPTION		_L("FileDescription")
#define VI_FILEVERSION			_L("FileVersion")
#define VI_INTERNALNAME			_L("InternalName")
#define VI_LEGALCOPYRIGHT		_L("LegalCopyright")
#define VI_LEGALTRADEMARKS		_L("LegalTrademarks")
#define VI_ORIGINALFILENAME		_L("OriginalFilename")
#define VI_PRIVATEBUILD			_L("PrivateBuild")
#define VI_PRODUCTNAME			_L("ProductName")
#define VI_PRODUCTVERSION		_L("ProductVersion")
#define VI_SPECIALBUILD			_L("SpecialBuild")

#define SFI_LENGHTINFO 3

// clases names declarations
//

// version info parser
class VIParserSimple;
// simple error handler (you MUST use your own class)
class ErrorHandlingDummy;

// class for user operations
template <class TParser = VIParserSimple, 
		  class TEHandler = ErrorHandlingDummy> class VersionInfo;

// classes incapsulates version info structure
class VIItem;
class VIItemContainer;
class VIHeaderItem;
	class VIHeaderDWORD;
	class VIHeaderVersion;
class VIHeader;

// user type declarations
typedef std::vector<VIItem *> VIVector;

//////////////////////////////////////////////////////////////////////////////////////////
// for example:
//
//	using namespace vi;
//
//	class YourOwnErrorHandler
//	{
//	public:
//		void error(LPCTSTR lpszErrorText) {
//			throw lpszErrorText;
//		}
//	};
//
//	void main(int argc, char* argv[])
//	{
//		VersionInfo<VIParserSimple, YourOwnErrorHandler> vi;
//	
//		try {
//			if(!vi.load(argv[1])) return -1;
//			
//			// change file version info
//			vi[VIH_FILEVERSION] = _L("1, 2, 3, 4");
//
//			// change product name
//			vi[VI_PRODUCTNAME] = _L("Your cool program name");
//
//			VIItem &item = vi[_L("OriginalFilename")];
//			
//			if(item == VIItem::dummy);
//				// the item was not found
//
//			vi.update();
//		}
//		catch(LPCTSTR lpszError)
//		{
//			printf("error: %s", lpszError);
//		}
//	}
//
//	thats is all ! 
//


/////////////////////////////////////////////////////////////////////////////////////////
// class ErrorHandlingDummy - simple error handling strategy
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
class ErrorHandlingDummy
{
public:
	void error(LPCTSTR lpszErrorText)
	{
		printf("error: %s\n", lpszErrorText);
	}
};

BOOL CALLBACK proc(HMODULE hModule, LPCTSTR lpType,
        LPCTSTR lpName, WORD  wLanguage, LONG lParam);

/////////////////////////////////////////////////////////////////////////////////////////
// class VersionInfo - class for user, incapsulates all version info operations
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
template <class TParser = VIParserSimple, class TEHandler = ErrorHandlingDummy> 
class VersionInfo
{
	// version resource handle
	HRSRC hVResource;

protected:
	// a memory parser
	TParser *parser;
	// error handler
	TEHandler eh;	

	// file name
	char lpszFileName_[_MAX_PATH];

public:
	// public constructor has no parameters
	VersionInfo() : parser(NULL) { strnset(lpszFileName_, 0, _MAX_PATH);}
	// desctructor
	virtual ~VersionInfo() { 
		if(parser) delete parser; 
	}

	// loads file version resource and parse it
	virtual bool load(LPCSTR lpszFileName)
	{
		strcpy(lpszFileName_, lpszFileName);	// store file name

		// load module that contains version resource
		HMODULE hModule = ::LoadLibrary(lpszFileName_);

		if(!hModule) {
			eh.error("Can't load module");
			return false;
		}

		BOOL bResult = ::EnumResourceLanguages(hModule, RT_VERSION, MAKEINTRESOURCE(1),
			proc, (LONG) this);

		hVResource = ::FindResourceEx(hModule, MAKEINTRESOURCE(1), RT_VERSION, 0x0409);

		// get version resource handle
		hVResource = ::FindResource(hModule, MAKEINTRESOURCE(1), RT_VERSION);

		if(!hVResource) {
			eh.error("Can't find version resource");
			return false;
		}
	
		// check in version resource in global memory
		HGLOBAL hGlobal = ::LoadResource(hModule, hVResource);

		if(!hGlobal) {
			eh.error("Can't load version resource");
			return false;
		}

		DWORD dwSize = ::SizeofResource(hModule, hVResource);

		// get the pointer to the version resource binary data
		LPVOID lpData = ::LockResource(hGlobal);

		// create resource parser
		parser = new TParser((LPWSTR) lpData);

		UnlockResource(hGlobal);

		// free resources
		::FreeResource(hGlobal);
		::FreeLibrary(hModule);

		return true;
	}

	// updates file version info
	virtual bool update()
	{
		if(!hVResource) {
			eh.error("Class is not initialized");
			return false;
		}
		
		// tell OS we wanna to update version resource
		HANDLE hUpdate = ::BeginUpdateResource(lpszFileName_, false);

		if(!hUpdate) {
			eh.error("Can't begin update resource");
			return false;
		}

		// get pointer to the memory block where version info places
		LPCWSTR lpBuffer = parser->serialize();

		// update resource
		BOOL bResult = ::UpdateResource(hUpdate, RT_VERSION,  MAKEINTRESOURCE(1),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPVOID) lpBuffer, parser->getSize());

		if(!bResult) {
			eh.error("Can't update version resource");
			return false;
		}

		// end update resources
		bResult = ::EndUpdateResource(hUpdate, false);

		if(!bResult) {
			eh.error("Can't save changes after update");
			return false;
		}

		// it's ok
		return true;
	}

	VIItem& operator[](LPCWSTR lpszPath)
	{
		if(!parser) return VIItem::dummy;

		return (*parser)[lpszPath];
	}

	VIHeaderItem& operator[] (const enum VIH_FILEATTRIBUTES &fa)
	{
		if(!parser) return VIHeaderItem::dummy;

		return (*parser)[fa];
	}
};

/////////////////////////////////////////////////////////////////////////////////////////
// class VersionInfoBinary - class for user, incapsulates all version info operations
//							 with binary file
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
template <class TParser = VIParserSimple, class TEHandler = ErrorHandlingDummy> 
class VersionInfoBinary : public VersionInfo<TParser, TEHandler>
{
	HANDLE m_hFile;
	HANDLE m_hFileMap;
	LPVOID m_lpView;
	LPWSTR m_lpData;

public:
	VersionInfoBinary() : VersionInfo<TParser, TEHandler>(), 
		m_hFile(NULL), m_hFileMap(NULL), m_lpView(NULL), m_lpData(NULL) {;}

	virtual ~VersionInfoBinary()
	{
		// close handles
		if(m_lpView) UnmapViewOfFile(m_lpView);
		if(m_hFileMap) CloseHandle(m_hFileMap);
		if(m_hFile) CloseHandle(m_hFile);
	}

	// overload loading function
	virtual bool load(LPCSTR lpszFileName)
	{
		m_hFile = ::CreateFile(lpszFileName, 
			GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if(!m_hFile) {
			eh.error("Can't open file");
			return false;
		}
		
		// obtain whole file size
		DWORD dwSizeHight= 0;
		DWORD dwSizeLow = ::GetFileSize(m_hFile, &dwSizeHight);

		// maps file into the memory
		m_hFileMap = ::CreateFileMapping(m_hFile, NULL, PAGE_READWRITE,
			dwSizeHight, dwSizeLow, NULL);

		if(!m_hFileMap) {
			eh.error("Can't map file into the memory");
			return false;
		}

		m_lpView = ::MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);


		// find the beggining of the version resource
		LPWSTR lpChar = (LPWSTR) m_lpView;
		DWORD dwSizeInWCHAR = dwSizeLow / sizeof(wchar_t);

		while(lpChar - (LPWSTR) m_lpView < dwSizeInWCHAR)
		{
			while(*lpChar == 0x0000) ++lpChar;

			if(wcsncmp(lpChar, _L("VS_"), 3) == 0) {
				if(wcsncmp(lpChar, _L("VS_VERSION_INFO"), 15) == 0) break;
			}
			else
				lpChar++;
		}

		if(lpChar >= (LPWSTR) m_lpView + dwSizeInWCHAR) {
			eh.error("The file doesn't contain version info.");
			return false;
		}
			
		m_lpData = lpChar - 3;

		// create resource parser
		parser = new TParser(m_lpData);

		return true;
	}		

	virtual bool update()
	{
		if(!parser) {
			eh.error("First you must call Load function.");
			return false;
		}

		LPCWSTR lpBuffer = parser->serialize();

		size_t t = parser->getSize();

		try {
			// copy new version info resource into the file
			memcpy(m_lpData, lpBuffer, parser->getSize());
		}
		catch(...) 
		{
			eh.error("Can't update resource info: access violation.");
			return false;
		}
	
		// obtain whole file size
		DWORD dwSizeHight= 0;
		DWORD dwWritten = 0;
		DWORD dwSizeLow = ::GetFileSize(m_hFile, &dwSizeHight);

		// flushes data
		FlushViewOfFile(m_lpView, dwSizeLow);

		return true;
	}		
};

////////////////////////////////////////////////////////////////////////////////////////////////
// class VIHeaderItem - abstract item value of the version info header
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
class VIHeaderItem
{
protected:
	std::vector<wchar_t> *dumb;
	std::vector<wchar_t>::iterator itb_;

	// private constructor for dummy object
	VIHeaderItem(void) {
		dumb = new std::vector<wchar_t>;

		for(int i = 0; i < 4; i++) dumb->push_back(0x0000);

		itb_ = dumb->begin();
	}
public:
	// public constructor gets iterator that points to the beggining of a data block
	VIHeaderItem(std::vector<wchar_t>::iterator itb) : itb_(itb), dumb(NULL) {;}

	virtual ~VIHeaderItem() { if(dumb) delete dumb;	}

	// assign DWORD value to the data block
	virtual VIHeaderItem& operator=(const DWORD& dw) { return *this; }
	// assign operator
	virtual VIHeaderItem& operator=(const LPCWSTR& lpszvinfo) { return *this; }

	// type conversion data block to the DWORD value
	virtual operator DWORD() { return 0; }
	// type conversion operator
	virtual operator LPCWSTR() { return _L(""); }

private:
	// no assign capability
	VIHeaderItem& operator=(const VIHeaderItem&) {;}
	// no copy constructor
	VIHeaderItem(const VIHeaderItem&) {;}

public:
	static VIHeaderItem dummy;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// class VIHeaderDWORD - item value (DWORD) of the version info header
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
class VIHeaderDWORD : public VIHeaderItem
{
public:
	// public constructor gets iterator that points to the beggining of a data block
	VIHeaderDWORD(std::vector<wchar_t>::iterator itb) : VIHeaderItem(itb) {;}

	// assign DWORD value to the data block
	virtual VIHeaderItem& operator=(const DWORD& dw) 
		{ *itb_ = HIWORD(dw); *(itb_ + 1) = LOWORD(dw); return *this;}

	// type conversion data block to the DWORD value
	virtual operator DWORD() { return MAKELONG(*itb_, *(itb_ + 1)); }
};


////////////////////////////////////////////////////////////////////////////////////////////////
// class VIHeaderVersion - version item value (double DWORD) of the version info header
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
const int MAX_DIGITS = 11;	// maximal amount of digits for DWORD variable;

class VIHeaderVersion : public VIHeaderItem
{
	LPWSTR pBuffer;	// internal buffer for return in operator LPCWSTR()

public:
	// public constructor gets iterator that points to the beggining of a data block
	VIHeaderVersion(std::vector<wchar_t>::iterator itb) : 
		VIHeaderItem(itb), pBuffer(NULL) {;}

	virtual ~VIHeaderVersion() { if(pBuffer) delete pBuffer; }

	// assign operator
	virtual VIHeaderItem& operator=(const LPCWSTR& lpszvinfo)
	{
		LPCWSTR lppl = lpszvinfo, lpprev = lppl, 
				lpend = lpszvinfo + wcslen(lpszvinfo);

		wchar_t num[MAX_DIGITS] = {0};

		// for string to dword transformation
		int position[] = {1, 0, 3, 2}, i = 0;

		// scan source string for ',' || '.'
		while(lppl < lpend) {

			while(*lppl != TCHAR(',') && *lppl++ != TCHAR('.') && lppl < lpend);

			// clear long value buffer
			wcsset(num, 0x0000);
			// cut value from source string
			wcsncpy(num, lpprev, lppl - lpprev);

			// assign value
			if(i < 4) *(itb_ + position[i++]) = _wtol(num);

			// go to the next item
			lpprev = ++lppl;
		}

		return *this;
	}

	// type conversion operator
	virtual operator LPCWSTR() {
		if(pBuffer == NULL) {
			pBuffer = new wchar_t[_MAX_PATH];
		}

		::wcsnset(pBuffer, 0x0000, _MAX_PATH);

		wchar_t num[MAX_DIGITS] = {0};

		_ltow((*(itb_ + 1)), num, 10);

		wcscat(pBuffer, num);
		wcscat(pBuffer, _L(", "));

		wcsset(num, 0x0000);
		_ltow((*itb_), num, 10);

		wcscat(pBuffer, num);
		wcscat(pBuffer, _L(", "));

		wcsset(num, 0x0000);
		_ltow((*(itb_ + 3)), num, 10);

		wcscat(pBuffer, num);
		wcscat(pBuffer, _L(", "));

		wcsset(num, 0x0000);
		_ltow((*(itb_ + 2)), num, 10);

		wcscat(pBuffer, num);

		return pBuffer;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////
// class VIHeader - version info header
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
class VIHeader
{
	std::vector<wchar_t> header;
	std::map<UINT, VIHeaderItem *> fileatt_;

public:
	// public constructor
	VIHeader(LPCWSTR pBuffer, size_t sizeBytes = 92)
	{
		// parse memory
		for(int i = 0; i < sizeBytes / 2; i++) {
			header.push_back(*pBuffer++);
		}

		fileatt_[VIH_FILEVERSION] = 
			new VIHeaderVersion(header.begin() + VIH_FILEVERSION / 2);
		fileatt_[VIH_PRODUCTVERSION] = 
			new VIHeaderVersion(header.begin() + VIH_PRODUCTVERSION / 2);
		fileatt_[VIH_FILEFLAGSMASK] = 
			new VIHeaderItem(header.begin() + VIH_FILEFLAGSMASK / 2);
		fileatt_[VIH_FILEFLAGS] = 
			new VIHeaderItem(header.begin() + VIH_FILEFLAGS / 2);
		fileatt_[VIH_FILEOS] = 
			new VIHeaderItem(header.begin() + VIH_FILEOS / 2);
		fileatt_[VIH_FILETYPE] = 
			new VIHeaderItem(header.begin() + VIH_FILETYPE / 2);
	}

	// descructor free maps
	~VIHeader()
	{
		for(std::map<UINT, VIHeaderItem *>::iterator it = fileatt_.begin();
			it != fileatt_.end(); ++it)	delete it->second;
	}

	// store header into the memory
	LPWSTR serialize(LPWSTR pBuffer)
	{
		memcpy(pBuffer, &header[0], getSize());

		return pBuffer + header.size();
	}

	// size of the header
	size_t getSize()
	{
		return header.size() * sizeof(header[0]);
	}

	// an access for header items 
	VIHeaderItem& operator[] (const enum VIH_FILEATTRIBUTES &fa)
	{
		return *(fileatt_.find(fa)->second);
	}

private:
	// no assign capability
	VIHeader& operator=(const VIHeader&) {;}
	// no copy constructor
	VIHeader(const VIHeader&) {;}
};


////////////////////////////////////////////////////////////////////////////////////////////////
// class VIItem - base class for version info item
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
class VIItem
{
protected:
	DWORD length_;	// size of item in wchar_t
	DWORD type_;	// type of item

public:
	DWORD size_;	// size of item in bytes (get it throw getSize())
	std::vector<wchar_t> name, value, tag;

	// default constructor
	VIItem(void) : size_(0), type_(0), length_(0) {;}

	// constructor, parse memory
	VIItem(LPCWSTR pBuffer) : size_(0), type_(0), length_(0)
	{
		size_ = *pBuffer;
		length_ = *(pBuffer + 1);
		type_ = *(pBuffer + 2);

		LPCWSTR lpStart = pBuffer;

		// name is placed after header
		pBuffer += SFI_LENGHTINFO;

		// add name
		while(*pBuffer != 0x0000) {
			name.push_back(*pBuffer++);
		}

		// add terminated nulls after name
		while(*pBuffer == 0x0000) {
			name.push_back(*pBuffer++);
		}

		if(length_> 0) // item has a value
		{
			int i = 0;

			if(type_ == 1)
			{
				// add value
				while(i++ < length_) {
					value.push_back(*pBuffer++);
				}

				while(*pBuffer == 0x0000) {
					tag.push_back(*pBuffer++);
				}
			}
			else
			{
				// add value
				while(i++ < length_ / sizeof(wchar_t)) {
					value.push_back(*pBuffer++);
				}
			}
		}
	}

	// virtual destructor
	virtual ~VIItem() {;}

	// store item info in memory
	virtual LPWSTR serialize(LPWSTR pBuffer) 
	{
		if(!pBuffer) return pBuffer;

		*pBuffer = getSize();

		if(type_ == 0) 
			*(pBuffer + 1) = getLength() * sizeof(wchar_t);
		else
			*(pBuffer + 1) = getLength();

		*(pBuffer + 2) = type_;

		pBuffer += SFI_LENGHTINFO;

		// store item name
		memcpy(pBuffer, &name[0], name.size() * sizeof(name[0]));

		pBuffer += name.size();
		
		// store item value if exists
		if(getLength() > 0)
		{
			// store item value
			memcpy(pBuffer, &value[0], value.size() * sizeof(value[0]));

			pBuffer += value.size();

			// store tag
			if(tag.size() > 0) 
			{
				memcpy(pBuffer, &tag[0], tag.size() * sizeof(tag[0]));

				pBuffer += tag.size();
			}
		}
			
		return pBuffer;
	}

	// return in bytes size of item only
	virtual size_t getItemSize() { 
		return (SFI_LENGHTINFO + name.size() + getLength()) * sizeof(wchar_t);
	}

	// return size in bytes
	virtual size_t getSize() { 
		return getItemSize() + tag.size() * sizeof(wchar_t);
	}

	// return actual length
	virtual size_t getLength() { return value.size(); }

	// find item by value
	virtual VIItem& operator[](LPCWSTR lpszPath)
	{
		if(wcscmp(&name[0], lpszPath) == 0)
			return *this;
		else
			return VIItem::dummy;
	}

	// comaprision operator
	bool operator==(const VIItem& op)
	{
		return size_ == op.size_ && type_ == op.type_ && length_ == op.length_;
	}

	// assign string to item value
	VIItem& operator=(const LPCWSTR nv)
	{
		int length = wcslen(nv);

		value.clear();

		if(length == 0) {
			return *this;
		}
		else
		{
			for(int i = 0; i < length; i++) value.push_back(nv[i]);

			tag.clear();

			if(value.size() % 2 == 0) {
				tag.push_back(0);
			}

			value.push_back(0);
		}

		return *this;
	}

	operator LPCWSTR() { return &this->value[0]; }

private:
	// no assign capability
	VIItem& operator=(const VIItem&) {;}
	// no copy constructor
	VIItem(const VIItem&) {;}

public:
	static VIItem dummy;
};

/////////////////////////////////////////////////////////////////////////////////////////
// class VIItemContainer - derived class for collection of items
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
class VIItemContainer : public VIItem
{
	VIVector items_;

public:
	// constructor
	VIItemContainer(LPCWSTR pBuffer) : VIItem(pBuffer)
	{
		// calc offset for the next item
		LPWSTR lpMark = (LPWSTR) pBuffer + name.size() + SFI_LENGHTINFO;

		int bytesReaded = 0;

		// calc item value block size
		int maxLength = size_ - (name.size() + SFI_LENGHTINFO) * sizeof(wchar_t);

		VIItem *pItem = NULL;

		// go throw subitems
		while(bytesReaded < maxLength)
		{
			// check next subitem parameters
			VIItem vitmp(lpMark);

 			if(vitmp.getLength() < 1 && vitmp.size_ > vitmp.getSize() + 2) {
				// container
				pItem = new VIItemContainer(lpMark);
			}
			else {
				// item
				pItem = new VIItem(lpMark);
			}

			items_.push_back(pItem);

			// increase iterator
			bytesReaded += pItem->getSize();

			// increase buffer pointer value
			lpMark += pItem->getSize() / sizeof(wchar_t);
		}
	}

	virtual ~VIItemContainer() 
	{
		// free items
		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
			delete (*it);
	}

	// return actual size of the item (size may be changed because of value of child items )
	virtual size_t getSize() 
	{
		size_t size = VIItem::getSize();

		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
		{
			size += (*it)->getSize();
		}

		return size;
	}

	// store item info in memory
	virtual LPWSTR serialize(LPWSTR pBuffer)
	{
		*pBuffer = getSize();
		*(pBuffer + 1) = getLength();
		*(pBuffer + 2) = type_;

		pBuffer += SFI_LENGHTINFO;

		// store item name
		memcpy(pBuffer, &name[0], name.size() * sizeof(name[0]));

		pBuffer += name.size();
		
		if(!pBuffer) return pBuffer;

		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
		{
			pBuffer = (*it)->serialize(pBuffer);
		}

		return pBuffer;
	}

	// find item by value
	virtual VIItem& operator[](LPCWSTR lpszPath)
	{
		int pos = -1, length = wcslen(lpszPath);
		
		while(++pos < length && lpszPath[pos] != TCHAR('\\') && lpszPath[pos] != TCHAR('/'));

		LPCWSTR lpszItem = NULL;

		if(pos < 0 || pos >= length)
			lpszItem = lpszPath;
		else
			lpszItem = &(lpszPath[pos + 1]);

		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
		{
			if((*(*it))[lpszItem] == VIItem::dummy)
				continue;
			else
				return (*(*it))[lpszItem];
		}

		return VIItem::dummy;
	}
};


/////////////////////////////////////////////////////////////////////////////////////////
// class VIParserSimple - simple method (strategy) of parsing version resource.
//						   this class is about to be an a template parameter
//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
class VIParserSimple
{
	// version info header
	VIHeader *header_;
	// version info item collections (StringFileInfo and VarFileInfo)
	VIVector items_;

	// result version info buffer
	LPWSTR pOutBuffer;

public:
	// public constructor
	VIParserSimple(LPWSTR pBuffer) : pOutBuffer(NULL), header_(NULL)
	{
		LPWSTR lpOffset = pBuffer;

		// version info header
		header_ = new VIHeader(lpOffset);

		lpOffset += header_->getSize() / sizeof(wchar_t);

		// stringfileinfo block and varfileinfo block
		VIItem *pItem;

		for(int i = 0; i < 2; i++)
		{
			pItem = new VIItemContainer(lpOffset);

			lpOffset += pItem->getSize() / sizeof(wchar_t);

			items_.push_back(pItem);
		}
	}

	~VIParserSimple()	
	{
		if(pOutBuffer)	delete[] pOutBuffer;
		if(header_) delete header_;

		// free items
		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
			delete (*it);
	}

	LPCWSTR serialize()
	{
		// size of version info resource
		size_t size = getSize();

		if(pOutBuffer) delete[] pOutBuffer;

		// allocate memory block
		pOutBuffer = new wchar_t[size];
		// fill memory block with nulls
		memset(pOutBuffer, 0, size);

		LPWSTR pBuffer = pOutBuffer;

		// serialize version info header
		pBuffer = header_->serialize(pBuffer);

		// serialize version info items
		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
			pBuffer = (*it)->serialize(pBuffer);

		// write size of resource
		*pOutBuffer = size;

		return pOutBuffer;
	}

	// return size of the version info resource
	size_t getSize()
	{
		if(!header_) return 0;

		size_t size = header_->getSize();

		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
			size += (*it)->getSize();

		return size;
	}

	// get strinfileinfo item
	VIItem& operator[](LPCWSTR lpszPath)
	{
		for(VIVector::iterator it = items_.begin(); it != items_.end(); ++it)
		{
			if((*(*it))[lpszPath] == VIItem::dummy)
				continue;
			else
				return (*(*it))[lpszPath];
		}

		return VIItem::dummy;
	}

	// get header item
	VIHeaderItem& operator[] (const enum VIH_FILEATTRIBUTES &fa)
	{
		if(!header_) return VIHeaderItem::dummy;

		return (*header_)[fa];
	}

private:
	// no assign capability
	VIParserSimple& operator=(const VIParserSimple&) {;}
	// no copy constructor
	VIParserSimple(const VIParserSimple&) {;}
};

}; // namespace

// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
#endif
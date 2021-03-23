#pragma once

#include <string>
 
#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

namespace SysUtils {

class Path
{
public:
	Path(void);
	~Path(void);
 
	static tstring GetAppPath();
	static tstring GetCurDir();
	static tstring GetTempPath();
	static tstring GetTempFileName(LPCTSTR lpszFileName);
	static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);
 
	static BOOL IsDirectory(LPCTSTR lpszPath);
	static BOOL FileIsExists(LPCTSTR lpszFileName);
	static BOOL DirectoryIsExists(LPCTSTR lpszPath);
	static BOOL CreateDirectory(LPCTSTR lpszPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
 
	static tstring GetPathRoot(LPCTSTR lpszPath);
	static tstring GetDirectoryName(LPCTSTR lpszPath);
	static tstring GetFileName(LPCTSTR lpszPath);
	static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);
	static tstring GetExtension(LPCTSTR lpszPath);
	static tstring GetFullPath(LPCTSTR lpszPath);
};

} // namespace SysUtils

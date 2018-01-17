#pragma once

#ifndef _WIDESTRING_HELPER_H
#define _WIDESTRING_HELPER_H

#include <Shlwapi.h>
#include <iostream>
#include <vector>
using namespace std;

class CWideStringHelper
{
public:
	CWideStringHelper() {}
	~CWideStringHelper() {}

	// ============================================================================================================================================
	// Given a resource filename (model, texture, shader), this method returns the full path name of where the resource should be
	// within the /Resources/MEDIA/ folder.
	// This method also checks for the file existence. If the resource is not found, this method returns false
	//
	// Documentation for GetModuleFilename:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
	// Documentation for GetCurrentDirectory:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa364934(v=vs.85).aspx
	// Documentation for fopen_s and _wfopen_s
	// https://msdn.microsoft.com/es-mx/library/z5hh6ee9.aspx
	// Documentation for MultiByteToWideChar
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072(v=vs.85).aspx
	// Documentation for WideCharToMultiByte
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130(v=vs.85).aspx
	// ============================================================================================================================================
	static bool GetResourceFullPath(const char *resourceFileName, std::wstring &resourceFullPathUTF16, std::string &resourceFullPath)
	{
		bool resourceFound = false;
		FILE *filePtr;

		// ----------------------------------------------------------------------------------
		// UNICODE 
		// ----------------------------------------------------------------------------------
		// Check if UNICODE is defined in the project properties:
		//   Configuration Properties --> General --> Project Defaults --> Character Set
#ifndef UNICODE

		// Get the full path of the current module (the .EXE that's being executed)
		vector<wchar_t> pathBuf;
		DWORD copied = 0;

		do {
			pathBuf.resize(pathBuf.size() + MAX_PATH);
			copied = GetModuleFileNameW(0, &pathBuf.at(0), (DWORD)pathBuf.size());
		} while (copied >= pathBuf.size());

		// Remove the filename of the .exe, to leave only the full path, without the filename
		PathRemoveFileSpecW(&pathBuf.at(0));

		//std::wstring path(pathBuf.begin(), pathBuf.end());
		std::wstring path(&pathBuf[0]);

		// Copy the resulting path to the output param
		resourceFullPathUTF16.append(path);

		// Convert the resource name from char* to wchar_t
		std::string tempResource(resourceFileName);
		std::wstring wresource = UTF8_to_UTF16(tempResource);

		// Now append the relative path where the resources are located
		resourceFullPathUTF16.append(L"\\..\\..\\..\\..\\Resources\\MEDIA\\");

		// Append the filename
		resourceFullPathUTF16.append(wresource);

		// Finally, check for the file existence
		errno_t err = _wfopen_s(&filePtr, &resourceFullPathUTF16[0], L"r, ccs=UTF-16LE");

		if (err == 0)
		{
			resourceFound = true;

			// We need to return both UTF8 and UTF16
			std::string resource = UTF16_to_UTF8(resourceFullPathUTF16);
			resourceFullPath.append(resource);
		}

		if (filePtr != nullptr)
		{
			fclose(filePtr);
		}

#else
		// ----------------------------------------------------------------------------------
		// NOT-UNICODE 
		// ----------------------------------------------------------------------------------

		// Get the full path of the current module (the .EXE that's being executed)
		char *workingDirectory = new char[MAX_PATH + 1];
		GetModuleFileNameA(NULL, workingDirectory, MAX_PATH + 1);

		// Remove the filename of the .exe, to leave only the full path, without the filename
		PathRemoveFileSpecA(workingDirectory);

		// Copy the resulting path to the output param
		resourceFullPath.append(workingDirectory);
		delete[] workingDirectory;

		// Now append the relative path where the resources are located
		resourceFullPath.append("\\..\\..\\..\\..\\Resources\\MEDIA\\");

		// Append the filename
		resourceFullPath.append(resourceFileName);

		// Finally, check for the file existence
		errno_t err = fopen_s(&filePtr, resourceFullPath.c_str(), "r, ccs=UTF-8");

		if (err == 0)
		{
			resourceFound = true;

			// We need to return both UTF8 and UTF16
			std::wstring wresource = UTF8_to_UTF16(resourceFullPath);
			resourceFullPathUTF16.append(wresource);
		}

		if (filePtr != nullptr)
		{
			fclose(filePtr);
		}
#endif

		return resourceFound;
	}

	// ============================================================================================================================================
	// Convert a multi-byte string to UTF16
	// ============================================================================================================================================
	static wstring UTF8_to_UTF16(const std::string &str)
	{
		if (str.empty())
		{
			return wstring();
		}

		size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0);

		if (charsNeeded == 0)
		{
			cout << "Failed converting UTF-8 string to UTF-16" << endl;
			return wstring();
		}

		vector<wchar_t> buffer(charsNeeded);
		int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &buffer[0], (int)buffer.size());

		if (charsConverted == 0)
		{
			cout << "Failed converting UTF-8 string to UTF-16" << endl;
			return wstring();
		}

		return wstring(&buffer[0], charsConverted);
	}

	// ============================================================================================================================================
	// Convert a UTF16 string to UTF8
	// ============================================================================================================================================
	static string UTF16_to_UTF8(const std::wstring &str)
	{
		// HOMEWORK !!!
		return string();
	}
};

#endif // !_WIDESTRING_HELPER_H

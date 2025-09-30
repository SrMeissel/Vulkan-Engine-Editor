#include "Utils.h"
#include <string>

HRESULT selectFile(std::string& fileName) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); // init COM
	//if (FAILED(hr)) return hr;
	CComPtr<IFileOpenDialog> pFileOpen; // COM Smart Pointer
	hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpen));
	//if (FAILED(hr)) return hr;
	hr = pFileOpen->Show(NULL);
	//if (FAILED(hr)) return hr;
	IShellItem* pItem;
	hr = pFileOpen->GetResult(&pItem);
	if (SUCCEEDED(hr))
	{
		PWSTR pszFilePath;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
		fileName.resize(size_needed);
		WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &fileName[0], size_needed, NULL, NULL);
		CoTaskMemFree(pszFilePath);
		std::wstring stemp = std::wstring(fileName.begin(), fileName.end());
		//MessageBox(NULL, L"engine", stemp.c_str(), MB_OK);
	}
	pItem->Release();
	CoUninitialize();
	return hr;
}
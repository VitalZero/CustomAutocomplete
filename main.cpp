// scratch.cpp
#include <windows.h>
#include <ole2.h>
#include <windowsx.h>
#include <shlobj.h>
#include <atlbase.h>
#include <atlcom.h>
CComModule _Module;

typedef CComEnum<IEnumString,
                 &IID_IEnumString,
                 LPOLESTR,
                 _Copy<LPOLESTR> > CComEnumString;

// LPOLESTR = OLECHAR* = WCHAR* = wchar_t*
LPOLESTR c_rgpszSuggestions[] = {
    L"Brave Horse Tavern",
    L"Cuoco",
    L"Dahlia Bakery",
    L"Dahlia Lounge",
    L"Etta's",
    L"Lola",
    L"Palace Kitchen",
    L"Seatown",
    L"Serious Pie",
    L"Ting Momo",
};

void OnInitDialog(HWND hdlg)
{
  CComPtr<IAutoComplete2> autoComplete2;
  CComObject<CComEnumString> *comEnumStr;

  HRESULT hr = CComObject<CComEnumString>::CreateInstance(&comEnumStr);

  CComPtr<IEnumString> stringEnumerator(comEnumStr);
  if (SUCCEEDED(hr) &&
      SUCCEEDED(comEnumStr->Init(&c_rgpszSuggestions[0],
                          &c_rgpszSuggestions[ARRAYSIZE(c_rgpszSuggestions)],
                          NULL)) &&
      SUCCEEDED(autoComplete2.CoCreateInstance(CLSID_AutoComplete)) &&
      SUCCEEDED(autoComplete2->Init(GetDlgItem(hdlg, 100), stringEnumerator, NULL, NULL)) &&
      SUCCEEDED(autoComplete2->SetOptions(ACO_AUTOSUGGEST | ACO_AUTOAPPEND | ACO_UPDOWNKEYDROPSLIST))) 
      {
      }
}

INT_PTR CALLBACK DlgProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
  case WM_INITDIALOG:
    OnInitDialog(hdlg);
    return TRUE;
  case WM_COMMAND:
    switch (GET_WM_COMMAND_ID(wParam, lParam)) {
    case IDOK:
      EndDialog(hdlg, 0);
      break;
    }
  }
  return FALSE;
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev,
                   LPSTR lpCmdLine, int nShowCmd)
{
  if (SUCCEEDED(CoInitialize(NULL))) 
  {
    DialogBox(hinst, MAKEINTRESOURCE(1), NULL, DlgProc);
    CoUninitialize();
  }
  return 0;
}
// IMESample.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include <string>


// Global Variables:
HINSTANCE hInst;                                // current instance
std::wstring szTitle = L"IMESample2";
std::wstring szWindowClass = L"IME.Sample.WindowClass";


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName  = szWindowClass.c_str();	

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//.

HWND hWndWindowq;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWndWindowq = CreateWindowW(szWindowClass.c_str(), szTitle.c_str(), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWndWindowq)
   {
      return FALSE;
   }

   ShowWindow(hWndWindowq, nCmdShow);
   UpdateWindow(hWndWindowq);

   return TRUE;
}

#include "TextServiceManager.h"


TextServiceManager mgr;

class EditBox : public ISinkObserver
{
public:

	EditBox(int sx, int sy, int ex, int ey)
	{
		move(sx,sy,ex,ey);
		selection = false;
	}

	void move(int sx, int sy, int ex, int ey)
	{
		::SetRect(&rect, sx, sy, ex, ey);
		// selection = false;
	}

	void onChar(wchar_t chars)
	{
		text += chars;
		mgr.getTextSink().updateBuffer();
		InvalidateRect(hWndWindowq, 0, 1);
	}

	void keydown(int vkey)
	{
		if (text.empty()) return;

		if (vkey == 8)
		{
			text = text.substr(0, text.size() - 1);
			mgr.getTextSink().updateBuffer();
		}
		InvalidateRect(hWndWindowq, 0, 1);
	}

	void render(HDC dc)
	{
		// WINGDIAPI COLORREF WINAPI SetDCBrushColor(_In_ HDC hdc, _In_ COLORREF color);
		// WINGDIAPI COLORREF WINAPI SetDCPenColor(_In_ HDC hdc, _In_ COLORREF color);
		//RECT rc;
		//::SetRect(&rc, 150, 140, 450, 500);
		auto oldPen = GetDCPenColor(dc);
		auto oldTextCol = GetTextColor(dc);

		SetDCPenColor(dc, RGB(255, 0, 255));    // red
		Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);

		SetTextColor(dc, 0x000000);
		// ::DrawText(dc, text.c_str(), (LONG)text.size(), &rect, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);
		TextOut(dc, rect.left, rect.top, text.c_str(), (LONG)text.size());

		SetTextColor(dc, oldTextCol);
		SetDCPenColor(dc, oldPen);    // red


		int xpos = 0;

		// Render cursor
		if (selection)
		{
			/* Show Cursor */
			SetBkMode(dc, TRANSPARENT);
			//SetBkColor(dc, 0xFFFFFF);
			//SetTextColor(dc, 0x000000);

			SIZE ss;
			GetTextExtentPoint32(dc, text.c_str(), (LONG)text.size(), &ss);

			TextOut(dc, rect.left + ss.cx - 2, rect.top, L"|", 1);


			xpos = rect.left + ss.cx - 2;

			SetBkMode(dc, OPAQUE);
		}

		LONG y = 100;

		// Draw UP arrow

		int sizeOfBiggest = 0;
		for(int cindx = 1; cindx <= _candidates.size();++cindx)
		{
			auto i = _candidates[cindx-1];

			if (cindx == _selectedCandidate+1)
			{
				SetBkColor(dc, 0x00FFFF);
				//SetTextColor(hdc, 0xFFFFFF);
			}
			else
			{
				SetBkColor(dc, 0xFFFFFF);
				//SetTextColor(hdc, 0x0);
			}

			std::wstring wsCandidate(i);

			if (cindx > _beg && cindx <= _end /*&& _selectedCandidate >=0*/ )
			{
				wsCandidate = std::to_wstring(cindx- _beg) + L" " + wsCandidate;
			}

			TextOut(dc, xpos, y, wsCandidate.c_str(), wsCandidate.size());



			SIZE ss;
			GetTextExtentPoint32(dc, wsCandidate.c_str(), (LONG)wsCandidate.size(), &ss);
			if (ss.cx > sizeOfBiggest) sizeOfBiggest = ss.cx;

			y += 15;

			if (cindx % 20 == 0)
			{
				xpos += sizeOfBiggest+20;
				y = 100;

				sizeOfBiggest = 0;
			}
		}
		// Draw Down arrow
	}


	bool canSelect(int x, int y)
	{
		if (x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom) return true;
		return false;
	}

	void select(bool sel)
	{
		selection = sel;
	}

private:

	virtual void			onCandidateListChanged() override
	{
		ITextSink&	textSink = mgr.getTextSink();
		ImeInfo		info = textSink.getImeInfo();

		auto candidates = info.candidateList;
		int selection = info.selectedCandidate;

		if (!candidates.empty() && candidates[candidates.size() - 1] == L"[Try cloud candidate]") candidates.erase(candidates.end() - 1);
		if (!candidates.empty() && candidates[candidates.size() - 1] == L"[Not available]") candidates.erase(candidates.end() - 1);

		_candidates = candidates;
		_selectedCandidate = selection;

		if (selection == 0)
		{
			_beg = 0;
			_end = 9;
		}
		else if(selection == -1)
		{
			_beg = 0;
			_end = 9;
		}


		if(_selectedCandidate >= _end)
		{
			_beg += 9;
			_end += 9;
		}

		if (_selectedCandidate < _beg)
		{
			_beg -= 9;
			_end -= 9;
		}

//		_visibileBeg = 0;
	//	_visibileEnd = 0;


		InvalidateRect(hWndWindowq, 0, 1);
		//setCandidateList(candidates, selection);
	}

	virtual void			onReadingStringChanged()  override
	{
		ITextSink&	textSink = mgr.getTextSink();
		ImeInfo		info = textSink.getImeInfo();

		text = info.composition;
		InvalidateRect(hWndWindowq, 0, 1);
	}

	virtual void			onCompositionChanged() override
	{
	}

	virtual void			onInputMethodChanged()  override
	{
	}

	virtual std::wstring	getText()	const  override
	{
		return text;
	}

	std::wstring text = L"";
	RECT rect;
	COLORREF bkColor = 0xFFFF00;
	bool selection;

	std::vector<std::wstring> _candidates;
	int _selectedCandidate =0;

	int _beg = 0;
	int _end = 9;

	//int _visibileBeg = 0;
	//int _visibileEnd = 0;
};

EditBox box1(0, 0, 500, 20);
EditBox box2(0, 40, 500, 60);

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

EditBox* pcurrent = &box1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            /*case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);*/
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			//ShowText(hdc);
			box1.render(hdc);
			box2.render(hdc);
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_CHAR:
		if (iswcntrl((wchar_t)wParam) == 0)
		{
			if (pcurrent) pcurrent->onChar((wchar_t)wParam);
		}
		else
		{
			if (pcurrent) pcurrent->keydown((wchar_t)wParam);
		}
		break;

	case WM_LBUTTONDOWN:
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (box1.canSelect(pt.x, pt.y))
		{
			pcurrent = &box1;
			box2.select(false);
			box1.select(true);
			mgr.getTextSink().setObserver(pcurrent);
		}
		else if (box2.canSelect(pt.x, pt.y))
		{
			pcurrent = &box2;
			box1.select(false);
			box2.select(true);
			mgr.getTextSink().setObserver(pcurrent);
		}
		else
		{
			mgr.getTextSink().setObserver(nullptr);
			box1.select(false);
			box2.select(false);
			pcurrent = nullptr;
		}
		InvalidateRect(hWnd, 0, 1);

		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_SIZE:
		RECT winrect;
		GetWindowRect(hWnd, &winrect);

		box1.move(5, 0,  winrect.right- winrect.left-50, 20);
		box2.move(5, 40, winrect.right- winrect.left-50, 60);

		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

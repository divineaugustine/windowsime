#include <windows.h>
#include "TextSink.h"
#include "ComPtr.h"


#include <msctf.h>
#include <OleCtl.h>


class Logger
{
	public:
		bool check(long hr, const wchar_t *format, ...) const
		{
			if (!FAILED(hr)) return true;
			return false;
		}
};

class TextStore : public ITextStoreACP, public ITfContextOwnerCompositionSink, public ITfUIElementSink, public ITfActiveLanguageProfileNotifySink, public ITfTextEditSink
{
public:
	TextStore(ITextService &textService);
	~TextStore();

	void activate	();
	void deactivate	();

	//IUnknown Interfaces
	HRESULT	STDMETHODCALLTYPE QueryInterface(REFIID, LPVOID*)	override;
	DWORD	STDMETHODCALLTYPE AddRef		()					override;
	DWORD	STDMETHODCALLTYPE Release		()					override;

	//TextStoreACP
    HRESULT	STDMETHODCALLTYPE AdviseSink							(REFIID riid, IUnknown* punk, DWORD dwMask)																																override;
    HRESULT	STDMETHODCALLTYPE UnadviseSink							(IUnknown* punk)																																						override;
    HRESULT	STDMETHODCALLTYPE RequestLock							(DWORD dwLockFlags, HRESULT* phrSession)																																override;
    HRESULT	STDMETHODCALLTYPE GetStatus								(TS_STATUS* pdcs)																																						override;
    HRESULT	STDMETHODCALLTYPE QueryInsert							(LONG acpTestStart, LONG acpTestEnd, ULONG cch, LONG* pacpResultStart, LONG* pacpResultEnd)																				override;
    HRESULT	STDMETHODCALLTYPE GetSelection							(ULONG ulIndex, ULONG ulCount, TS_SELECTION_ACP* pSelection, ULONG* pcFetched)																							override;
    HRESULT	STDMETHODCALLTYPE SetSelection							(ULONG ulCount, const TS_SELECTION_ACP* pSelection)																														override;
    HRESULT	STDMETHODCALLTYPE GetText								(LONG acpStart, LONG acpEnd, WCHAR* pchPlain, ULONG cchPlainReq, ULONG* pcchPlainOut, TS_RUNINFO* prgRunInfo, ULONG ulRunInfoReq, ULONG* pulRunInfoOut, LONG* pacpNext)	override;
    HRESULT	STDMETHODCALLTYPE SetText								(DWORD dwFlags, LONG acpStart, LONG acpEnd, const WCHAR* pchText, ULONG cch, TS_TEXTCHANGE* pChange)																	override;
    HRESULT	STDMETHODCALLTYPE GetFormattedText						(LONG acpStart, LONG acpEnd, IDataObject* *ppDataObject)																												override;
    HRESULT	STDMETHODCALLTYPE GetEmbedded							(LONG acpPos, REFGUID rguidService, REFIID riid, IUnknown* *ppunk)																										override;
    HRESULT	STDMETHODCALLTYPE QueryInsertEmbedded					(const GUID* pguidService, const FORMATETC* pFormatEtc, BOOL* pfInsertable)																								override;
    HRESULT	STDMETHODCALLTYPE InsertEmbedded						(DWORD dwFlags, LONG acpStart, LONG acpEnd, IDataObject* pDataObject, TS_TEXTCHANGE* pChange)																			override;
    HRESULT	STDMETHODCALLTYPE RequestSupportedAttrs					(DWORD dwFlags, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs)																										override;
    HRESULT	STDMETHODCALLTYPE RequestAttrsAtPosition				(LONG acpPos, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags)																						override;
    HRESULT	STDMETHODCALLTYPE RequestAttrsTransitioningAtPosition	(LONG acpPos, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags)																						override;
    HRESULT	STDMETHODCALLTYPE FindNextAttrTransition				(LONG acpStart, LONG acpHalt, ULONG cFilterAttrs, const TS_ATTRID* paFilterAttrs, DWORD dwFlags, LONG* pacpNext, BOOL* pfFound, LONG* plFoundOffset)					override;
    HRESULT	STDMETHODCALLTYPE RetrieveRequestedAttrs				(ULONG ulCount, TS_ATTRVAL* paAttrVals, ULONG* pcFetched)																												override;
    HRESULT	STDMETHODCALLTYPE GetEndACP								(LONG* pacp)																																							override;
    HRESULT	STDMETHODCALLTYPE GetActiveView							(TsViewCookie* pvcView)																																					override;
    HRESULT	STDMETHODCALLTYPE GetACPFromPoint						(TsViewCookie vcView, const POINT* pt, DWORD dwFlags, LONG* pacp)																										override;
    HRESULT	STDMETHODCALLTYPE GetTextExt							(TsViewCookie vcView, LONG acpStart, LONG acpEnd, RECT* prc, BOOL* pfClipped)																							override;
    HRESULT	STDMETHODCALLTYPE GetScreenExt							(TsViewCookie vcView, RECT* prc)																																		override;
    HRESULT	STDMETHODCALLTYPE GetWnd								(TsViewCookie vcView, HWND* phwnd)																																		override;
    HRESULT	STDMETHODCALLTYPE InsertTextAtSelection					(DWORD dwFlags, const WCHAR* pchText, ULONG cch, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange)																override;
    HRESULT	STDMETHODCALLTYPE InsertEmbeddedAtSelection				(DWORD dwFlags, IDataObject* pDataObject, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange)																		override;

	//ITfContextOwnerCompositionSink
	HRESULT STDMETHODCALLTYPE OnStartComposition	(ITfCompositionView *pComposition,BOOL *pfOk)				override;
	HRESULT STDMETHODCALLTYPE OnUpdateComposition	(ITfCompositionView *pComposition, ITfRange *pRangeNew)		override;
	HRESULT STDMETHODCALLTYPE OnEndComposition		(ITfCompositionView *pComposition)							override;

	//ITfUIElementSink
	HRESULT	STDMETHODCALLTYPE BeginUIElement		(DWORD dwUIElementId, BOOL *pbShow)							override;
	HRESULT	STDMETHODCALLTYPE UpdateUIElement		(DWORD dwUIElementId)										override;
	HRESULT	STDMETHODCALLTYPE EndUIElement			(DWORD dwUIElementId)										override;

	//ITfActiveLanguageProfileNotifySink
	HRESULT	STDMETHODCALLTYPE OnActivated(REFCLSID clsid, REFGUID guidProfile, BOOL fActivated)					override;

	//ITfTextEditSink
	HRESULT STDMETHODCALLTYPE OnEndEdit(ITfContext*pic, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord)	override;

	void insertSelection(LPCWSTR pchText, LONG cch, TS_TEXTCHANGE* pChange);
		 
	void insertCandidate		(int index);
	void setCandidateSelected	(int index);

	void clearBuffer();
	void setBuffer	(const std::wstring& text);
	void setPosition(int start, int stop);
	//ITfDocumentMgr*	getDocumentManager() const;

	ComPtr<ITfContext>			context;
	ComPtr<IUnknown>			sinkOwner;
	ComPtr<ITextStoreACPSink>	sink;
	//ComPtr<ITfDocumentMgr>		documentManager;

	TsViewCookie		viewCookie;
	TfEditCookie		editCookie;
	DWORD				editSinkCookie;
	HWND hwnd;

	DWORD	sinkMask;
	DWORD	lockType;
	DWORD	uiElementSinkCookie;
	DWORD	languageSinkCookie;
	DWORD   uiElementId;
	BOOL	pendingLockUpgrade;
	BOOL	layoutChanged;
	BOOL	interimChar;

	GUID	chineseGuid;
	GUID	japaneseGuid;

	std::wstring readingInfo;
	ImeInfo		 info;

	ISinkObserver *observer;

	DWORD ref;

	int updateCount = 0;
	ITextService &textService;
	Logger gpu;
};

TextStore::TextStore( ITextService &textService)
	:	ref					(1)
	,	pendingLockUpgrade	(FALSE)
	,	lockType			(0)
	,	layoutChanged		(FALSE)
	,	hwnd				(NULL)
	,	viewCookie			(0)
	,	languageSinkCookie	(0)
	,	observer			(nullptr)
	,	textService			(textService)
{
	chineseGuid  =	{		0xFA550B04
						,	0x5AD7
						,	0x411F
						,	{ 0xA5, 0xAC, 0xCA, 0x03, 0x8E, 0xC5, 0x15, 0XD7 }
					};

	japaneseGuid =	{
							0xA76C93D9
						,	0x5523
						,	0x4E90
						,	{ 0xAA, 0xFA, 0x4D, 0xB1, 0x12, 0xF9, 0xAC, 0x76 }
					};

	HKL englishLanguageLayout	= LoadKeyboardLayout(L"04090409", KLF_SUBSTITUTE_OK);
	HKL chineseLanguageLayout	= LoadKeyboardLayout(L"00000804", KLF_SUBSTITUTE_OK);
	HKL japaneseLanguageLayout	= LoadKeyboardLayout(L"00000411", KLF_SUBSTITUTE_OK);

	HWND	fore	= GetForegroundWindow();
	DWORD	tpid	= GetWindowThreadProcessId(fore, 0);
	HKL		hKL		= GetKeyboardLayout(tpid);

	if (hKL == englishLanguageLayout	)	info.inputMethod = InputMethod::English;	else
	if (hKL == chineseLanguageLayout	)	info.inputMethod = InputMethod::Chinese;	else
	if (hKL == japaneseLanguageLayout	)	info.inputMethod = InputMethod::Japanese;	else
											info.inputMethod = InputMethod::Other;

	activate();
}

TextStore::~TextStore()
{
	deactivate();
}

void TextStore::activate()
{
	auto threadManager			= textService.getThreadManager();
	auto elementManager			= textService.getElementManager();
	auto clientId				= textService.getClientId();
    auto documentManager        = textService.getDocumentManager();
	//HRESULT hr = threadManager->CreateDocumentMgr(&documentManager.reset());
	//if (!SUCCEEDED(hr)) return;
	if (!threadManager || !documentManager || !elementManager || clientId == 0) return;

	gpu.check(documentManager->CreateContext(clientId, 0, (ITextStoreACP*)this, &context.reset(), &editCookie),									L"Could not create TSF context");

	ComPtr<ITfSource> source;
	gpu.check(elementManager->QueryInterface(IID_ITfSource, (LPVOID*)&source.reset()),															L"Could not get TSF source");

	if (!source) return;

	gpu.check(source->AdviseSink(IID_ITfUIElementSink,						(ITfUIElementSink*)this,					&uiElementSinkCookie),	L"Could not get element sink cookie");

	gpu.check(source->AdviseSink(IID_ITfActiveLanguageProfileNotifySink,	(ITfActiveLanguageProfileNotifySink*)this,	&languageSinkCookie	),	L"Could not create TSF language sink");
	
	gpu.check(context->QueryInterface(IID_ITfSource, (LPVOID*)&source.reset()),																	L"Could not create source");
	
	if (!source) return;

	gpu.check(source->AdviseSink(IID_ITfTextEditSink,						(ITfTextEditSink*)this,						&editSinkCookie		),	L"Could not create TSF edit sink"	);

	if (!documentManager) return;

	gpu.check(documentManager->Push(context),																									L"Could not push document context");
}

void TextStore::deactivate()
{
	auto documentManager		= textService.getDocumentManager();
	auto elementManager			= textService.getElementManager();
	
	if (!documentManager || !elementManager || !context) return;

	ComPtr<ITfSource> source;
	gpu.check(elementManager->QueryInterface(IID_ITfSource, (LPVOID*)&source.reset()), L"Could not get TSF source");

	if (!source) return;

	gpu.check(source->UnadviseSink(uiElementSinkCookie	), L"Could not unadvise element sink cookie"	);
	gpu.check(source->UnadviseSink(languageSinkCookie	), L"Could not unadvise language sink cookie"	);

	gpu.check(context->QueryInterface		(IID_ITfSource, (LPVOID*)&source.reset()), L"Could not create source");

	gpu.check(source->UnadviseSink(editSinkCookie		), L"Could not unadvise edit sink cookie"		);

	//gpu.check(documentManager->Pop(0), L"Could not pop document context");

	if (context		) context	.reset();
	if (sink		) sink		.reset();
	if (sinkOwner	) sinkOwner	.reset();
}

HRESULT __stdcall TextStore::QueryInterface(REFIID riid, LPVOID* ppInterface)
{
	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITextStoreACP))	{ AddRef(); *ppInterface = (ITextStoreACP*	 )					this;	return S_OK;			} else
	if (IsEqualIID(riid, IID_ITfUIElementSink))									{ AddRef(); *ppInterface = (ITfUIElementSink*)					this;	return S_OK;			} else
	if (IsEqualIID(riid, IID_ITfContextOwnerCompositionSink))					{ AddRef(); *ppInterface = (ITfContextOwnerCompositionSink*)	this;	return S_OK;			} else
	if (IsEqualIID(riid, IID_ITfActiveLanguageProfileNotifySink))				{ AddRef(); *ppInterface = (ITfActiveLanguageProfileNotifySink*)this;	return S_OK;			} else
	if (IsEqualIID(riid, IID_ITfTextEditSink))									{ AddRef(); *ppInterface = (ITfTextEditSink*)					this;	return S_OK;			}
	else																		{			*ppInterface = NULL;										return E_NOINTERFACE;	}
}

DWORD __stdcall TextStore::AddRef()
{
	return ++ref;
}

DWORD __stdcall TextStore::Release()
{
	return --ref;
}

HRESULT __stdcall TextStore::AdviseSink(REFIID riid, IUnknown *punk, DWORD dwMask)
{
	if (!punk) return E_INVALIDARG;

	ComPtr<IUnknown> punkId;
	gpu.check(punk->QueryInterface(IID_IUnknown, (LPVOID*)&punkId.reset()), L"Could not obtain TSF punk id");

	if (sinkOwner && punkId == sinkOwner){ sinkMask = dwMask; return S_OK; }

	if (!sink)
	{
		if (!IsEqualIID(riid, IID_ITextStoreACPSink)) return E_INVALIDARG;

		gpu.check(punk->QueryInterface(IID_ITextStoreACPSink, (LPVOID*)&sink.reset()), L"Could not get TSF ACP sink");

		sinkOwner	= punkId;
		sinkMask	= dwMask;

		return S_OK;
	}
	else return CONNECT_E_ADVISELIMIT;
}

HRESULT __stdcall TextStore::UnadviseSink(IUnknown *punk)
{
	if (!punk) return E_INVALIDARG;

	ComPtr<IUnknown> punkId;

	gpu.check(punk->QueryInterface(IID_IUnknown, (LPVOID*)&punkId.reset()), L"Could not get TSF punk id");
	
	if (punkId == sinkOwner && sink)
	{
		sinkOwner	= nullptr;
		sink		= nullptr;
		sinkMask	= 0;

		return S_OK;
	}
	else return CONNECT_E_NOCONNECTION;
}

HRESULT __stdcall TextStore::RequestLock(DWORD dwLockFlags, HRESULT *phrSession)
{
	if(sink)
	{
		if(phrSession)
		{
			if(lockType == 0)
			{
				lockType = dwLockFlags;
			    
				*phrSession = sink->OnLockGranted(dwLockFlags);

				lockType = 0;
			    
				if(pendingLockUpgrade)
				{
					pendingLockUpgrade = FALSE;

					HRESULT hr;
					RequestLock(TS_LF_READWRITE, &hr);
				}

				if(layoutChanged)
				{
					layoutChanged = FALSE;
					sink->OnLayoutChange(TS_LC_CHANGE, viewCookie);
				}

				return S_OK;
			}
			else if(dwLockFlags & TS_LF_SYNC)
			{
				*phrSession = TS_E_SYNCHRONOUS;

				return S_OK;
			}
			else if(((lockType & TS_LF_READWRITE) == TS_LF_READ) &&  ((dwLockFlags & TS_LF_READWRITE) == TS_LF_READWRITE))
			{
				pendingLockUpgrade = true;

				*phrSession = TS_S_ASYNC;

				return S_OK;
			}
			else
			{
				*phrSession = E_FAIL;

				return E_FAIL;
			}
		}
		else return E_INVALIDARG;
	}
	else return E_UNEXPECTED;
}

HRESULT __stdcall TextStore::GetStatus(TS_STATUS *pdcs)
{
	/*if (updateCount > 0)
	{
		return E_FAIL;
	}*/

	if (pdcs)
	{
		pdcs->dwDynamicFlags	= 0;
		pdcs->dwStaticFlags		= TS_SS_REGIONS;

		return S_OK;
	}
	else return E_INVALIDARG;
}

HRESULT __stdcall TextStore::QueryInsert(LONG acpTestStart, LONG acpTestEnd, ULONG, LONG* pacpResultStart, LONG* pacpResultEnd)
{
	if (!pacpResultStart || !pacpResultEnd)		return E_INVALIDARG;
	if ((lockType & TS_LF_READ) != TS_LF_READ)	return TS_E_NOLOCK;
	
	LONG lTextLength = (LONG)info.composition.size();

	if (acpTestStart >= 0 && acpTestStart <= acpTestEnd && acpTestEnd <= lTextLength)
	{
		*pacpResultStart	= acpTestStart;
		*pacpResultEnd		= acpTestEnd;

		return S_OK;
	}
	else return E_INVALIDARG;
}

HRESULT __stdcall TextStore::GetSelection(ULONG ulIndex, ULONG ulCount, TS_SELECTION_ACP *pSelection, ULONG *pcFetched)
{
	if(pSelection && pcFetched)
	{
		if((lockType & TS_LF_READ) == TS_LF_READ)
		{
			if(ulIndex == TF_DEFAULT_SELECTION || ulIndex == 0)
			{
				*pcFetched = 1;

				if(ulCount != 0)
				{
					pSelection[0].acpStart				= info.selectBegin;
					pSelection[0].acpEnd				= info.selectEnd;
					pSelection[0].style.fInterimChar	= interimChar;

					if(interimChar)
					{
						pSelection[0].style.ase = TS_AE_NONE;
					}
					else
					{
						pSelection[0].style.ase = TS_AE_END;
					}
				}
				return S_OK;
			}
			else return E_INVALIDARG;
		}
		else return TS_E_NOLOCK;
	}
	else return E_INVALIDARG;
}

HRESULT __stdcall TextStore::SetSelection(ULONG ulCount, const TS_SELECTION_ACP *pSelection)
{
	if(pSelection)
	{
		if((lockType & TS_LF_READWRITE) == TS_LF_READWRITE)
		{
			if(ulCount == 1)
			{
				info.selectBegin	= pSelection[0].acpStart;
				info.selectEnd		= pSelection[0].acpEnd;
				interimChar			= pSelection[0].style.fInterimChar;
				
				if (observer) observer->onReadingStringChanged();

				return S_OK;
			}
			else return E_INVALIDARG;
		}
		else return TS_E_NOLOCK;
	}
	else return E_INVALIDARG;
}

HRESULT __stdcall TextStore::GetText(LONG acpStart, LONG acpEnd, WCHAR *pchPlain, ULONG cchPlainReq, ULONG *pcchPlainRet, TS_RUNINFO *prgRunInfo, ULONG cRunInfoReq, ULONG *pcRunInfoRet, LONG *pacpNext)
{
	if((lockType & TS_LF_READ) == TS_LF_READ)
	{
		if(pacpNext && pcchPlainRet && pcRunInfoRet)
		{
			LONG lTextLength = (LONG)info.composition.size();

			if(acpEnd == -1)
			{
				acpEnd = lTextLength;
			}

			if ((acpStart >= 0) && (acpStart <= acpEnd) && (acpEnd <= lTextLength))
			{
				ULONG cchReq = min(cchPlainReq, (ULONG)(acpEnd - acpStart));

				if(cchPlainReq > 0 && pchPlain && !info.composition.empty())
				{
					LPCWSTR pwszText = info.composition.c_str();

					memcpy(pchPlain, pwszText + acpStart, sizeof(pwszText[0]) * cchReq);

					*pcchPlainRet	= cchReq;
					*pacpNext		= acpStart + cchReq;
				}
				else if(cchPlainReq == 0)
				{
					*pcchPlainRet	= 0;
					*pacpNext		= acpStart;
				}
				else return E_INVALIDARG;		

				if(cRunInfoReq>0 && prgRunInfo)
				{
					prgRunInfo[0].type		= TS_RT_PLAIN;
					prgRunInfo[0].uCount	= cchReq;
					
					*pcRunInfoRet = 1;
				}
				else if(cRunInfoReq == 0)	*pcRunInfoRet = 0;
				else						return E_INVALIDARG;

				return S_OK;
			}
			else return TF_E_INVALIDPOS;
		}
		else return E_INVALIDARG;
	}
	else return TS_E_NOLOCK;
}

HRESULT __stdcall TextStore::SetText(DWORD, LONG acpStart, LONG acpEnd, const WCHAR* pchText, ULONG cch, TS_TEXTCHANGE* pChange)
{
    TS_SELECTION_ACP tsa;

    tsa.acpStart			= acpStart;
    tsa.acpEnd				= acpEnd;
    tsa.style.ase			= TS_AE_END;
    tsa.style.fInterimChar	= FALSE;

	gpu.check(SetSelection(1, &tsa), L"Could not set TSF selection");

	LONG start, end;

    return InsertTextAtSelection(TS_IAS_NOQUERY, pchText, cch, &start, &end, pChange);
}

HRESULT __stdcall TextStore::GetFormattedText(LONG, LONG, IDataObject **)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::GetEmbedded(LONG, REFGUID, REFIID, IUnknown **)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::QueryInsertEmbedded(const GUID*, const FORMATETC*, BOOL* pfInsertable)
{
	if (pfInsertable)
	{
		*pfInsertable = FALSE;

		return S_OK;
	}
	else return E_INVALIDARG;
}

HRESULT __stdcall TextStore::InsertEmbedded(DWORD, LONG, LONG, IDataObject *pDataObject, TS_TEXTCHANGE *)
{
	if (pDataObject)	return TS_E_FORMAT;
	else				return E_INVALIDARG;
}

HRESULT __stdcall TextStore::RequestSupportedAttrs(DWORD, ULONG, const TS_ATTRID *)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::RequestAttrsAtPosition(LONG, ULONG, const TS_ATTRID *, DWORD)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::RequestAttrsTransitioningAtPosition(LONG, ULONG, const TS_ATTRID *, DWORD)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::FindNextAttrTransition(LONG, LONG, ULONG, const TS_ATTRID *, DWORD, LONG *, BOOL *, LONG *)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::RetrieveRequestedAttrs(ULONG, TS_ATTRVAL *, ULONG *)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::GetEndACP(LONG *pacp)
{
	if ((lockType & TS_LF_READ) == TS_LF_READ)
	{
		if (pacp)
		{
			*pacp = info.selectEnd;

			return S_OK;
		}
		else return E_INVALIDARG;
	}
	else return TS_E_NOLOCK;
}

HRESULT __stdcall TextStore::GetActiveView(TsViewCookie *pvcView)
{
	if (pvcView)
	{
		*pvcView = viewCookie;

		return S_OK;
	}
	else return E_INVALIDARG;
}

HRESULT __stdcall TextStore::GetACPFromPoint(TsViewCookie, const POINT *, DWORD, LONG *)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::GetTextExt(TsViewCookie , LONG, LONG, RECT*, BOOL*)
{
	return S_OK;
}

HRESULT __stdcall TextStore::GetScreenExt(TsViewCookie, RECT*)
{
	return S_OK;
}

HRESULT __stdcall TextStore::GetWnd(TsViewCookie vcView, HWND* phwnd)
{
	if (vcView == viewCookie)
	{
		if (phwnd)
		{
			*phwnd = hwnd;
		}
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT __stdcall TextStore::InsertTextAtSelection(DWORD dwFlags, const WCHAR* pchText, ULONG cch, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange)
{
    if((lockType & TS_LF_READWRITE) != TS_LF_READWRITE) return TS_E_NOLOCK;
	{
		if(dwFlags == 0)
		{
			if(pacpStart && pacpEnd && pChange)
			{
				insertSelection(pchText, cch, pChange);

				*pacpStart	= info.selectBegin;
				*pacpEnd	= info.selectBegin + cch;

				return S_OK;
			}
			else
			{
				return E_INVALIDARG;
			}
		}
		else if(dwFlags & TF_IAS_NOQUERY)
		{
			if(pChange)
			{
				insertSelection(pchText, cch, pChange);

				if(pacpStart && pacpEnd)
				{
					*pacpStart	= info.selectBegin;
					*pacpEnd	= info.selectBegin + cch;
				}

				return S_OK;
			}
			else
			{
				return E_INVALIDARG;
			}
		}
		else if(dwFlags & TS_IAS_QUERYONLY)
		{
			if(pacpStart && pacpEnd)
			{
				*pacpStart	= info.selectBegin;
				*pacpEnd	= info.selectBegin + cch;

				if(pChange)
				{
					pChange->acpStart	= info.selectBegin;
					pChange->acpOldEnd	= info.selectEnd;
					pChange->acpNewEnd	= info.selectBegin + cch;
				}

				return S_OK;
			}
			else
			{
				return E_INVALIDARG;
			}
		}
		else
		{
			return E_INVALIDARG;
		}
	}
}

HRESULT __stdcall TextStore::InsertEmbeddedAtSelection(DWORD, IDataObject*, LONG*, LONG*, TS_TEXTCHANGE*)
{
	return E_NOTIMPL;
}

HRESULT __stdcall TextStore::OnStartComposition(ITfCompositionView*, BOOL* pfOk)
{
	if (pfOk) *pfOk = TRUE;

	return S_OK;
}

HRESULT __stdcall TextStore::OnUpdateComposition(ITfCompositionView*, ITfRange*)
{
	return S_OK;
}

HRESULT __stdcall TextStore::OnEndComposition(ITfCompositionView *)
{
	info.underlineStart	 = 0;
	info.underlineLength = 0;
	
	if (observer) observer->onCompositionChanged();

	return S_OK;
}

HRESULT __stdcall TextStore::BeginUIElement(DWORD dwUIElementId, BOOL* pbShow)
{
	if (pbShow) { *pbShow = FALSE;	uiElementId = dwUIElementId; return  S_OK; }
	else							return E_INVALIDARG;
}

struct ScopedIncr
{
	ScopedIncr(int& in) :ref(in)
	{
		++ref;
	}
	~ScopedIncr()
	{
		--ref;
	}
	int& ref;
};
HRESULT __stdcall TextStore::UpdateUIElement(DWORD dwUIElementId)
{
	

	//update candidate list and reading string
	auto elementManager			= textService.getElementManager();

	// ComPtr<ITfUIElement>					uiElement;
	ITfUIElement*					uiElement;
	ComPtr<ITfReadingInformationUIElement>	readingInformation;
	ComPtr<ITfCandidateListUIElement>		candidateListUiElement;

	gpu.check(elementManager->GetUIElement(dwUIElementId, &uiElement), L"Could not get TSF UI Element");

	if (SUCCEEDED(uiElement->QueryInterface(IID_ITfCandidateListUIElement, (LPVOID*)&candidateListUiElement.reset())))
	{
		info.candidateList.clear();
		UINT count;
			
		//TF_CLUIE_SELECTION

		std::vector<std::wstring> clist;

		{
			ScopedIncr scope(updateCount);
			gpu.check(candidateListUiElement->GetCount(&count), L"Could not get candidate list count");

			count = count > 100 ? 100 : count;

			for (UINT i = 0; i < count; ++i)
			{
				BSTR candidate;

				if (SUCCEEDED(candidateListUiElement->GetString(i, &candidate)))
				{
					LPWSTR text = candidate;
					clist.push_back(text);
				}
				else
				{
					break;
				}
			}
		}
		


		info.candidateList = clist;

		UINT	selection = UINT_MAX;
		unsigned int		listLength = 7;//  int(clist.size());// 7;

		if (FAILED(candidateListUiElement->GetSelection(&selection)))
		{
			selection = UINT_MAX;
		}

		//GetSelection returns UINT_MAX when there is no currently selected item...
		info.selectedCandidate = selection == UINT_MAX ? -1 : static_cast<int>(selection);

		//TODO: Seems like the count returned from the UI Element handled by Windows Text Input Processor is not always correct
		//Some say that there are over 1000 items to be read, however GetString will stop succeeding at 100 each time
		//Because most users are not going to parse through ~15 pages to find their desired string this work around is implemented
		if (info.selectedCandidate >= static_cast<int>(info.candidateList.size()))
		{
			setCandidateSelected(static_cast<int>(info.candidateList.size()) - 1);
		}

		//UINT bufferSize = static_cast<UINT>(info.candidateList.size()) / listLength;

		////Extra offset for last page that is smaller than the listLength
		//if (info.candidateList.size() > listLength && info.candidateList.size() % listLength != 0) bufferSize++;
		//
		//if (bufferSize > 0)
		//{
		//	UINT* pageIndex = new UINT[bufferSize];

		//	for (UINT i = 0; i < bufferSize; ++i) pageIndex[i] = i * listLength;

		//	gpu.check(candidateListUiElement->SetPageIndex(pageIndex, bufferSize-1), L"Could not set candidate list page indices");

		//	delete[] pageIndex;
		//}

		if (observer) observer->onCandidateListChanged();
	}

	if (SUCCEEDED(uiElement->QueryInterface(IID_ITfReadingInformationUIElement, (LPVOID*)&readingInformation.reset())))
	{
		BSTR reading;
		gpu.check(readingInformation->GetString(&reading), L"Could not get TSF reading string");
		readingInfo = reading;

		if (observer) observer->onReadingStringChanged();
	}

	return S_OK;
}

HRESULT __stdcall TextStore::EndUIElement(DWORD dwUIElementId)
{
	auto elementManager		  = textService.getElementManager();
	if (!elementManager) return E_FAIL;

	ITfUIElement*					uiElement = nullptr;
	gpu.check(elementManager->GetUIElement(dwUIElementId, &uiElement), L"Could not get handle to ui element");

	ComPtr<ITfCandidateListUIElement>		candidateListUiElement;
	ComPtr<ITfReadingInformationUIElement>	readingInformationUiElement;

	if		(SUCCEEDED(uiElement->QueryInterface(IID_ITfCandidateListUIElement,			(LPVOID*)&candidateListUiElement		.reset())))
	{ 
		info.candidateList	.clear(); 
		if (observer) observer->onCandidateListChanged(); 
	}
	else
	{
		if (SUCCEEDED(uiElement->QueryInterface(IID_ITfReadingInformationUIElement, (LPVOID*)&readingInformationUiElement.reset())))
		{
			readingInfo.clear();
		}
	}

	return S_OK;
}

HRESULT __stdcall TextStore::OnActivated(REFCLSID, REFGUID guidProfile, BOOL fActivated)
{
	if (fActivated)
	{
		if (guidProfile == chineseGuid	) info.inputMethod = InputMethod::Chinese;	else
		if (guidProfile == japaneseGuid	) info.inputMethod = InputMethod::Japanese;

		if (observer) observer->onInputMethodChanged();
	}

	return S_OK;
}

HRESULT __stdcall TextStore::OnEndEdit(ITfContext * pic, TfEditCookie, ITfEditRecord * pEditRecord)
{
	if (!pic || !pEditRecord) return E_INVALIDARG;
	
	info.underlineStart		= 0;
	info.underlineLength	= 0;

	ComPtr<ITfContextComposition> contextComposition;
	
	if (!gpu.check(pic->QueryInterface(IID_ITfContextComposition, (void**)&contextComposition.reset()), L"Could not get TSF context composition"	)) return E_FAIL;
	
	ComPtr<IEnumITfCompositionView> enumCompositionView;
	
	if (!gpu.check(contextComposition->EnumCompositions(&enumCompositionView.reset()),					L"Could not get TSF enum composition view"	)) return E_FAIL;
	
	ComPtr<ITfCompositionView>	compositionView;
	ComPtr<ITfRange>			range;
	ComPtr<ITfRangeACP>			rangeAcp;
  
	if (!gpu.check(enumCompositionView->Next(1, &compositionView.reset(), nullptr),						L"Could not get next composition view"		)) return E_FAIL;

	if (!compositionView) return S_OK;

	if (!gpu.check(compositionView->GetRange(&range.reset()),											L"Could not get composition view range"		)) return E_FAIL;
	if (!gpu.check(range->QueryInterface(IID_ITfRangeACP, (void**)&rangeAcp.reset()),					L"Could not get ACP range"					)) return E_FAIL;

    LONG start	= 0;
    LONG length	= 0;

    if (!gpu.check(rangeAcp->GetExtent(&start, &length),												L"Could not get ACP range extent"			)) return E_FAIL;

	info.underlineStart		= static_cast<int>(start	);
	info.underlineLength	= static_cast<int>(length	);

	if (observer) observer->onCompositionChanged();

	return S_OK;
}

void TextStore::insertSelection(LPCWSTR pchText, LONG cch, TS_TEXTCHANGE * pChange)
{
	info.composition.erase (info.composition.begin() + info.selectBegin, info.composition.begin() + info.selectEnd);
	info.composition.insert(info.composition.begin() + info.selectBegin, pchText, pchText + cch);

	pChange->acpStart	= info.selectBegin;
	pChange->acpOldEnd  = info.selectEnd;
	pChange->acpNewEnd  = info.selectBegin + cch;

	info.selectEnd = info.selectBegin + cch;
	
	layoutChanged = TRUE;
}

void TextStore::insertCandidate(int index)
{

	ComPtr<ITfCandidateListUIElementBehavior> candidateBehavior;

	auto elementManager = textService.getElementManager();

	ITfUIElement*					uiElement = nullptr;

	gpu.check(elementManager->GetUIElement(uiElementId, &uiElement), L"Could not get TSF UI Element");

	uiElement->QueryInterface(IID_ITfCandidateListUIElementBehavior, (void**)&candidateBehavior.reset());

	candidateBehavior->SetSelection(index);
	candidateBehavior->Finalize();
}

void TextStore::setCandidateSelected(int index)
{

	ComPtr<ITfCandidateListUIElementBehavior> candidateBehavior;

	auto elementManager = textService.getElementManager();

	ComPtr<ITfUIElement> uiElement;

	gpu.check(elementManager->GetUIElement(0, &uiElement.reset()), L"Could not get TSF UI Element");

	uiElement->QueryInterface(IID_ITfCandidateListUIElementBehavior, (void**)&candidateBehavior.reset());

	candidateBehavior->SetSelection(index);
}

void TextStore::clearBuffer()
{
	if (!sink) return;

	sink->OnStartEditTransaction();

	TS_TEXTCHANGE textChange;
	textChange.acpNewEnd	= 0;
	textChange.acpStart		= 0;
	textChange.acpOldEnd	= info.selectEnd;

	info.composition	= L"";
	info.selectBegin	= 0;
	info.selectEnd		= 0;

	sink->OnTextChange(0, &textChange);

	sink->OnEndEditTransaction();
}

void TextStore::setBuffer(const std::wstring& text)
{
	if (!sink) return;

	sink->OnStartEditTransaction();

	LONG pos = text.empty() ? 0 : (LONG)text.size() - 1;

	TS_TEXTCHANGE textChange;
	textChange.acpNewEnd	= pos;
	textChange.acpStart		= pos;
	textChange.acpOldEnd	= info.selectEnd;

	info.composition	= text;
	info.selectBegin	= pos;
	info.selectEnd		= pos;

	sink->OnSelectionChange	();

	sink->OnEndEditTransaction();
}

void TextStore::setPosition(int start, int stop)
{
	if (!sink) return;

	sink->OnStartEditTransaction();

	TS_TEXTCHANGE textChange;
	textChange.acpNewEnd	= stop;
	textChange.acpStart		= start;
	textChange.acpOldEnd	= info.selectEnd;

	info.selectBegin	= start;
	info.selectEnd		= stop;

	sink->OnSelectionChange();

	sink->OnEndEditTransaction();
}

//ITfDocumentMgr* TextStore::getDocumentManager() const
//{
//	return documentManager;
//}

TextSink::TextSink(ITextService &textService)
	:	textService(textService)
	,	textStore(std::make_shared<TextStore>(textService))
{
}

TextSink::~TextSink()
{
}

void TextSink::setHandle(HWND hwnd)
{
	if (textStore) textStore->hwnd = hwnd;
}

ImeInfo TextSink::getImeInfo() const
{
	if (textStore) return textStore->info;

	return ImeInfo();
}

void TextSink::insertCandidate(int index)
{
	if (textStore) textStore->insertCandidate(index);
}

void TextSink::setPosition(int start, int stop)
{
	if (textStore) textStore->setPosition(start, stop);
}

void TextSink::setSelection(int index)
{
	if (textStore) textStore->setCandidateSelected(index);
}

void TextSink::updateBuffer()
{
	if (textStore && textStore->observer)
	{
		textStore->clearBuffer();

		std::wstring text = textStore->observer->getText();

		textStore->setBuffer(text);
	}
}

void TextSink::setObserver(ISinkObserver* observer)
{
	if (textStore)
	{
		if (observer && observer == textStore->observer) return;

		textStore->info.candidateList.clear();
		textStore->observer = observer;
		ITfThreadMgr* threadManager = textService.getThreadManager();

		if (observer)
		{			
			threadManager->SetFocus(textService.getDocumentManager());
			textStore->clearBuffer();
			std::wstring text = observer->getText();
			textStore->setBuffer(text);
		}

	}
}

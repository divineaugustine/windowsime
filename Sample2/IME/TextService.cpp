#include "TextService.h"
#include "ComPtr.h"

#include <windows.h>
#include <msctf.h>
#include <OleCtl.h>


//class ThreadManager 
//{
//public:
//
//	void initResources();
//	void exitResources();
//
//	ComPtr<ITfThreadMgr>				threadManager;
//	ComPtr<ITfDocumentMgr>				documentManager;
//	ComPtr<ITfMessagePump>				messagePump;
//	ComPtr<ITfKeystrokeMgr>				keyManager;
//	ComPtr<ITfUIElementMgr>				uiElementManager;
//	ComPtr<ITfCategoryMgr>				categoryManager;
//	ComPtr<ITfDisplayAttributeMgr>		displayManager;
//
//	TfClientId clientId;
//
//	HRESULT hr;
//
//
//	ThreadManager();
//	~ThreadManager();
////public:
////	static ThreadManager instance;
//};

//ThreadManager ThreadManager::instance;

ThreadManager::ThreadManager()
	:	threadManager	(nullptr			)
	,	documentManager	(nullptr			)
	,	messagePump		(nullptr			)
	,	keyManager		(nullptr			)
	,	clientId		((TfClientId) - 1	)
{
}

ThreadManager::~ThreadManager()
{
}

void ThreadManager::initResources()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	// CoInitialize(nullptr);


	hr = CoCreateInstance(CLSID_TF_ThreadMgr,				NULL, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr,				(LPVOID*)&threadManager .reset());
	if (!SUCCEEDED(hr)) return;

	hr = threadManager->QueryInterface(IID_ITfMessagePump,		(void **)&messagePump	.reset());
	if (!SUCCEEDED(hr)) return;

	hr = threadManager->QueryInterface(IID_ITfKeystrokeMgr,		(LPVOID*)&keyManager	.reset());
	if (!SUCCEEDED(hr)) return;

	hr = threadManager->CreateDocumentMgr(&documentManager.reset());
	if (!SUCCEEDED(hr)) return;

	threadManager->SetFocus(documentManager);

	ComPtr<ITfThreadMgrEx> threadManagerEx;

	hr = threadManager->QueryInterface(IID_ITfThreadMgrEx, (LPVOID*)&threadManagerEx.reset());
	if (!SUCCEEDED(hr)) return;

	hr = threadManagerEx->ActivateEx(&clientId, TF_TMAE_UIELEMENTENABLEDONLY);
	if (!SUCCEEDED(hr)) return;

	hr = threadManager->QueryInterface(IID_ITfUIElementMgr, (LPVOID*)&uiElementManager.reset());
	if (!SUCCEEDED(hr)) return;
	//uiElementManager->AddRef();

	hr = CoCreateInstance(CLSID_TF_CategoryMgr,				NULL, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr,				(LPVOID*)&categoryManager	.reset());
	if (!SUCCEEDED(hr)) return;

	hr = CoCreateInstance(CLSID_TF_DisplayAttributeMgr,		NULL, CLSCTX_INPROC_SERVER, IID_ITfDisplayAttributeMgr,		(LPVOID*)&displayManager	.reset());
	if (!SUCCEEDED(hr)) return;
}

void ThreadManager::exitResources()
{
	if (threadManager)
	{
		threadManager->Deactivate();
		threadManager.reset();
		documentManager->Pop(TF_POPF_ALL);
	}

	if (documentManager	 ) documentManager		.reset();
	if (messagePump		 ) messagePump			.reset();
	if (keyManager		 ) keyManager			.reset();
	if (uiElementManager ) uiElementManager		.reset();
	if (categoryManager	 ) categoryManager		.reset();
	if (displayManager	 ) displayManager		.reset();

	CoUninitialize();
}


TextService::TextService():threadMgr()
{
	threadMgr.initResources();	//exitResources();
}

TextService::~TextService()
{
	threadMgr.exitResources();	//exitResources();
}

ITfThreadMgr * TextService::getThreadManager() const
{
	return threadMgr.threadManager;
}

ITfCategoryMgr* TextService::getCategoryManager() const
{
	return threadMgr.categoryManager;
}

ITfDisplayAttributeMgr* TextService::getDisplayManager() const
{
	return threadMgr.displayManager;
}

ITfDocumentMgr* TextService::getDocumentManager() const
{
	return threadMgr.documentManager;
}

ITfUIElementMgr* TextService::getElementManager() const
{
	return threadMgr.uiElementManager;
}

ITfKeystrokeMgr* TextService::getKeyManager() const
{
	return threadMgr.keyManager;
}

unsigned long TextService::getClientId() const
{
	return threadMgr.clientId;
}

void TextService::setFocus(ITfDocumentMgr *documentmanager)
{
	if (threadMgr.threadManager && documentmanager) threadMgr.threadManager->SetFocus(documentmanager);
}


#pragma once

#include "ITextService.h"

#include <memory>
#include <functional>
#include "ComPtr.h"
// class ThreadManager;

class ThreadManager
{
public:

	void initResources();
	void exitResources();

	ComPtr<ITfThreadMgr>				threadManager;
	ComPtr<ITfDocumentMgr>				documentManager;
	ComPtr<ITfMessagePump>				messagePump;
	ComPtr<ITfKeystrokeMgr>				keyManager;
	ComPtr<ITfUIElementMgr>				uiElementManager;
	ComPtr<ITfCategoryMgr>				categoryManager;
	ComPtr<ITfDisplayAttributeMgr>		displayManager;

	TfClientId clientId;

	HRESULT hr;


	ThreadManager();
	~ThreadManager();
	//public:
	//	static ThreadManager instance;
};

//! \ingroup SenseGraphics
//! The TextService is used for blocking UI rendering of the default IME and getting a handle to internal Text Service Framework objects
class TextService : public ITextService
{
public:
	TextService();
	virtual ~TextService();

	ITfThreadMgr*				getThreadManager		() const	override;				//!< Get a handle to the ITfThreadMgr
	ITfCategoryMgr*				getCategoryManager		() const	override;				//!< Get a handle to the ITfCategoryMgr
	ITfDisplayAttributeMgr*		getDisplayManager		() const	override;				//!< Get a handle to the ITfDisplayAttributeMgr
	ITfDocumentMgr*				getDocumentManager		() const	override;				//!< Get a handle to the ITfDocumentMgr
	ITfUIElementMgr*			getElementManager		() const	override;				//!< Get a handle to the ITfUIElementMgr
	ITfKeystrokeMgr*			getKeyManager			() const	override;				//!< Get a handle to the ITfKeystrokeMgr
	unsigned long				getClientId				() const	override;				//!< Get the client Id associated with the current process
	void						setFocus				(ITfDocumentMgr *documentmanager) 			override;				//!< Set the focus of the TSF to our process

private:
	
	ThreadManager threadMgr;
private:
	TextService				(const TextService& ) = delete;
	TextService& operator=	(const TextService& ) = delete;
	TextService				(const TextService&&) = delete;
	TextService& operator=	(const TextService&&) = delete;
};


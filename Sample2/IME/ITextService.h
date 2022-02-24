
#pragma once
#include <ctffunc.h>
//Forward declare for TSF interfaces
struct ITfThreadMgr;
struct ITfDisplayAttributeMgr;
struct ITfCategoryMgr;
struct ITfDocumentMgr;
struct ITfUIElementMgr;
struct ITfKeystrokeMgr;

//! \ingroup SenseGraphics
//! The ITextService is used for blocking UI rendering of the default IME and getting a handle to internal Text Service Framework objects
class ITextService
{
public:
	virtual ITfThreadMgr*				getThreadManager	() const	= 0;			//!< Get a handle to the ITfThreadMgr
	virtual ITfCategoryMgr*				getCategoryManager	() const	= 0;			//!< Get a handle to the ITfCategoryMgr
	virtual ITfDisplayAttributeMgr*		getDisplayManager	() const	= 0;			//!< Get a handle to the ITfDisplayAttributeMgr
	virtual ITfDocumentMgr*				getDocumentManager	() const	= 0;			//!< Get a handle to the ITfDocumentMgr
	virtual ITfUIElementMgr*			getElementManager	() const	= 0;			//!< Get a handle to the ITfUIElementMgr
	virtual ITfKeystrokeMgr*			getKeyManager		() const	= 0;			//!< Get a handle to the ITfKeystrokeMgr
	virtual unsigned long				getClientId			() const	= 0;			//!< Get the client Id associated with the current process
	virtual void						setFocus			(ITfDocumentMgr *documentmanager)			= 0;			//!< Set the focus of the TSF to our process
};
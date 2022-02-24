
#pragma once

#include "ITextSink.h"
#include "ITextService.h"
#include <memory>
#include <functional>

class TextStore;

//! \ingroup SenseGraphics
//! The TextSink is used for handling the text buffer of the IME and interfacing with Microsoft Text Services Framework.
class TextSink : public ITextSink
{
public:
	TextSink(ITextService &textService);
	virtual ~TextSink();

	void						setHandle				(HWND hwnd)							override;			//!< Set the handle to the current window
	ImeInfo						getImeInfo				()							const	override;			//!< Get info on the IME state			
	void						insertCandidate			(int index)							override;			//!< Select a candidate using its index
	void						setPosition				(int start, int stop)				override;			//!< Set the selected position
	void						setSelection			(int index)							override;			//!< Set the currently selected candidate using its index
	void						updateBuffer			()									override;			//!< Update the buffer with new text
	void						setObserver				(ISinkObserver* observer)			override;			//!< Subscribe an observer for updates from the text sink

private:
	std::shared_ptr<TextStore>	textStore;																		//!< Implementation of Windows text store

	ITextService &textService;
	bool enableNumeric = false;

private:
	TextSink			(const TextSink& ) = delete;
	TextSink& operator=	(const TextSink& ) = delete;
	TextSink			(const TextSink&&) = delete;
	TextSink& operator=	(const TextSink&&) = delete;

};


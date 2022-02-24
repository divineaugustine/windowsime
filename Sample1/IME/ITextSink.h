
#pragma once

#include <string>
#include <vector>
#include <functional>

enum class InputMethod
{
	English
	, Chinese
	, Japanese
	, Other
};

//! \ingroup SenseGraphics
//! The ImeInfo is used to store information about the current state of the ime
struct ImeInfo
{
	ImeInfo()
		: composition		(L"")
		, inputMethod		(InputMethod::Other)
		, underlineStart	(0)
		, underlineLength	(0)
		, selectedCandidate	(0)
		, selectBegin		(0)
		, selectEnd			(0)
	{
	}

	std::vector<std::wstring>	candidateList;
	std::wstring				composition;
	InputMethod					inputMethod;
	int							underlineStart;
	int							underlineLength;
	int							selectedCandidate;
	long						selectBegin;
	long						selectEnd;
};


//! \ingroup SenseGraphics
//! The ISinkObserver is to be inherited by any control which implements IME functionality
class ISinkObserver
{
public:
	virtual void			onCandidateListChanged	()			= 0;				//!< Called whenever the candidate list of the IME is changed
	virtual void			onReadingStringChanged	()			= 0;				//!< Called whenever the reading string of the IME is changed
	virtual void			onCompositionChanged	()			= 0;				//!< Called whenever the reading strings format is changed
	virtual void			onInputMethodChanged	()			= 0;				//!< Called whenever the user changes the current input method
	virtual std::wstring	getText					()	const	= 0;				//!< Gets the text from the control
};

//! \ingroup SenseGraphics
//! The ITextSink is used for handling the text buffer of the IME and interfacing with Microsoft Text Services Framework.
class ITextSink
{
public:
	virtual void						setHandle		(HWND hwnd)							= 0;			//!< Set the handle to the current window
	virtual ImeInfo						getImeInfo		()							const	= 0;			//!< Get info on the IME state
	virtual void						insertCandidate	(int index)							= 0;			//!< Select a candidate using its index
	virtual void						setPosition		(int start, int stop)				= 0;			//!< Set the selected position
	virtual void						setSelection	(int index)							= 0;			//!< Set the currently selected candidate using its index
	virtual void						updateBuffer	()									= 0;			//!< Update the buffer with new text
	virtual void						setObserver		(ISinkObserver* observer)			= 0;			//!< Subscribe an observer for updates from the text sink
};


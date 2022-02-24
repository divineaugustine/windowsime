#pragma once

#include "IME\TextService.h"
#include "IME\TextSink.h"


class TextServiceManager final
{
public:
	TextServiceManager()
		: 
		textService()
		,textSink(textService)
		
	{
		
	}
	
	ITextService & getTextService()
	{
		return textService;
	}

	ITextSink& getTextSink()
	{
		return textSink;
	}

private:
	TextService textService;
	TextSink textSink;

};
/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Provides a source for WAV format.

#ifdef _FORMAT_WAV
#ifndef XAL_WAV_SOURCE_H
#define XAL_WAV_SOURCE_H

#include <hltypes/hresource.h>
#include <hltypes/hstring.h>

#include "AudioManager.h"
#include "Source.h"
#include "xalExport.h"

namespace xal
{
	class xalExport WAV_Source : public Source
	{
	public:
		WAV_Source(chstr filename, SourceMode sourceMode, BufferMode bufferMode);
		~WAV_Source();

		bool open() override;
		void rewind() override;
		bool load(hstream& output) override;
		int loadChunk(hstream& output, int size = STREAM_BUFFER_SIZE) override;

	protected:
		void _findData();

	};

}

#endif
#endif

/// @file
/// @version 4.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/harray.h>
#include <hltypes/hrdir.h>
#include <hltypes/hresource.h>
#include <hltypes/hstring.h>

#include "AudioManager.h"
#include "Buffer.h"
#include "Category.h"
#include "Sound.h"
#include "Source.h"
#include "xal.h"

namespace xal
{
	Sound::Sound(chstr filename, Category* category, chstr prefix)
	{
		this->filename = filename;
		this->category = category;
		this->buffer = xal::manager->_createBuffer(this);
		// extracting filename without extension and prepending the prefix
		this->name = prefix + hresource::withoutExtension(hrdir::baseName(filename));
	}

	Sound::Sound(chstr name, Category* category, unsigned char* data, int size, int channels, int samplingRate, int bitsPerSample)
	{
		this->filename = "";
		this->category = category;
		this->buffer = xal::manager->_createBuffer(category, data, size, channels, samplingRate, bitsPerSample);
		// extracting filename without extension and prepending the prefix
		this->name = name;
		// calling this here, because otherwise it would have to be called in the constructor of Buffer
		xal::manager->_convertStream("manual sound in category '" + name + "'", channels, samplingRate, bitsPerSample, this->buffer->getStream());
	}

	Sound::~Sound()
	{
		xal::manager->_destroyBuffer(this->buffer);
	}
	
	int Sound::getSize()
	{
		return this->buffer->getSize();
	}

	int Sound::getSourceSize()
	{
		Source* source = this->buffer->getSource();
		if (source == NULL)
		{
			return 0;
		}
		return source->getRamSize();
	}

	int Sound::getChannels()
	{
		return this->buffer->getChannels();
	}

	int Sound::getSamplingRate()
	{
		return this->buffer->getSamplingRate();
	}

	int Sound::getBitsPerSample()
	{
		return this->buffer->getBitsPerSample();
	}

	float Sound::getDuration()
	{
		return this->buffer->getDuration();
	}

	Format Sound::getFormat() const
	{
		return this->buffer->getFormat();
	}
	
	float Sound::getBufferIdleTime()
	{
		return this->buffer->getIdleTime();
	}

	bool Sound::isStreamed() const
	{
		return this->buffer->isStreamed();
	}

	bool Sound::isLoaded()
	{
		return this->buffer->isLoaded();
	}

	void Sound::readPcmData(hstream& output)
	{
		Buffer(this).readPcmData(output);
	}

}

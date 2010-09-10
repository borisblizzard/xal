/************************************************************************************\
This source file is part of the KS(X) audio library                                  *
For latest info, see http://code.google.com/p/libxal/                                *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>
#include "Category.h"
#include "Source.h"
#include "SoundBuffer.h"
#include "StreamSound.h"
#include "AudioManager.h"

#ifndef __APPLE__
	#include <AL/al.h>
#else
	#include <OpenAL/al.h>
#endif

namespace xal
{
/******* CONSTRUCT / DESTRUCT ******************************************/

	Source::Source(SoundBuffer* sound) : Sound(), sourceId(0),
		gain(1.0f), looping(false), paused(false), fadeTime(0.0f),
		fadeSpeed(0.0f), bound(true)
	{
		this->sound = sound;
	}

	Source::~Source()
	{
		this->stop();
	}

/******* METHODS *******************************************************/
	
	void Source::update(float k)
	{
		if (this->sourceId == 0)
		{
			return;
		}
		this->sound->setSourceId(this->sourceId);
		this->sound->update(k);
		if (this->isPlaying())
		{
			if (this->isFading())
			{
				this->fadeTime += this->fadeSpeed * k;
				if (this->fadeTime >= 1.0f && this->fadeSpeed > 0.0f)
				{
					alSourcef(this->sourceId, AL_GAIN, this->gain *
						this->sound->getCategory()->getGain() * xal::mgr->getGlobalGain());
					this->fadeTime = 1.0f;
					this->fadeSpeed = 0.0f;
				}
				else if (this->fadeTime <= 0.0f && this->fadeSpeed < 0.0f)
				{
					this->paused ? this->pause() : this->stop();
					this->fadeTime = 0.0f;
					this->fadeSpeed = 0.0f;
				}
				else
				{
					alSourcef(this->sourceId, AL_GAIN, this->fadeTime * this->gain *
						this->sound->getCategory()->getGain() * xal::mgr->getGlobalGain());
				}
			}
		}
		if (!this->isPlaying() && !this->isPaused())
		{
			this->unbind();
		}
	}

	Sound* Source::play(float fadeTime, bool looping)
	{
		if (this->sourceId == 0)
		{
			this->sourceId = xal::mgr->allocateSourceId();
			if (this->sourceId == 0)
			{
				return NULL;
			}
		}
		if (!this->paused)
		{
			this->looping = looping;
		}
		if (this->sound->getCategory()->isStreamed())
		{
			this->sound->setSourceId(this->sourceId);
			((StreamSound*)this->sound)->queueBuffers();
			alSourcei(this->sourceId, AL_LOOPING, false);
		}
		else if (!this->isPaused())
		{
			alSourcei(this->sourceId, AL_BUFFER, this->getBuffer());
			alSourcei(this->sourceId, AL_LOOPING, this->looping);
		}
		if (this->isPaused())
		{
			alSourcef(this->sourceId, AL_SEC_OFFSET, this->sampleOffset);
		}
		bool alreadyFading = this->isFading();
		if (fadeTime > 0)
		{
			this->fadeSpeed = 1.0f / fadeTime;
		}
		else
		{
			this->fadeTime = 1.0f;
			this->fadeSpeed = 0.0f;
		}
		alSourcef(this->sourceId, AL_GAIN, this->fadeTime * this->gain *
			this->sound->getCategory()->getGain() * xal::mgr->getGlobalGain());
		if (!alreadyFading)
		{
			alSourcePlay(this->sourceId);
		}
		this->paused = false;
		return this;
	}

	void Source::stop(float fadeTime)
	{
		this->_stop(fadeTime);
	}

	void Source::pause(float fadeTime)
	{
		this->_stop(fadeTime, true);
	}

	void Source::_stop(float fadeTime, bool pause)
	{
		if (this->sourceId == 0)
		{
			return;
		}
		if (fadeTime > 0)
		{
			this->fadeSpeed = -1.0f / fadeTime;
		}
		else
		{
			this->fadeTime = 0.0f;
			this->fadeSpeed = 0.0f;
			if (pause)
			{
				alGetSourcef(this->sourceId, AL_SEC_OFFSET, &this->sampleOffset);
				if (this->sound->getCategory()->isStreamed())
				{
					alSourcePause(this->sourceId);
					this->sound->setSourceId(this->sourceId);
					((StreamSound*)this->sound)->unqueueBuffers();
				}
				else
				{
					alSourceStop(this->sourceId);
				}
				if (!this->isLocked())
				{
					this->sourceId = 0;
				}
			}
			else
			{
				alSourceStop(this->sourceId);
				if (this->sound->getCategory()->isStreamed())
				{
					this->sound->setSourceId(this->sourceId);
					((StreamSound*)this->sound)->rewindStream();
				}
				this->unbind();
			}
		}
		this->paused = pause;
	}

	void Source::unbind()
	{
		if (!this->isLocked())
		{
			this->sourceId = 0;
			this->bound = false;
		}
	}
	
/******* PROPERTIES ****************************************************/

	void Source::setGain(float gain)
	{
		this->gain = gain;
		if (this->sourceId != 0)
		{
			alSourcef(this->sourceId, AL_GAIN, this->gain *
				this->sound->getCategory()->getGain() * xal::mgr->getGlobalGain());
		}
	}

	unsigned int Source::getBuffer()
	{
		return this->sound->getBuffer();
	}
	
	bool Source::isPlaying()
	{
		if (this->sourceId == 0)
		{
			return false;
		}
		if (this->sound->getCategory()->isStreamed())
		{
			return (!this->isPaused());
		}
		int state;
		alGetSourcei(this->sourceId, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	bool Source::isPaused()
	{
		return (this->paused && !this->isFading());
	}
	
	bool Source::isFading()
	{
		return (this->fadeSpeed != 0);
	}

	bool Source::isFadingIn()
	{
		return (this->fadeSpeed < 0);
	}

	bool Source::isFadingOut()
	{
		return (this->fadeSpeed < 0);
	}

}

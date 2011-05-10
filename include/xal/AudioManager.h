/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @author  Ivan Vucica
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Provides an interface for audio managers.

#ifndef XAL_AUDIO_MANAGER_H
#define XAL_AUDIO_MANAGER_H

#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hthread.h>

#include "xalExport.h"

#define STREAM_BUFFER_COUNT 4
#define STREAM_BUFFER_SIZE 32768

namespace xal
{
	enum Format
	{
#if HAVE_M4A
		M4A,
#endif
#if HAVE_MP3
		MP3,
#endif
#if HAVE_OGG
		OGG,
#endif
#if HAVE_SPX
		SPX,
#endif
#if HAVE_WAV
		WAV,
#endif
		UNKNOWN
	};

	enum HandlingMode
	{
		/// @brief Handles on Sound creation, keeps results in memory.
		FULL = 0,
		/// @brief Handles when first need arises, keeps results in memory.
		LAZY = 1,
		/// @brief Handles when first need arises, removes from memory when not needed anymore.
		ON_DEMAND = 2,
		/// @brief Handles streamed.
		STREAMED = 3
	};

	class Buffer;
	class Category;
	class Player;
	class Sound;
	class Source;

	class xalExport AudioManager
	{
	public:
		AudioManager(chstr systemName, unsigned long backendId, bool threaded = false, float updateTime = 0.01f, chstr deviceName = "");
		virtual ~AudioManager();
		void clear();
		
		hstr getName() { return this->name; }
		bool isEnabled() { return this->enabled; }
		hstr getDeviceName() { return this->deviceName; }
		float getUpdateTime() { return this->updateTime; }
		float getGlobalGain() { return this->gain; }
		void setGlobalGain(float value);

		static void update();
		void update(float k);
		void lockUpdate();
		void unlockUpdate();

		Category* createCategory(chstr name, HandlingMode loadMode = FULL, HandlingMode decodeMode = FULL);
		Category* getCategoryByName(chstr name);
		float getCategoryGain(chstr category);
		void setCategoryGain(chstr category, float gain);
		
		Sound* createSound(chstr filename, chstr categoryName, chstr prefix = "");
		Sound* getSound(chstr name);
		void destroySound(Sound* sound);
		void destroySoundsWithPrefix(chstr prefix);
		harray<hstr> createSoundsFromPath(chstr path, chstr prefix = "");
		harray<hstr> createSoundsFromPath(chstr path, chstr category, chstr prefix);

		Player* createPlayer(chstr name);
		void destroyPlayer(Player* player);
		virtual Buffer* _createBuffer(chstr filename, HandlingMode loadMode, HandlingMode decodeMode);
		virtual Source* _createSource(chstr filename, Format format);

		void play(chstr name, float fadeTime = 0.0f, bool looping = false, float gain = 1.0f);
		void stop(chstr name, float fadeTime = 0.0f);
		void stopFirst(chstr name, float fadeTime = 0.0f);
		void stopAll(float fadeTime = 0.0f);
		// TODO - pauses everything and resumes the right players / pauses and resumes entire audio module
		//void pauseAll(float fadeTime = 0.0f);
		//void resumeAll(float fadeTime = 0.0f);
		void stopCategory(chstr name, float fadeTime = 0.0f);
		bool isAnyPlaying(chstr name);
		bool isAnyFading(chstr name);
		bool isAnyFadingIn(chstr name);
		bool isAnyFadingOut(chstr name);

	protected:
		unsigned long backendId;
		hstr name;
		bool enabled;
		hstr deviceName;
		float updateTime;
		float gain;
		hmap<hstr, Category*> categories;
		harray<Player*> players;
		harray<Player*> managedPlayers;
		hmap<hstr, Sound*> sounds;
		hthread* thread;
		bool updating;
		
		void _setupThread();
		void _destroyThread();
		Player* _createManagedPlayer(chstr name);
		void _destroyManagedPlayer(Player* player);

		virtual Player* _createAudioPlayer(Sound* sound, Buffer* buffer);
		
	};
	
	xalExport extern xal::AudioManager* mgr;

}

#endif

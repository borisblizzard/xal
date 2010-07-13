/************************************************************************************\
This source file is part of the KS(X) audio library                                  *
For latest info, see http://libatres.sourceforge.net/                                *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef XAL_SIMPLESOUND_H
#define XAL_SIMPLESOUND_H

#include <hltypes/hstring.h>
#include "xalExport.h"
#include "Sound.h"

namespace xal
{
	class xalExport SimpleSound : public Sound
	{
	public:
		SimpleSound(chstr name, chstr category, chstr prefix = "");
		~SimpleSound();
		
		bool load();
		
		unsigned int getBuffer() { return this->buffer; }
		
	protected:
		unsigned int buffer;
		
		bool _loadOgg();
		
	};

}

#endif

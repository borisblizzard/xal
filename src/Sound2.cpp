/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/harray.h>
#include <hltypes/hfile.h>
#include <hltypes/hstring.h>

#include "Sound2.h"
#include "Category.h"
#include "xal.h"

namespace xal
{
	Sound2::Sound2(chstr filename, Category* category, chstr prefix) : duration(0.0f)
	{
		this->filename = filename;
		this->virtualFilename = this->filename;
		this->category = category;
		// extracting filename without extension and prepending the prefix
		this->name = prefix + hstr(filename).replace("\\", "/").rsplit("/").pop_back().rsplit(".", 1).pop_front();
	}

	Sound2::~Sound2()
	{
		xal::log("destroying sound " + this->name);
	}
	
	hstr Sound2::_findLinkedFile()
	{
		if (!hfile::exists(this->filename))
		{
			return this->filename;
		}
		harray<hstr> newFolders = hfile::hread(this->filename).split("/");
		harray<hstr> folders = this->filename.split("/");
		folders.pop_back();
		foreach (hstr, it, newFolders)
		{
			if ((*it) != "..")
			{
				folders += (*it);
			}
			else
			{
				folders.pop_back();
			}
		}
		return folders.join("/");
	}

}
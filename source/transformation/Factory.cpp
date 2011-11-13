/*
 * Copyright (c) 2009-2011, Albertas Vyšniauskas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of the software author nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Factory.h"

#include "ColorVisionDeficiency.h"
#include "GammaModification.h"

#include <string.h>

namespace transformation {

boost::shared_ptr<Transformation> Factory::create(const char *type)
{
	if (strcmp(ColorVisionDeficiency::getName(), type) == 0){
		return boost::shared_ptr<Transformation>(new ColorVisionDeficiency());
	}
	if (strcmp(GammaModification::getName(), type) == 0){
		return boost::shared_ptr<Transformation>(new GammaModification());
	}
	return boost::shared_ptr<Transformation>();
}

std::vector<Factory::TypeInfo> Factory::getAllTypes()
{
	std::vector<TypeInfo> result;
	result.push_back(TypeInfo(ColorVisionDeficiency::getName(), ColorVisionDeficiency::getReadableName()));
	result.push_back(TypeInfo(GammaModification::getName(), GammaModification::getReadableName()));
	return result;
}

Factory::TypeInfo::TypeInfo(const char *name_, const char *human_name_):
	name(name_), human_name(human_name_)
{
}

}


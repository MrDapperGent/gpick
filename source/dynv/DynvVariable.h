/*
 * Copyright (c) 2009-2016, Albertas Vyšniauskas
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

#ifndef DYNVVARIABLE_H_
#define DYNVVARIABLE_H_

#include <stdint.h>
#ifndef _MSC_VER
#include <stdbool.h>
#endif

struct dynvHandler;
struct dynvVariable
{
	char* name;
	struct dynvHandler* handler;
	union{
		void* ptr_value;
		bool bool_value;
		int32_t int_value;
		float float_value;
	};
	enum class Flag: uintptr_t
	{
		none = 0,
		no_save = 1,
		read_only = 2,
	}flags;
	dynvVariable *next;
};

dynvVariable* dynv_variable_create(const char* name, dynvHandler* handler);
void dynv_variable_destroy(dynvVariable* variable);
void dynv_variable_destroy_data(dynvVariable* variable);

dynvVariable::Flag operator&(dynvVariable::Flag x, dynvVariable::Flag y);
bool operator!=(dynvVariable::Flag x, dynvVariable::Flag y);

#endif /* DYNVVARIABLE_H_ */

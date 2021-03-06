/*
 * Copyright (c) 2009-2017, Albertas Vyšniauskas
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

#ifndef GPICK_CONVERTERS_H_
#define GPICK_CONVERTERS_H_
#include <map>
#include <vector>
struct ColorObject;
struct Converter;
struct Color;
struct Converters
{
	enum class Type
	{
		display,
		colorList,
		copy,
	};
	Converters();
	~Converters();
	void add(Converter *converter);
	const std::vector<Converter*> &all() const;
	const std::vector<Converter*> &allCopy() const;
	const std::vector<Converter*> &allPaste() const;
	Converter *byName(const char *name) const;
	Converter *display() const;
	Converter *colorList() const;
	void display(const char *name);
	void colorList(const char *name);
	void display(Converter *converter);
	void colorList(Converter *converter);
	Converter *firstCopy() const;
	Converter *firstCopyOrAny() const;
	Converter *byNameOrFirstCopy(const char *name) const;
	std::string serialize(ColorObject *color_object, Type type);
	std::string serialize(const Color &color, Type type);
	bool deserialize(const char *value, ColorObject **color_object);
	void rebuildCopyPasteArrays();
	void reorder(const char **names, size_t count);
	bool hasCopy() const;
	private:
	std::map<std::string, Converter*> m_converters;
	std::vector<Converter*> m_all_converters;
	std::vector<Converter*> m_copy_converters;
	std::vector<Converter*> m_paste_converters;
	Converter *m_display_converter;
	Converter *m_color_list_converter;
};
#endif /* GPICK_CONVERTERS_H_ */

/*******************************************************************************
 * Copyright (c) 2011, Jean-David Gadina - www.xs-labs.com
 * Distributed under the Boost Software License, Version 1.0.
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/
 
/* $Id$ */

#include "MP4.MDAT.h"
#include <memory>
#include "..\\tinyxml2.h"
#include "..\\Base64.h"

using namespace MP4;
using namespace tinyxml2;
          
MDAT::MDAT( void )
{
    this->_type.append( "MDAT" );
}

std::string MDAT::description( void )
{
    std::ostringstream o;
    
	o << this->_type;
    
    return o.str();
}

void MDAT::processData( MP4::BinaryStream * stream, uint64_t length )
{	
	if (stream->good())
	{
		//print data of mdat box!
		std::unique_ptr<char> str(new char[length + 1]);
		stream->read(str.get(), length);
		std::cout << "Content of mdat box is: " << std::endl;
		std::cout << str.get() << std::endl;

		//parse
		XMLDocument doc;
		XMLError err = doc.Parse(str.get());	//parse xml string
		XMLNode *rootnode = doc.FirstChild();	//get root node

		//search for metadata element in the xml string
		XMLElement *element = rootnode->NextSibling()->FirstChildElement("head")->FirstChildElement("metadata");
		
		//search for smtpe:image node
		for (const XMLNode* node = element->FirstChild(); node; node = node->NextSibling()) {
			const XMLElement * data = node->ToElement();
			const char* encoded = data->GetText();
			std::string strFilename(data->Attribute("xml:id"));
			strFilename.append(".");
			strFilename.append(data->Attribute("imagetype"));
			FILE *out = nullptr;
			fopen_s(&out, strFilename.c_str(), "wb"); // open output file
			if (out)
			{
				size_t len = strlen(encoded); // size of the encoded string as xml data
				size_t outputlen; //size of output string
				std::unique_ptr<unsigned char> base64DecodeOutput(new unsigned char[len + 1]);
				//decode the encoded string
				base64decode((char *)encoded, len, base64DecodeOutput.get(), &outputlen);
				//write it into the file
				fwrite(base64DecodeOutput.get(), sizeof(unsigned char), outputlen, out);
				//close the file
				fclose(out);
			}
		}
	}
}

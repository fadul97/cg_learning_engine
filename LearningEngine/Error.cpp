/**********************************************************************************
// Error (Source File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Handling errors Class
//
**********************************************************************************/

// ---------------------------------------------------------------------------------
// Includes

#include "Error.h"              // Error handling
#include <comdef.h>             // COM definitions for C++
#include <sstream>              // String buffer for text

// -------------------------------------------------------------------------------
// Constructor
LearningEngine::Error::Error() : hres_code(S_OK), line_num(-1)
{
}

// -------------------------------------------------------------------------------
// Constructor with parameters
LearningEngine::Error::Error(HRESULT hr, const std::string& func, const std::string& file, int line)
	: hres_code(hr), func_name(func), line_num(line)
{
	size_t pos = file.find_last_of('\\');

	if (pos != std::string::npos)
		file_name = file.substr(pos + 1);
}

// -------------------------------------------------------------------------------
// Write error message
std::string LearningEngine::Error::ToString() const
{
	_com_error err(hres_code);

	std::stringstream text;
	text << func_name
		<< " failed in " << file_name
		<< ", line " << std::to_string(line_num)
		<< ":\n" << err.ErrorMessage();

	return text.str();
}
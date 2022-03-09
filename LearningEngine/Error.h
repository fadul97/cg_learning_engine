/**********************************************************************************
// Error (Header File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Handling errors Class
//
**********************************************************************************/

#ifndef LE_ERROR_H
#define LE_ERROR_H

// ---------------------------------------------------------------------------------
// Includes

#include <Windows.h>																			// Windows functions
#include <string>																				// Type string

namespace LearningEngine
{
	class Error
	{
	public:
		Error();																				// Constructor
		Error(HRESULT hr, const std::string& func, const std::string& file, int line);			// Constructor with parameters
		std::string ToString() const;															// Write error message

	private:
		HRESULT hres_code;																		// Windows error code
		std::string func_name;																	// Function name where error occurred
		std::string file_name;																	// File name where error occurred
		int line_num;																			// Error line number
	};
}

// ---------------------------------------------------------------------------------

// Code needs to be a macro because the file name, line 
// and function need to be captured at the local error
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)										\
{																\
	HRESULT hr = (x);											\
																\
	if (FAILED(hr))												\
		throw Error(hr, __func__, __FILE__, __LINE__);			\
}

#endif

#endif // !LE_ERROR_H


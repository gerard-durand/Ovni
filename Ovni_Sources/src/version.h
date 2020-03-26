#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.03";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 1;
	static const long BUILD  = 9;
	static const long REVISION  = 91;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 121;
	#define RC_FILEVERSION 5,1,9,91
	#define RC_FILEVERSION_STRING "5, 1, 9, 91\0"
	static const char FULLVERSION_STRING [] = "5.1.9.91";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 43;
	

}
#endif //VERSION_H

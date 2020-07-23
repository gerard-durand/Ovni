#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "23";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.07";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 2;
	static const long BUILD  = 13;
	static const long REVISION  = 3305;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 664;
	#define RC_FILEVERSION 5,2,13,3305
	#define RC_FILEVERSION_STRING "5, 2, 13, 3305\0"
	static const char FULLVERSION_STRING [] = "5.2.13.3305";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 4;
	

}
#endif //VERSION_H

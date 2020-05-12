#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.05";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 2;
	static const long BUILD  = 4;
	static const long REVISION  = 1289;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 345;
	#define RC_FILEVERSION 5,2,4,1289
	#define RC_FILEVERSION_STRING "5, 2, 4, 1289\0"
	static const char FULLVERSION_STRING [] = "5.2.4.1289";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 58;
	

}
#endif //VERSION_H

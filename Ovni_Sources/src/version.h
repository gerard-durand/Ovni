#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.06";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 2;
	static const long BUILD  = 10;
	static const long REVISION  = 2303;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 508;
	#define RC_FILEVERSION 5,2,10,2303
	#define RC_FILEVERSION_STRING "5, 2, 10, 2303\0"
	static const char FULLVERSION_STRING [] = "5.2.10.2303";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 34;
	

}
#endif //VERSION_H

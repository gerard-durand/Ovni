#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.12";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 3;
	static const long BUILD  = 8;
	static const long REVISION  = 8517;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1553;
	#define RC_FILEVERSION 5,3,8,8517
	#define RC_FILEVERSION_STRING "5, 3, 8, 8517\0"
	static const char FULLVERSION_STRING [] = "5.3.8.8517";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 19;
	

}
#endif //VERSION_H

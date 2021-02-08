#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "03";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2021";
	static const char UBUNTU_VERSION_STYLE[] =  "21.02";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 3;
	static const long BUILD  = 18;
	static const long REVISION  = 9295;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1560;
	#define RC_FILEVERSION 5,3,18,9295
	#define RC_FILEVERSION_STRING "5, 3, 18, 9295\0"
	static const char FULLVERSION_STRING [] = "5.3.18.9295";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 25;
	

}
#endif //VERSION_H

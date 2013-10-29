/**
	@class gong::Dictionary

	@brief	A map that can be accessed either by key or sequentially.

	The syntax map[key] = value is explicitly avoided.
	When key does not exist, an empty value is returned:
		If value is a pointer, its value is 0
		If value is a class, a reference to an internal empty value is returned
 */

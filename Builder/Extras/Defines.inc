#define SEM(%0,%1) SendClientMessage(%0,X11_GRAY,%1) 					// SEM = Send Error Message by 	Myself
#define Loop(%0,%1) for(new %0 = 0; %0 < %1; %0++)                      // Loop                     by 	Myself
#define IsNull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))    // IsNull macro 			by 	Y_Less
#define strToLower(%0) \
    for(new i; %0[i] != EOS; ++i) \
        %0[i] = ('A' <= %0[i] <= 'Z') ? (%0[i] += 'a' - 'A') : (%0[i])  // strToLower 				by 	RyDeR`
#define RGBAToInt(%0,%1,%2,%3) \
	((16777216 * (%0)) + (65536 * (%1)) + (256 * (%2)) + (%3))          // RGBAToInt 				by 	RyDeR`
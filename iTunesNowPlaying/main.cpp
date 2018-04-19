#include "np.h"
#include <iostream>

void main()
{
	try {
		CoInitializeEx(0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);

		auto np = new NowPlaying();

		getchar();
		delete np;
		CoUninitialize();
	}
	catch (NowPlayingException e) {
		printf("‚È‚É‚©‚ª‚¨‚«‚½ %s", e.what());
	}	
}
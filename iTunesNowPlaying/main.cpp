#include "np.h"
#include <iostream>

void main()
{
	try {
		auto np = new NowPlaying();

		do {
			if (np->update()) printf("Nowplaying %s - %s", np->get_artist(), np->get_title());
		} while (getchar() == '\n'); // Enter�����čX�V�C�Ȃɂ�������͂���Enter����ƏI��
	}
	catch (NowPlayingException e) {
		printf("%s", e);
	}
	
}
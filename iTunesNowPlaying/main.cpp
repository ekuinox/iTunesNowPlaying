#include "np.h"
#include <iostream>

void main()
{
	auto np = new NowPlaying();

	do {
		np->update();
		printf("Nowplaying %s - %s", np->get_artist(), np->get_title());
	} while (getchar() == '\n'); // Enter�����čX�V�C�Ȃɂ�������͂���Enter����ƏI��
	
}
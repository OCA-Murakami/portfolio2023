#include "../WinMain.h"
#include "../Class/Vector3.h"
#include "Effect.h"

Effect::Effect() :
	playingHandle(-1),isPlaying(false)
{
}

Effect::~Effect()
{
}

void Effect::SetPlay(const int _effectHandle, Vector3 _effectPos)
{
	isPlaying = true;
	playingHandle = PlayEffekseer3DEffect(_effectHandle);
	SetPosPlayingEffekseer3DEffect(playingHandle, _effectPos.x, _effectPos.y, _effectPos.z);
}

void Effect::Stop(const int _handle)
{
	StopEffekseer3DEffect(_handle);
}

void Effect::ReleasePlay()
{
	isPlaying = false;
}

bool Effect::IsPlay() const
{
	return isPlaying;
}

bool Effect::CheckPlayEnd() const
{
	if (IsEffekseer3DEffectPlaying(playingHandle) && IsPlay())
	{
		return true;
	}

	return false;
}

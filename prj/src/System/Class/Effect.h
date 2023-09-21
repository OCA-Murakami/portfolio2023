#pragma once
//------------------------------------------------
//!@class Effect�N���X
//!@brief �G�t�F�N�g�̍Đ��ɕK�v�ȑ�����܂Ƃ߂Ă�
//------------------------------------------------
class Effect
{
	int  playingHandle;		//!< �Đ��������G�t�F�N�g�̃n���h��������
	bool isPlaying;			//!< �d���Đ���h���p�i�G�t�F�N�g�ɂ���Ďg�p���Ȃ��ꍇ������j
public:
	Effect();		//!< �R���X�g���N�^
	~Effect();		//!< �f�X�g���N�^

	void SetPlay(const int _effectHandle, Vector3 _effectPos);
	
	void Stop(const int _handle);

	void ReleasePlay();
	bool IsPlay()		const;
	bool CheckPlayEnd() const;
};


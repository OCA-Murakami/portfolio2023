#pragma once
//-----------------------------------------------------------
//!@class Tutorial�N���X
//!@brief �`���[�g���A�����ɕ`�悷��e���b�v����������
//-----------------------------------------------------------
class Tutorial
{
	// -----------------------------------------------------
	// �萔
	// -----------------------------------------------------
	const int mWIDTHMAX  = 20;		//!< 1�s������̕�����
	const int mHEIGHTMAX = 3;		//!< �ő�s��
	const int mCHARABITE = 2;		//!< ANSI�`���̂P����������̃o�C�g��

	// -----------------------------------------------------
	// �O���t�@�C������s�P�ʂŎ擾���������������
	// -----------------------------------------------------
	std::vector<std::string> mLineBuf;						//!< �t�@�C������擾�����P�s���i�[����
	int						 mLineRefeNum;					//!< �e�s�Q�Ɨp�̔ԍ�
	int						 mLineCharaIdx;					//!< �s���̂P�����Q�Ɨp�̔ԍ�

	// -----------------------------------------------------
	// ��������P�����P�ʂň���
	// -----------------------------------------------------
	char mCharaBuf[3][20 * 2 + 1];			//!< �P�s��������P��������؂��Ċi�[����
	int  mCharaCursorX;						//!< X�����̎Q�ƈʒu
	int  mCharaCursorY;						//!< Y�����̎Q�ƈʒu




	int sec;

private:

	//! @brief �O���t�@�C������e���b�v�����[�h
	//! @return �P�s����؂��Ċi�[���ꂽ������
	std::vector<std::string> LoadTelops(std::string _filePath) const;

	//! @brief 1�����o�b�t�@�̏�����
	void CharaBufInit();

	//! @brief �`�悷��e���b�v�̉��s
	void NewLineTelops();

	//! @brief �`�悵�Ă���e���b�v���P�s�������������`�F�b�N
	bool IsMaxCharaWidth() const;

	//! @brief �P�s����̃e���b�v�`��I���`�F�b�N
	//! @return true : �P�s���̃e���b�v�`��I��		false : �܂��`�撆
	bool IsFinishDrawLine() const;

public:
	Tutorial();		//!< �R���X�g���N�^
	~Tutorial();	//!< �f�X�g���N�^

	//! @brief ������
	//! @param _filePath	�ǂݍ��ރt�@�C���̃p�X 
	void Init(std::string _filePath);

	void Update();

	void Draw();

	//! @brief �e�L�X�g�t�@�C������ǂݍ��񂾍s�����擾����
	//! @return mLineStr�̃T�C�Y
	size_t GetLineSize() const;
};


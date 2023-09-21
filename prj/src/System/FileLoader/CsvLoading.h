#pragma once
//---------------------------------------------------------------------------
//!	@file	CsvLoading.h
//! @brief	CSV�t�@�C���̓ǂݍ���
//---------------------------------------------------------------------------
#include"../../Game/BossStatus.h"

//! @brief [,]��؂�ŕ�������擾����
//! @param _input		CSV����擾�����P�s�̕�����
//! @param _delimiter	��؂镶���i��{�I�Ɂu,�v������j
//! @return				��؂�ꂽ����������ꂼ��i�[����vector<string>�^�̒l
std::vector<std::string> split(std::string& _input, char _delimiter);

//! @brief �{�X�̃X�e�[�^�X���擾����
//! @param _fileName	�J���t�@�C���̃p�X
void LoadStatus(const char* _fileName, BossStatus& _status);

//! @brief �{�X�̓����蔻��̐ݒ���擾���� 
void LoadCollData(const char* _fileName, BossCollInfo& _collInfo);

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "CsvLoading.h"

// CSV�t�@�C������u,�v��؂�Ŏ擾����
std::vector<std::string> split(std::string& _input, char _delimiter)
{
	// getline �Ŏ擾�����f�[�^�� stringstream�ɕϊ�����
	std::istringstream stream(_input);
	std::string field;
	std::vector<std::string> result;

	// stream�ɂ���P�s���@���@_delimiter�łP������؂��ā@���@field�ɓ����
	while (std::getline(stream, field, _delimiter))
	{
		result.emplace_back(field);
	}

	return result;
}

void LoadStatus(const char* _fileName, BossStatus& _status)
{
	// �t�@�C�����J��
	std::ifstream ifs(_fileName);
	// �P�s�S�Ă̕����������
	std::string lineString;

	// �t�@�C���̂P�s�ڂ�ǂݍ��܂Ȃ����߂̕ϐ�
	int fakeNum = 0;

	while (std::getline(ifs, lineString))
	{
		// �擾�����P�s���J���}��؂�Ŕz��ɓ����
		std::vector<std::string> strVec = split(lineString, ',');

		// �P�s�ڂ͔�΂���������
		if (fakeNum == 0)
		{
			++fakeNum;
			continue;
		}

		// �J���}��؂�Ŏ擾�����l�����ꂼ��ϐ��ɓ���Ă���
		for (int i = 0; i < strVec.size(); ++i)
		{
			// ��f�[�^�������ꍇ�͎��̃f�[�^��
			if (strVec.at(i).empty()) continue;

			switch (i)
			{
				// HP
			case BOSSSTATUSTYPE::HpMax:
				_status.hpMax = std::stof(strVec.at(i));
				break;

				// ��U����
			case BOSSSTATUSTYPE::LightPower:
				_status.attackPower[BOSSATTACKTYPE::Light] = std::stof(strVec.at(i));
				break;

				// ���U����
			case BOSSSTATUSTYPE::HeavyPower:
				_status.attackPower[BOSSATTACKTYPE::Heavy] = std::stof(strVec.at(i));
				break;

				// �R���{�U����
			case BOSSSTATUSTYPE::ComboPower:
				_status.attackPower[BOSSATTACKTYPE::Combo] = std::stof(strVec.at(i));
				break;

				// �W�����v�U����
			case BOSSSTATUSTYPE::JumpPower:
				_status.attackPower[BOSSATTACKTYPE::Jump] = std::stof(strVec.at(i));
				break;

				// �ړ����x
			case BOSSSTATUSTYPE::MoveSpeed:
				_status.moveSpeed = std::stof(strVec.at(i));

				// ��U�����̓����蔻��T�C�Y
			case BOSSSTATUSTYPE::CollSize:
				_status.collSize = std::stof(strVec.at(i));
				break;

			default:
				break;
			}
		}
	}
	ifs.close();

}

void LoadCollData(const char* _fileName, BossCollInfo& _collInfo)
{
	// �t�@�C�����J��
	std::ifstream ifs(_fileName);
	// �P�s�S�Ă̕����������
	std::string lineString;

	// �t�@�C���̂P�s�ڂ�ǂݍ��܂Ȃ����߂̕ϐ�
	int fakeNum = 0;

	while (std::getline(ifs, lineString))
	{
		// �擾�����P�s���J���}��؂�Ŕz��ɓ����
		std::vector<std::string> strVec = split(lineString, ',');

		// �P�s�ڂ͔�΂���������
		if (fakeNum == 0)
		{
			++fakeNum;
			continue;
		}

		// �J���}��؂�Ŏ擾�����l�����ꂼ��ϐ��ɓ���Ă���
		for (int i = 0; i < strVec.size(); ++i)
		{
			// ��f�[�^�������ꍇ�͎��̃f�[�^��
			if (strVec.at(i).empty()) continue;

			// ���ꂼ��̎󂯎���֊i�[����
			switch (i)
			{
				// �{�f�B
			case BOSSCOLLTYPE::Body:
				_collInfo.bodySize = std::stof(strVec.at(i));
				break;

				// ���̐�[X���W
			case BOSSCOLLTYPE::FistTopX:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Top].x = std::stof(strVec.at(i));
				break;

				// ���̐�[Y���W
			case BOSSCOLLTYPE::FistTopY:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Top].y = std::stof(strVec.at(i));
				break;

				// ���̐�[Z���W
			case BOSSCOLLTYPE::FistTopZ:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Top].z = std::stof(strVec.at(i));
				break;

				// ���̍��{X���W
			case BOSSCOLLTYPE::FistUnderX:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Under].x = std::stof(strVec.at(i));
				break;

				// ���̍��{Y���W
			case BOSSCOLLTYPE::FistUnderY:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Under].y = std::stof(strVec.at(i));
				break;

				// ���̍��{Z���W
			case BOSSCOLLTYPE::FistUnderZ:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Under].z = std::stof(strVec.at(i));
				break;

				// ���̃T�C�Y
			case BOSSCOLLTYPE::FistSize:
				_collInfo.fistSize = std::stof(strVec.at(i));
				break;

				// �܂̐�[X���W
			case BOSSCOLLTYPE::ClawTopX:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Top].x = std::stof(strVec.at(i));
				break;

				// �܂̐�[Y���W
			case BOSSCOLLTYPE::ClawTopY:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Top].y = std::stof(strVec.at(i));
				break;

				// �܂̐�[Z���W
			case BOSSCOLLTYPE::ClawTopZ:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Top].z = std::stof(strVec.at(i));
				break;

				// �܂̍��{X���W
			case BOSSCOLLTYPE::ClawUnderX:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Under].x = std::stof(strVec.at(i));
				break;

				// �܂̍��{Y���W
			case BOSSCOLLTYPE::ClawUnderY:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Under].y = std::stof(strVec.at(i));
				break;

				// �܂̍��{Z���W
			case BOSSCOLLTYPE::ClawUnderZ:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Under].z = std::stof(strVec.at(i));
				break;

				// �܂̃T�C�Y
			case BOSSCOLLTYPE::ClawSize:
				_collInfo.clawSize = std::stof(strVec.at(i));
				break;
			}
		}
	}
	ifs.close();
}

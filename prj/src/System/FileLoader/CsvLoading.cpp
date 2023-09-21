#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "CsvLoading.h"

// CSVファイルから「,」区切りで取得する
std::vector<std::string> split(std::string& _input, char _delimiter)
{
	// getline で取得したデータを stringstreamに変換する
	std::istringstream stream(_input);
	std::string field;
	std::vector<std::string> result;

	// streamにある１行を　→　_delimiterで１文ずつ区切って　→　fieldに入れる
	while (std::getline(stream, field, _delimiter))
	{
		result.emplace_back(field);
	}

	return result;
}

void LoadStatus(const char* _fileName, BossStatus& _status)
{
	// ファイルを開く
	std::ifstream ifs(_fileName);
	// １行全ての文字列を入れる
	std::string lineString;

	// ファイルの１行目を読み込まないための変数
	int fakeNum = 0;

	while (std::getline(ifs, lineString))
	{
		// 取得した１行をカンマ区切りで配列に入れる
		std::vector<std::string> strVec = split(lineString, ',');

		// １行目は飛ばしたいから
		if (fakeNum == 0)
		{
			++fakeNum;
			continue;
		}

		// カンマ区切りで取得した値をそれぞれ変数に入れていく
		for (int i = 0; i < strVec.size(); ++i)
		{
			// 空データだった場合は次のデータへ
			if (strVec.at(i).empty()) continue;

			switch (i)
			{
				// HP
			case BOSSSTATUSTYPE::HpMax:
				_status.hpMax = std::stof(strVec.at(i));
				break;

				// 弱攻撃力
			case BOSSSTATUSTYPE::LightPower:
				_status.attackPower[BOSSATTACKTYPE::Light] = std::stof(strVec.at(i));
				break;

				// 強攻撃力
			case BOSSSTATUSTYPE::HeavyPower:
				_status.attackPower[BOSSATTACKTYPE::Heavy] = std::stof(strVec.at(i));
				break;

				// コンボ攻撃力
			case BOSSSTATUSTYPE::ComboPower:
				_status.attackPower[BOSSATTACKTYPE::Combo] = std::stof(strVec.at(i));
				break;

				// ジャンプ攻撃力
			case BOSSSTATUSTYPE::JumpPower:
				_status.attackPower[BOSSATTACKTYPE::Jump] = std::stof(strVec.at(i));
				break;

				// 移動速度
			case BOSSSTATUSTYPE::MoveSpeed:
				_status.moveSpeed = std::stof(strVec.at(i));

				// 被攻撃時の当たり判定サイズ
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
	// ファイルを開く
	std::ifstream ifs(_fileName);
	// １行全ての文字列を入れる
	std::string lineString;

	// ファイルの１行目を読み込まないための変数
	int fakeNum = 0;

	while (std::getline(ifs, lineString))
	{
		// 取得した１行をカンマ区切りで配列に入れる
		std::vector<std::string> strVec = split(lineString, ',');

		// １行目は飛ばしたいから
		if (fakeNum == 0)
		{
			++fakeNum;
			continue;
		}

		// カンマ区切りで取得した値をそれぞれ変数に入れていく
		for (int i = 0; i < strVec.size(); ++i)
		{
			// 空データだった場合は次のデータへ
			if (strVec.at(i).empty()) continue;

			// それぞれの受け取り先へ格納する
			switch (i)
			{
				// ボディ
			case BOSSCOLLTYPE::Body:
				_collInfo.bodySize = std::stof(strVec.at(i));
				break;

				// 拳の先端X座標
			case BOSSCOLLTYPE::FistTopX:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Top].x = std::stof(strVec.at(i));
				break;

				// 拳の先端Y座標
			case BOSSCOLLTYPE::FistTopY:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Top].y = std::stof(strVec.at(i));
				break;

				// 拳の先端Z座標
			case BOSSCOLLTYPE::FistTopZ:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Top].z = std::stof(strVec.at(i));
				break;

				// 拳の根本X座標
			case BOSSCOLLTYPE::FistUnderX:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Under].x = std::stof(strVec.at(i));
				break;

				// 拳の根本Y座標
			case BOSSCOLLTYPE::FistUnderY:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Under].y = std::stof(strVec.at(i));
				break;

				// 拳の根本Z座標
			case BOSSCOLLTYPE::FistUnderZ:
				_collInfo.fistPos[BOSSATTACKCOLLTYPE::Under].z = std::stof(strVec.at(i));
				break;

				// 拳のサイズ
			case BOSSCOLLTYPE::FistSize:
				_collInfo.fistSize = std::stof(strVec.at(i));
				break;

				// 爪の先端X座標
			case BOSSCOLLTYPE::ClawTopX:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Top].x = std::stof(strVec.at(i));
				break;

				// 爪の先端Y座標
			case BOSSCOLLTYPE::ClawTopY:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Top].y = std::stof(strVec.at(i));
				break;

				// 爪の先端Z座標
			case BOSSCOLLTYPE::ClawTopZ:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Top].z = std::stof(strVec.at(i));
				break;

				// 爪の根本X座標
			case BOSSCOLLTYPE::ClawUnderX:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Under].x = std::stof(strVec.at(i));
				break;

				// 爪の根本Y座標
			case BOSSCOLLTYPE::ClawUnderY:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Under].y = std::stof(strVec.at(i));
				break;

				// 爪の根本Z座標
			case BOSSCOLLTYPE::ClawUnderZ:
				_collInfo.clawPos[BOSSATTACKCOLLTYPE::Under].z = std::stof(strVec.at(i));
				break;

				// 爪のサイズ
			case BOSSCOLLTYPE::ClawSize:
				_collInfo.clawSize = std::stof(strVec.at(i));
				break;
			}
		}
	}
	ifs.close();
}

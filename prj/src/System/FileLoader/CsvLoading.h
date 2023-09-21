#pragma once
//---------------------------------------------------------------------------
//!	@file	CsvLoading.h
//! @brief	CSVファイルの読み込み
//---------------------------------------------------------------------------
#include"../../Game/BossStatus.h"

//! @brief [,]区切りで文字列を取得する
//! @param _input		CSVから取得した１行の文字列
//! @param _delimiter	区切る文字（基本的に「,」を入れる）
//! @return				区切られた文字列をそれぞれ格納したvector<string>型の値
std::vector<std::string> split(std::string& _input, char _delimiter);

//! @brief ボスのステータスを取得する
//! @param _fileName	開くファイルのパス
void LoadStatus(const char* _fileName, BossStatus& _status);

//! @brief ボスの当たり判定の設定を取得する 
void LoadCollData(const char* _fileName, BossCollInfo& _collInfo);

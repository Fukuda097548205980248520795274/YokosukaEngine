#pragma once
#include "../../../Externals/nlohmann/json.hpp"
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include <variant>
#include <map>

class GlobalVariables
{
public:

	static GlobalVariables* GetInstance();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="gourpName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 項目な追加
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 項目な追加
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddItem(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 項目な追加
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 数値を取得する
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	int32_t GetIntValue(const std::string& groupName, const std::string& key);

	/// <summary>
	/// 数値を取得する
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	float GetFloatValue(const std::string& groupName, const std::string& key);

	/// <summary>
	/// 数値を取得する
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key);


private:

	GlobalVariables() = default;

	~GlobalVariables() = default;

	// 項目
	using Item = std::variant<int32_t, float, Vector3>;

	// グループ
	using Group = std::map<std::string, Item>;

	// 全データ
	std::map<std::string, Group> datas_;

	// Json
	using json = nlohmann::json;

	// 保存先のファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";
};


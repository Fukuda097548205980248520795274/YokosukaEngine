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


private:

	GlobalVariables() = default;

	~GlobalVariables() = default;

	// 項目
	struct Item
	{
		std::variant<int32_t, float, Vector3> value;
	};

	// グループ
	struct Group
	{
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;

	// Json
	using json = nlohmann::json;

	// 保存先のファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";
};


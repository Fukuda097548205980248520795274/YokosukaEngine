#include "GlobalVariables.h"

/// <summary>
/// インスタンスのGetter
/// </summary>
/// <returns></returns>
GlobalVariables* GlobalVariables::GetInstance()
{
	static GlobalVariables globalVariables;
	return &globalVariables;
}

/// <summary>
/// 更新処理
/// </summary>
void GlobalVariables::Update()
{
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar))
	{
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())return;


	// 各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin() ; itGroup != datas_.end() ; ++itGroup)
	{
		// グループ名を取得
		const std::string& groupName = itGroup->first;

		// グループを参照
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		// 各アイテムついて
		for (std::map<std::string, Item>::iterator itItem = group.begin(); itItem != group.end(); ++itItem)
		{
			// 項目名を取得
			const std::string& itemName = itItem->first;

			// 項目を参照
			Item& item = itItem->second;


			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item))
			{
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}

			// float型の値を保持していれば
			if (std::holds_alternative<float>(item))
			{
				float* ptr = std::get_if<float>(&item);
				ImGui::SliderFloat(itemName.c_str(), ptr, -100.0f, 100.0f);
			}

			// Vector3型の値を保持していれば
			if (std::holds_alternative<Vector3>(item))
			{
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}

		// 改行
		ImGui::Text("\n");
		if (ImGui::Button("Save"))
		{
			// jsonファイルを書き出しセーブする
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "Global Variables", 0);
		}

		ImGui::EndMenu();
	}


	ImGui::EndMenuBar();
	ImGui::End();
}

/// <summary>
/// グループの作成
/// </summary>
/// <param name="gourpName"></param>
void GlobalVariables::CreateGroup(const std::string& groupName)
{
	// 指定名のオブジェクトがなければ追加する
	datas_[groupName];
}

/// <summary>
/// ファイルに書き出し
/// </summary>
/// <param name="groupName"></param>
void GlobalVariables::SaveFile(const std::string& groupName)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	json root;
	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();

	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
		itItem != itGroup->second.end(); ++itItem)
	{
		// 各項目名の取得
		const std::string& itemName = itItem->first;

		// 項目の参照の取得
		Item& item = itItem->second;


		// int32_t型の場合
		if (std::holds_alternative<int32_t>(item))
		{
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item);
		}

		// float型の場合
		if (std::holds_alternative<float>(item))
		{
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item);
		}

		// Vector3型の場合
		if (std::holds_alternative<Vector3>(item))
		{
			// float型のjson配列登録
			Vector3 value = std::get<Vector3>(item);
			root[groupName][itemName] = json::array({ value.x , value.y , value.z });
		}
	}

	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath))
	{
		std::filesystem::create_directories(kDirectoryPath);
	}


	// 書き込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";

	// 書き込み用ファイルストリーム
	std::ofstream ofs;

	// ファイルは書き込み用に開く
	ofs.open(filePath);

	// ファイルオープン失敗
	if (ofs.fail())
	{
		std::string message = "Faild open data file for write";
		MessageBoxA(nullptr, message.c_str(), "Global Varibles", 0);
		assert(0);
		return;
	}

	// ファイルをjson文字列に書き込む
	ofs << std::setw(4) << root << std::endl;

	// ファイルを閉じる
	ofs.close();
}


/// <summary>
/// ディレクトリの全ファイル読み込み
/// </summary>
void GlobalVariables::LoadFiles()
{
	// 保存先ディレクトリのパスをローカル変数で宣言する
	std::filesystem::path dir(kDirectoryPath);

	// 保存先ディレクトリがないときはスキップする
	if (!std::filesystem::exists(kDirectoryPath))
	{
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it)
	{
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();

		// jsonファイル以外はスキップ
		if (extension.compare(".json") != 0)
		{
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

/// <summary>
/// ファイルから読み込む
/// </summary>
/// <param name="groupName"></param>
void GlobalVariables::LoadFile(const std::string& groupName)
{
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";

	// 読み込み用ファイルストリーム
	std::ifstream ifs;

	// ファイルを読み込みように開く
	ifs.open(filePath);

	// エラーハンドリング
	if (ifs.fail())
	{
		std::string message = "Faild open data file for load";
		MessageBoxA(nullptr, message.c_str(), "Global Varibles", 0);
		assert(0);
		return;
	}

	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;

	// ファイルを閉じる
	ifs.close();


	// グループを検索
	json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());


	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem)
	{
		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の場合
		if (itItem->is_number_integer())
		{
			// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}

		// float型の場合
		if (itItem->is_number_float())
		{
			// float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		}

		// 要素数3の配列であれば
		if (itItem->is_array() && itItem->size() == 3)
		{
			// float型のjson配列登録
			Vector3 value = { itItem->at(0) , itItem->at(1) , itItem->at(2) };
			SetValue(groupName, itemName, value);
		}
	}
}


/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <param name="value"></param>
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;

	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <param name="value"></param>
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;

	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <param name="value"></param>
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value)
{
	// グループの参照を取得
	Group& group = datas_[groupName];

	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;

	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}


/// <summary>
/// 項目な追加
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <param name="value"></param>
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 項目が未登録なら
	if (itGroup == datas_.end())
	{
		SetValue(groupName, key, value);
		return;
	}

	// 項目検索
	std::map<std::string, Item>::iterator itItem = itGroup->second.find(key);

	// 項目が未登録なら
	if (itItem == itGroup->second.end())
	{
		SetValue(groupName, key, value);
		return;
	}
}

/// <summary>
/// 項目な追加
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <param name="value"></param>
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 項目が未登録なら
	if (itGroup == datas_.end())
	{
		SetValue(groupName, key, value);
		return;
	}

	// 項目検索
	std::map<std::string, Item>::iterator itItem = itGroup->second.find(key);

	// 項目が未登録なら
	if (itItem == itGroup->second.end())
	{
		SetValue(groupName, key, value);
		return;
	}
}

/// <summary>
/// 項目な追加
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <param name="value"></param>
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 項目が未登録なら
	if (itGroup == datas_.end())
	{
		SetValue(groupName, key, value);
		return;
	}

	// 項目検索
	std::map<std::string, Item>::iterator itItem = itGroup->second.find(key);

	// 項目が未登録なら
	if (itItem == itGroup->second.end())
	{
		SetValue(groupName, key, value);
		return;
	}
}


/// <summary>
/// 数値を取得する
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <returns></returns>
int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	std::map<std::string, Item>::iterator itItem = itGroup->second.find(key);

	assert(itGroup != datas_.end());
	assert(itItem != itGroup->second.end());

	int32_t value = std::get<int32_t>(itItem->second);

	return value;
}

/// <summary>
/// 数値を取得する
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <returns></returns>
float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	std::map<std::string, Item>::iterator itItem = itGroup->second.find(key);

	assert(itGroup != datas_.end());
	assert(itItem != itGroup->second.end());

	float value = std::get<float>(itItem->second);

	return value;
}

/// <summary>
/// 数値を取得する
/// </summary>
/// <param name="groupName"></param>
/// <param name="key"></param>
/// <returns></returns>
Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key)
{
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	std::map<std::string, Item>::iterator itItem = itGroup->second.find(key);

	assert(itGroup != datas_.end());
	assert(itItem != itGroup->second.end());

	Vector3 value = std::get<Vector3>(itItem->second);

	return value;
}
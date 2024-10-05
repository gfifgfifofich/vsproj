#pragma once

#include <string>
#include <strstream>
#include <vector>
#include <map>
#include <glm/glm.hpp>
class DataStorage
{
public:

	std::map<std::string,std::map<std::string, std::string>> data;

	void AddObject(std::string ObjectName);
	void PopObject(std::string ObjectName);

	void PopProperty(std::string ObjectName, std::string Property);
	void AddProperty(std::string ObjectName, std::string Property, std::string value = "");

	std::map<std::string, std::string> GetObject(std::string ObjectName);
	std::vector<std::string> GetProperties(std::string ObjectName);

	std::string GetProperty(std::string ObjectName, std::string Property);
	int GetPropertyAsInt(std::string ObjectName, std::string Property);
	float GetPropertyAsFloat(std::string ObjectName, std::string Property);
	bool GetPropertyAsBool(std::string ObjectName, std::string Property);
	glm::vec2 GetPropertyAsVec2(std::string ObjectName, std::string Property);
	glm::vec3 GetPropertyAsVec3(std::string ObjectName, std::string Property);
	glm::vec4 GetPropertyAsVec4(std::string ObjectName, std::string Property);

	void SetProperty(std::string ObjectName, std::string Property, std::string value = "");
	void SetProperty(std::string ObjectName, std::string Property, bool value);
	void SetProperty(std::string ObjectName, std::string Property, int value);
	void SetProperty(std::string ObjectName, std::string Property, float value);
	void SetProperty(std::string ObjectName, std::string Property, glm::vec2 value);
	void SetProperty(std::string ObjectName, std::string Property, glm::vec3 value);
	void SetProperty(std::string ObjectName, std::string Property, glm::vec4 value);

	void Save(std::string filename);
	void Load(std::string filename);
	std::string ToString();
};
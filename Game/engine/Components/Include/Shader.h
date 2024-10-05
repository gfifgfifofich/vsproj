#pragma once
void LoadShader(unsigned int* dir, const char* vertexPath, const char* fragmentPath);
void UseShader(unsigned int Shader);
void SetShader1f(unsigned int* Shader, const char* paramName, float f);
void SetShader2f(unsigned int* Shader, const char* paramName, float f1, float f2);
void SetShader2f(unsigned int* Shader, const char* paramName, glm::vec2 vec2);

void SetShader3f(unsigned int* Shader, const char* paramName, float f1, float f2, float f3);
void SetShader3f(unsigned int* Shader, const char* paramName, glm::vec3 vec3);

void SetShader4f(unsigned int* Shader, const char* paramName, float f1, float f2, float f3, float f4);
void SetShader4f(unsigned int* Shader, const char* paramName, glm::vec4 vec4);

void SetShader1i(unsigned int* Shader, const char* paramName, int i);
void SetShaderBool(unsigned int* Shader, const char* paramName, bool b);

void BindTexture(unsigned int* Shader, const char* TextureName, unsigned int Texture, int level = 0);

void DetachShader();

class Shader
{
public:
	struct Uniform
	{
		std::string name;
		int type;
		int type_id;
		std::string TypeName;
	};

	int id = 0;//meaningless number
	std::string Name;//meaningless text
	std::string VertexPath;
	std::string FragmentPath;
	unsigned int program;

	std::vector<Uniform> Uniforms;
	std::vector<float> uniformfloat; //0
	std::vector<int> uniformint; //1
	std::vector<glm::vec2> uniformvec2; //2
	std::vector<glm::vec3> uniformvec3; //3
	std::vector<glm::vec4> uniformvec4; //4
	std::vector<unsigned int> uniformTextures; //5
	std::vector<unsigned int> MapTextures; 

	void GetUniformsFromFile(const char* filename);
	void UpdateUniforms();
	void Load();
	void ClearUniforms();
	void GetUniforms();
	void Delete();

};

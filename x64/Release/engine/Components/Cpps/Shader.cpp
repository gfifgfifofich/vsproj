#include "../Include/Helper.h"
#include "../Include/Shader.h"

void Shader::GetUniformsFromFile(const char* filename)
{
	std::ifstream f(filename);

	while (!f.eof())
	{
		char junk;
		char line[256];
		f.getline(line, 256);
		std::strstream s;
		s << line;


		if (line[0] == 'u' && line[1] == 'n' && line[2] == 'i' && line[3] == 'f' && line[4] == 'o' && line[5] == 'r' && line[6] == 'm' && line[7] == ' ')
		{
			//reading uniform
			if (line[8] == 'f' && line[9] == 'l' && line[10] == 'o' && line[11] == 'a' && line[12] == 't' && line[13] == ' ')
			{ // float
				Uniform un;
				int i = 14;
				while (i < 256 && line[i] != ' ' && line[i] != ';' && line[i] != '=')
				{
					un.name.push_back(line[i]);
					i++;
				}
				if (un.name[0] != 'e' && un.name[0] != 'n' && un.name[0] != '_')
				{
					un.type = 0;
					un.TypeName = "float";

					uniformfloat.push_back(0.0f);
					un.type_id = uniformfloat.size() - 1;
					Uniforms.push_back(un);
				}
			}
			if (line[8] == 'i' && line[9] == 'n' && line[10] == 't' && line[11] == ' ')
			{ // int
				Uniform un;
				int i = 12;
				while (i < 256 && line[i] != ' ' && line[i] != ';' && line[i] != '=')
				{
					un.name.push_back(line[i]);
					i++;
				}
				if (un.name[0] != 'e' && un.name[0] != 'n' && un.name[0] != '_')
				{
					un.type = 1;
					un.TypeName = "int";

					uniformint.push_back(0);
					un.type_id = uniformint.size() - 1;
					Uniforms.push_back(un);
				}
			}
			if (line[8] == 'b' && line[9] == 'o' && line[10] == 'o' && line[11] == 'l' && line[12] == ' ')
			{ // bool
				Uniform un;
				int i = 13;
				while (i < 256 && line[i] != ' ' && line[i] != ';' && line[i] != '=')
				{
					un.name.push_back(line[i]);
					i++;
				}
				if (un.name[0] != 'e' && un.name[0] != 'n' && un.name[0] != '_')
				{
					un.type = 1;
					un.TypeName = "bool";

					uniformint.push_back(0.0f);
					un.type_id = uniformint.size() - 1;
					Uniforms.push_back(un);
				}
			}
			if (line[8] == 'v' && line[9] == 'e' && line[10] == 'c' && line[11] == '2' && line[12] == ' ')
			{ // vec2
				Uniform un;
				int i = 13;
				while (i < 256 && line[i] != ' ' && line[i] != ';' && line[i] != '=')
				{
					un.name.push_back(line[i]);
					i++;
				}
				if (un.name[0] != 'e' && un.name[0] != 'n' && un.name[0] != '_')
				{
					un.type = 2;
					un.TypeName = "vec2";

					uniformvec2.push_back({ 0.0f,0.0f });
					un.type_id = uniformvec2.size() - 1;
					Uniforms.push_back(un);
				}
			}
			if (line[8] == 'v' && line[9] == 'e' && line[10] == 'c' && line[11] == '3' && line[12] == ' ')
			{ // vec3
				Uniform un;
				int i = 13;
				while (i < 256 && line[i] != ' ' && line[i] != ';' && line[i] != '=')
				{
					un.name.push_back(line[i]);
					i++;
				}
				if (un.name[0] != 'e' && un.name[0] != 'n' && un.name[0] != '_')
				{
					un.type = 3;
					un.TypeName = "vec3";

					uniformvec3.push_back({ 0.0f,0.0f,0.0f });
					un.type_id = uniformvec3.size() - 1;
					Uniforms.push_back(un);
				}
			}
			if (line[8] == 'v' && line[9] == 'e' && line[10] == 'c' && line[11] == '4' && line[12] == ' ')
			{ // vec4
				Uniform un;
				int i = 13;
				while (i < 256 && line[i] != ' ' && line[i] != ';' && line[i] != '=')
				{
					un.name.push_back(line[i]);
					i++;
				}
				if (un.name[0] != 'e' && un.name[0] != 'n' && un.name[0] != '_')
				{
					un.type = 4;
					un.TypeName = "vec4";

					uniformvec4.push_back({ 0.0f,0.0f,0.0f,0.0f });
					un.type_id = uniformvec4.size() - 1;
					Uniforms.push_back(un);
				}
			}
			if (line[8] == 's' && line[9] == 'a' && line[10] == 'm' && line[11] == 'p' && line[12] == 'l' && line[13] == 'e' && line[14] == 'r' && line[15] == '2' && line[16] == 'D' && line[17] == ' ')
			{ // sampler2D
				Uniform un;
				int i = 18;
				while (i < 256 && line[i] != ' ' && line[i] != ';' && line[i] != '=')
				{
					un.name.push_back(line[i]);
					i++;
				}
				if (un.name[0] != 'e' && un.name[0] != 'n' && un.name[0] != '_')
				{
					un.type = 5;
					un.TypeName = "Sampler2D (Texture)";

					uniformTextures.push_back(0);
					un.type_id = uniformTextures.size() - 1;
					Uniforms.push_back(un);
				}
			}
		}



	}

}

void Shader::UpdateUniforms()
{
	int a = 0;
	for (int i = 0; i < Uniforms.size(); i++)
	{
		if (Uniforms[i].type == 0)//float
			SetShader1f(&program, Uniforms[i].name.c_str(), uniformfloat[Uniforms[i].type_id]);
		if (Uniforms[i].type == 1)//int/bool
			SetShader1i(&program, Uniforms[i].name.c_str(), uniformint[Uniforms[i].type_id]);

		if (Uniforms[i].type == 2)//vec2
			SetShader2f(&program, Uniforms[i].name.c_str(), uniformvec2[Uniforms[i].type_id]);
		if (Uniforms[i].type == 3)//vec3
			SetShader3f(&program, Uniforms[i].name.c_str(), uniformvec3[Uniforms[i].type_id]);
		if (Uniforms[i].type == 4)//vec4
			SetShader4f(&program, Uniforms[i].name.c_str(), uniformvec4[Uniforms[i].type_id]);

		if (Uniforms[i].type == 5)//Sampler2d
		{
			BindTexture(&program, Uniforms[i].name.c_str(), uniformTextures[Uniforms[i].type_id], a);
			a++;
		}

	}
}
void Shader::Load()
{
	if (program != NULL)
		Delete();
	LoadShader(&program, VertexPath.c_str(), FragmentPath.c_str());



	//GetUniformsFromFile(VertexPath.c_str());



}
void Shader::ClearUniforms()
{

	Uniforms.clear();
	uniformfloat.clear();
	uniformint.clear();
	uniformvec2.clear();
	uniformvec3.clear();
	uniformvec4.clear();
	uniformTextures.clear();
}
void Shader::GetUniforms()
{

	GetUniformsFromFile(FragmentPath.c_str());
}
void Shader::Delete()
{


	glDeleteProgram(program);
	program = NULL;
}

void LoadShader(unsigned int* dir, const GLchar* vertexPath, const GLchar* fragmentPath)
{
	if (*dir != NULL)
	{
		glDeleteProgram(*dir);
		*dir = NULL;
	}

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];


	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);


	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << fragmentPath <<"  ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout <<  vertexPath <<"  ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	if (success)
	{
		GLuint Program = glCreateProgram();
		glAttachShader(Program, vertex);
		glAttachShader(Program, fragment);
		glLinkProgram(Program);

		glGetProgramiv(Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			glDeleteProgram(Program);
			Program = NULL;
		}

		*dir = Program;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void UseShader(unsigned int Shader)
{
	glUseProgram(Shader);
}
void SetShader1f(unsigned int* Shader, const char* paramName, float f)
{
	glUniform1f(glGetUniformLocation(*Shader, paramName), f);
}
void SetShader2f(unsigned int* Shader, const char* paramName, float f1, float f2)
{
	glUniform2f(glGetUniformLocation(*Shader, paramName), f1, f2);
}
void SetShader2f(unsigned int* Shader, const char* paramName, glm::vec2 vec2)
{
	glUniform2f(glGetUniformLocation(*Shader, paramName), vec2.x, vec2.y);
}

void SetShader3f(unsigned int* Shader, const char* paramName, float f1, float f2, float f3)
{
	glUniform3f(glGetUniformLocation(*Shader, paramName), f1, f2, f3);
}
void SetShader3f(unsigned int* Shader, const char* paramName, glm::vec3 vec3)
{
	glUniform3f(glGetUniformLocation(*Shader, paramName), vec3.x, vec3.y, vec3.z);
}

void SetShader4f(unsigned int* Shader, const char* paramName, float f1, float f2, float f3, float f4)
{
	glUniform4f(glGetUniformLocation(*Shader, paramName), f1, f2, f3, f4);
}
void SetShader4f(unsigned int* Shader, const char* paramName, glm::vec4 vec4)
{
	glUniform4f(glGetUniformLocation(*Shader, paramName), vec4.x, vec4.y, vec4.z, vec4.w);
}

void SetShader1i(unsigned int* Shader, const char* paramName, int i)
{
	glUniform1i(glGetUniformLocation(*Shader, paramName), i);
}
void SetShaderBool(unsigned int* Shader, const char* paramName, bool b)
{
	glUniform1i(glGetUniformLocation(*Shader, paramName), b);
}

void BindTexture(unsigned int* Shader, const char* TextureName, unsigned int Texture, int level)
{
	glActiveTexture(GL_TEXTURE0 + level);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(glGetUniformLocation(*Shader, TextureName), 0);
	glActiveTexture(GL_TEXTURE0);
}

void DetachShader()
{
	glUseProgram(0);
}

#include "../Include/Helper.h"
#include "../Include/Drawing.h"
#include "../Include/Text.h"

void LoadFont(const char* font,int size)
{

	 FT_Library ft;
	 if (FT_Init_FreeType(&ft))
	 	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	 FT_Face face;
	 if (FT_New_Face(ft, font, 0, &face))
	 	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	 FT_Set_Pixel_Sizes(face, 0, size);


	 if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	 	std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	 for (auto c : Characters)
	 {
	 	glDeleteTextures(1,&c.second.TextureID);

	 }
	 Characters.clear();

	 glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	 for (GLubyte c = 0; c < 128; c++)
	 {
	 	// Load character glyph
	 	if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	 	{
	 		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	 		continue;
	 	}
	 	// Generate texture
	 	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	 	glTexImage2D(
	 		GL_TEXTURE_2D,
	 		0,
	 		GL_RED,
	 		face->glyph->bitmap.width,
	 		face->glyph->bitmap.rows,
	 		0,
	 		GL_RED,
	 		GL_UNSIGNED_BYTE,
	 		face->glyph->bitmap.buffer
	 	);
	 	// Set texture options
	 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 	// Now store character for later use
	 	Character character = {
	 		texture,
	 		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows ),
	 		glm::ivec2(face->glyph->bitmap_left , face->glyph->bitmap_top ),
	 		static_cast<GLint>(face->glyph->advance.x)
	 	};
	 	Characters.insert(std::pair<GLchar, Character>(c, character));
	 	// Characters[c] = character;
	 }
	 FT_Done_Face(face);
	 FT_Done_FreeType(ft);
}
glm::vec2 getTextSize(std::string Text, GLfloat scale)
{
	glm::vec2 size = {0.0f,0.0f};
	for (int i = 0; i < Text.size(); i++)
	{
		size.x += (Characters[Text[i]].Advance >> 6) * scale;
		if (size.y < Characters[Text[i]].Size.y * scale) size.y = Characters[Text[i]].Size.y * scale;
	}
	size.x += Characters[Text[Text.size()-1]].Size.x * scale;
	//return { size.x* ScreenDivisorX, size.y * ScreenDivisorY };
	return { size.x , size.y };
}

// actually draws text
void _DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color)
{
	//glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	position -= CameraPosition;
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(
		(position.x) * ScreenDivisorX * CameraScale.x,
		(position.y) * ScreenDivisorY * CameraScale.y,
		0.0f));


	trans = glm::scale(trans, glm::vec3(CameraScale.x * ScaleMultiplyer, CameraScale.y * ScaleMultiplyer, 0.0f));



	// Activate corresponding render state
	UseShader(TextShader);
	glUniform4f(glGetUniformLocation(TextShader, "color"), color.x, color.y, color.z, color.w);
	glUniformMatrix4fv(glGetUniformLocation(TextShader, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	glUniform2f(glGetUniformLocation(TextShader, "scr"), (float)WIDTH, (float)HEIGHT);
	glUniform1i(glGetUniformLocation(TextShader, "flipY"), true);


	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(TextVAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = (x)+ch.Bearing.x * scale;
		GLfloat ypos = (y)-(ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color, int Z_Index, bool Additive)
{
	TextLine txtline;

	txtline.text = text;
	txtline.x = x;
	txtline.y = y;
	txtline.scale = scale;
	txtline.color = color;
	txtline.aboveEverything = false;
	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false
	SceneLayers[SLI].TextLines.push_back(txtline);


}
void DrawText(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color, int Z_Index, bool Additive)
{
	DrawText(text, position.x, position.y, scale, color, Z_Index, Additive);
}

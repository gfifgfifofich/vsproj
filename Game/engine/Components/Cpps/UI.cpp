
#include "../Include/Helper.h"
#include "../Include/Drawing.h"
#include "../Include/Text.h"
#include "../Include/Objects.h"
#include "../Include/Collisions/CircleToQuad.h"
#define UI_Implementation
#include "../Include/UI.h"

//void UI_NormalMapDraw(position, scale, NormalMap, rotation, Z_Index, texture);

//rotation not implemented, just twise size of object for bound checking
bool CheckScreenBounds(glm::vec2 position, glm::vec2 scale, float rotation = 0.0f)
{
	scale *=2.0f;

	if(position.y - scale.y < HEIGHT * 0.5f)
		return false;
	if(position.y + scale.y > HEIGHT * 0.5f)
		return false;
	
	if(position.x - scale.x < WIDTH * 0.5f)
		return false;
	if(position.x + scale.x > WIDTH * 0.5f)
		return false;

	return true;
}

void UI_DrawSmoothQuad(glm::vec2 position, glm::vec2 scale, float rotation , glm::vec4 color , int Z_Index , bool Additive , bool flipX , bool flipY )
{
	float aspx = ScreenDivisorX ;
	float aspy = ScreenDivisorY ;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);

	if(CheckScreenBounds(position,scale,rotation)) return;

	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false




	SceneLayers[SLI].SmoothQuadcolors.push_back(color);
	SceneLayers[SLI].SmoothQuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].SmoothQuadRotations.push_back(rotation);
}
void UI_NormalMapDraw(glm::vec2 position, glm::vec2 scale, unsigned int NormalMap, float rotation, int Z_Index, unsigned int Texture, bool Additive, float depth, unsigned int HeightMap)
{
	
	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false
	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;	
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);	
	
	if(CheckScreenBounds(position,scale,rotation)) return;
	int TQA = -1;
	for (int i = 0; i < SceneLayers[SLI].NormalMaps.size(); i++)
		if (SceneLayers[SLI].NormalMaps[i].material.NormalMap == NormalMap && SceneLayers[SLI].NormalMaps[i].material.Texture == Texture && SceneLayers[SLI].NormalMaps[i].material.HeightMap == HeightMap)
			TQA = i;
	if (TQA == -1)
	{
		TexturedQuadArray NewTQA;
		NewTQA.material.NormalMap = NormalMap;
		NewTQA.material.Texture = Texture;
		NewTQA.material.HeightMap = HeightMap;
		SceneLayers[SLI].NormalMaps.push_back(NewTQA);
		for (int i = 0; i < SceneLayers[SLI].NormalMaps.size(); i++)
			if (SceneLayers[SLI].NormalMaps[i].material.NormalMap == NormalMap && SceneLayers[SLI].NormalMaps[i].material.Texture == Texture && SceneLayers[SLI].NormalMaps[i].material.HeightMap == HeightMap)
				TQA = i;
	}
	SceneLayers[SLI].NormalMaps[TQA].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].NormalMaps[TQA].QuadRotations.push_back(rotation);
	SceneLayers[SLI].NormalMaps[TQA].QuadRotations.push_back(rotation);
	SceneLayers[SLI].NormalMaps[TQA].QuadDepth.push_back(depth);


}

void UI_DrawQuadWithMaterial(cube c, Material material, float rotation, glm::vec4 color,bool flipY, int Z_Index, bool Additive, float depth)
{


	glm::vec2 position = c.position;
	glm::vec2 scale = glm::vec2(c.width, c.height);

	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);
	if(CheckScreenBounds(position,scale,rotation)) return;


	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	int TQA = -1;

	for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
		if (SceneLayers[SLI].TexturedQuads[i].material == material)
			TQA = i;
	if (TQA == -1)
	{
		TexturedQuadArray NewTQA;
		NewTQA.material = material;
		SceneLayers[SLI].TexturedQuads.push_back(NewTQA);
		for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
			if (SceneLayers[SLI].TexturedQuads[i].material == material)
				TQA = i;
	}
	SceneLayers[SLI].TexturedQuads[TQA].Quadcolors.push_back(color);
	SceneLayers[SLI].TexturedQuads[TQA].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].TexturedQuads[TQA].QuadRotations.push_back(rotation);
	SceneLayers[SLI].TexturedQuads[TQA].QuadDepth.push_back(depth);



}
void UI_DrawQuadWithMaterial(glm::vec2 position, glm::vec2 scale, Material material, float rotation, glm::vec4 color,bool flipY, int Z_Index, bool Additive , float depth)
{


	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);

	if(CheckScreenBounds(position,scale,rotation)) return;

	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false


	int TQA = -1;

	for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
		if (SceneLayers[SLI].TexturedQuads[i].material == material)
			TQA = i;
	if (TQA == -1)
	{
		TexturedQuadArray NewTQA;
		NewTQA.material = material;
		SceneLayers[SLI].TexturedQuads.push_back(NewTQA);
		for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
			if (SceneLayers[SLI].TexturedQuads[i].material == material)
				TQA = i;
	}

	SceneLayers[SLI].TexturedQuads[TQA].Quadcolors.push_back(color);
	SceneLayers[SLI].TexturedQuads[TQA].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].TexturedQuads[TQA].QuadRotations.push_back(rotation);
	SceneLayers[SLI].TexturedQuads[TQA].QuadDepth.push_back(depth);

}

void UI_DrawTexturedQuad(glm::vec2 position, glm::vec2 scale, unsigned int texture, float rotation, glm::vec4 color,  int Z_Index, unsigned int NormalMap, bool Additive, bool flipX, bool flipY , float depth, unsigned int HeightMap)
{

	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);
	if(CheckScreenBounds(position,scale,rotation)) return;


	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false


	Material m;
	m.Texture = texture;
	m.NormalMap = NormalMap;
	m.Specular = 0;
	m.Reflective = 0;
	m.HeightMap = HeightMap;
	m.flipX = flipX;
	m.flipY = flipY;
	int TQA = -1;

	for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
		if (SceneLayers[SLI].TexturedQuads[i].material == m)
			TQA = i;
	if (TQA == -1)
	{
		TexturedQuadArray NewTQA;
		NewTQA.material = m;
		SceneLayers[SLI].TexturedQuads.push_back(NewTQA);
		for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
			if (SceneLayers[SLI].TexturedQuads[i].material == m)
				TQA = i;
	}
	SceneLayers[SLI].TexturedQuads[TQA].Quadcolors.push_back(color);
	SceneLayers[SLI].TexturedQuads[TQA].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].TexturedQuads[TQA].QuadRotations.push_back(rotation);
	SceneLayers[SLI].TexturedQuads[TQA].QuadDepth.push_back(depth);

}
void UI_DrawTexturedQuad(cube c, unsigned int texture, glm::vec4 color, float rotation, int Z_Index, unsigned int NormalMap, bool Additive, bool flipX, bool flipY, float depth, unsigned int HeightMap)
{


	glm::vec2 position = c.position;
	glm::vec2 scale = glm::vec2(c.width, c.height);

	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);

	if(CheckScreenBounds(position,scale,rotation)) return;

	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	Material m;
	m.Texture = texture;
	m.NormalMap = NormalMap;
	m.Specular = 0;
	m.Reflective = 0;
	m.HeightMap = HeightMap;
	m.flipX = flipX;
	m.flipY = flipY;
	int TQA = -1;

	for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
		if (SceneLayers[SLI].TexturedQuads[i].material == m)
			TQA = i;
	if (TQA == -1)
	{
		TexturedQuadArray NewTQA;
		NewTQA.material = m;
		SceneLayers[SLI].TexturedQuads.push_back(NewTQA);
		for (int i = 0; i < SceneLayers[SLI].TexturedQuads.size(); i++)
			if (SceneLayers[SLI].TexturedQuads[i].material == m)
				TQA = i;
	}
	SceneLayers[SLI].TexturedQuads[TQA].Quadcolors.push_back(color);
	SceneLayers[SLI].TexturedQuads[TQA].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].TexturedQuads[TQA].QuadRotations.push_back(rotation);
	SceneLayers[SLI].TexturedQuads[TQA].QuadDepth.push_back(depth);



}
void UI_DrawTexturedLine(unsigned int Texture, glm::vec2 p1, glm::vec2 p2, float width, glm::vec4 color, unsigned int NormalMap, int Z_Index , float depth, unsigned int HeightMap)
{
	glm::vec2 midpos = (p2 + p1) / 2.0f;
	float rotation = get_angle_between_points(p1, p2);
	glm::vec2 dif = p1 - p2;
	float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;
	UI_DrawTexturedQuad(midpos, glm::vec2(width, length), Texture, rotation, color, Z_Index, NormalMap,false,false,false,  depth,  HeightMap);
}
void UI_DrawCircle(glm::vec2 position, float r, glm::vec4 color, bool Lighted, unsigned int NormalMap, int Z_Index, bool Additive)
{
	glm::vec2 scale = glm::vec2(r, r);

	if (Lighted)
		UI_NormalMapDraw(position, scale, NormalMap, 0.0f, Z_Index);

	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);
	if(CheckScreenBounds(position,scale)) return;


	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	SceneLayers[SLI].CirclePosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].CircleRotations.push_back(0.0f);
	SceneLayers[SLI].Circlecolors.push_back(color);

}
void UI_DrawCircle(ball b, glm::vec4 color, bool Lighted, unsigned int NormalMap, int Z_Index, bool Additive)
{

	glm::vec2 position = b.position;
	float r = b.r;
	glm::vec2 scale = glm::vec2(r, r);
	if (Lighted)
		UI_NormalMapDraw(position, scale, NormalMap, 0.0f, Z_Index);

	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);
	if(CheckScreenBounds(position,scale)) return;

	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false


	SceneLayers[SLI].CirclePosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].CircleRotations.push_back(0.0f);
	SceneLayers[SLI].Circlecolors.push_back(color);
}
void UI_DrawCube(glm::vec2 position, glm::vec2 scale, float rotation, glm::vec4 color, bool Lighted, unsigned int NormalMap, int Z_Index, bool Additive)
{


	if (Lighted)
		UI_NormalMapDraw(position, scale, NormalMap, rotation, Z_Index);

	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);
	if(CheckScreenBounds(position,scale,rotation)) return;




	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	SceneLayers[SLI].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].QuadRotations.push_back(rotation);

	SceneLayers[SLI].Quadcolors.push_back(color);


}
void UI_DrawCube(cube c, glm::vec4 color, float rotation, bool Lighted, unsigned int NormalMap, int Z_Index, bool Additive)
{


	if (Lighted)
		UI_NormalMapDraw(c.position, glm::vec2(c.width, c.height), NormalMap, rotation, Z_Index);

	float aspx = ScreenDivisorX;
	float aspy = ScreenDivisorY;

	glm::vec2 position = c.position;
	position *= glm::vec2(aspx, aspy);
	glm::vec2 scale = glm::vec2(c.width, c.height) * glm::vec2(aspx, aspy);
	if(CheckScreenBounds(position,scale,rotation)) return;

	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	SceneLayers[SLI].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].QuadRotations.push_back(rotation);

	SceneLayers[SLI].Quadcolors.push_back(color);
}
void UI_DrawLine(glm::vec2 p1, glm::vec2 p2, float width, glm::vec4 color, bool Lighted, unsigned int NormalMap, int Z_Index)
{
	glm::vec2 midpos = (p2 + p1) / 2.0f;
	float rotation = get_angle_between_points(p1, p2);
	glm::vec2 dif = p1 - p2;
	float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;
	UI_DrawCube(midpos, glm::vec2(width, length), rotation, color, Lighted, NormalMap, Z_Index);
}
void UI_DrawBall(ball b, glm::vec4 Color1, glm::vec4 Color2, bool Lighted, unsigned int NormalMap, int Z_Index)
{

	glm::vec2 univec;
	univec.x = cos(b.rotation * 5) - sin(b.rotation * 5);
	univec.y = sin(b.rotation * 5) + cos(b.rotation * 5);
	UI_DrawLine(b.position - univec * b.r * 0.7f, b.position + univec * b.r * 0.7f, 3.0f, Color2, Lighted, CubeNormalMapTexture, Z_Index);
	UI_DrawLine(b.position + glm::vec2(-univec.y * b.r * 0.7f, univec.x * b.r * 0.7f), b.position + glm::vec2(univec.y * b.r * 0.7f, -univec.x * b.r * 0.7f), 3.0f, Color2, Lighted, CubeNormalMapTexture, Z_Index);
	UI_DrawCircle(b, Color1, Lighted, NormalMap, Z_Index - 1);
}
void _UI_DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color)
{
	//glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(
		(position.x) * ScreenDivisorX,
		(position.y) * ScreenDivisorY,
		0.0f));


	trans = glm::scale(trans, glm::vec3(ScaleMultiplyer, ScaleMultiplyer, 0.0f));



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

		if(CheckScreenBounds(glm::vec2(xpos,ypos) ,glm::vec2(w,h))) continue;
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

//UI returns size of object
glm::vec2 UI_DrawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color, int Z_Index, bool Additive)
{
	TextLine txtline;

	txtline.text = text;
	txtline.x = x;
	txtline.y = y;
	txtline.scale = scale;
	txtline.color = color;
	txtline.aboveEverything = false;
	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	SceneLayers[SLI].UI_TextLines.push_back(txtline);

	return getTextSize(text, scale);
}
//UI returns size of object
glm::vec2 UI_DrawText(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color, int Z_Index, bool Additive)
{
	UI_DrawText(text, position.x, position.y, scale, color, Z_Index, Additive);
	return getTextSize(text, scale);
}
//UI returns size of object
glm::vec2 UI_DrawTextOnPlate(std::string text, glm::vec2 position, GLfloat scale, glm::vec4 color, glm::vec4 platecolor, int Z_Index, bool Additive)
{
	UI_DrawText(text, position.x, position.y, scale, color, Z_Index, Additive);

	glm::vec2 scl = getTextSize(text, scale);
	UI_DrawCube(position + scl * 0.50f, scl * 0.6f, 0.0f, platecolor,false,NULL, Z_Index -1, Additive);
	return scl * 0.6f;
}

//UI returns size of object
glm::vec2 UI_CheckBox(bool* param, const char* text, glm::vec2 scrPosition, float scale, float textScale , glm::vec4 colorON , glm::vec4 ColorOFF, int Z_Index , bool Additive )
{


	scrPosition.x += scale;
	float r = scale  * 1.25;

	float cs = scale;
	bool hover = false;
	if (sqrlength(ScreenMousePosition - scrPosition) <= r * r && GetWindow(window_id)->active)
	{
		cs *= 1.1f;
		if (JustReleasedbutton[GLFW_MOUSE_BUTTON_1])
			*param = !*param;
		hover = true;
	}
	float TextScale = textScale;
	glm::vec2 textOffset = glm::vec2(30.0f , -20.0f  * TextScale);




	if (*param || (buttons[GLFW_MOUSE_BUTTON_1] && hover))
	{
		UI_DrawCube(scrPosition, { cs , cs }, 0.0f, colorON, false, 0, Z_Index, Additive);
		UI_DrawText(text, scrPosition + textOffset, TextScale, colorON, Z_Index, Additive);
	}
	else
	{
		UI_DrawCube(scrPosition, { cs , cs }, 0.0f, ColorOFF, false, 0, Z_Index, Additive);
		UI_DrawText(text, scrPosition + textOffset, TextScale , ColorOFF, Z_Index, Additive);
	}

	glm::vec2 Testsize = getTextSize(text, TextScale);
	Testsize.x += textOffset.x + scale;
	Testsize.y = textOffset.y + scale*1.2f > Testsize.y? textOffset.y + scale * 1.2f : Testsize.y;
	return Testsize * 2.0f;


}
//UI returns size of object
glm::vec2 UI_buttonOnlyON(bool* param, const char* text, glm::vec2 scrPosition, float scale, float textScale, glm::vec4 colorON, glm::vec4 ColorOFF, int Z_Index, bool Additive)
{


	scrPosition.x += scale;

	float r = scale * 1.25;
	bool hover = false;
	if (sqrlength(ScreenMousePosition - scrPosition) <= r * r && GetWindow(window_id)->active)
	{
		scale *= 1.1f;
		if (JustReleasedbutton[GLFW_MOUSE_BUTTON_1])
			*param = true;
		hover = true;
	}
	float TextScale = textScale;
	glm::vec2 textOffset = glm::vec2(30.0f, -20.0f * TextScale);




	if (*param || (buttons[GLFW_MOUSE_BUTTON_1] && hover))
	{
		UI_DrawCube(scrPosition, { scale , scale }, 0.0f, colorON, false, 0, Z_Index, Additive);
		UI_DrawText(text, scrPosition + textOffset, TextScale, colorON, Z_Index, Additive);
	}
	else
	{
		UI_DrawCube(scrPosition, { scale , scale }, 0.0f, ColorOFF, false, 0, Z_Index, Additive);
		UI_DrawText(text, scrPosition + textOffset, TextScale, ColorOFF, Z_Index, Additive);
	}


	glm::vec2 Testsize = getTextSize(text, TextScale);
	Testsize.x += textOffset.x + scale;
	Testsize.y = textOffset.y + scale > Testsize.y ? textOffset.y + scale : Testsize.y;
	return Testsize * 2.0f;

}
//UI returns size of object
glm::vec2 UI_button(bool* param, const char* text, glm::vec2 scrPosition, glm::vec2 scale, float textScale, glm::vec4 TextColorAdder, glm::vec4 ColorON, glm::vec4 ColorOFF, int Z_Index, bool Additive)
{
	glm::vec2 mp = ScreenMousePosition;

	glm::vec2 pos = scrPosition;

	bool hover = false;
	if ((mp.x - pos.x) > 0 && (mp.x - pos.x) < scale.x &&
		mp.y > pos.y - scale.y*0.5f && mp.y < pos.y + scale.y * 0.5f)
	{
		if(GetWindow(window_id)->active){
			if (JustReleasedbutton[GLFW_MOUSE_BUTTON_1])
				*param = true;
			else
				*param = false;
		hover = true;
		}
	}
	pos = scrPosition + glm::vec2(scale.x * 0.5f, 0.0f);

	
	glm::vec2 Testsize = getTextSize(text, textScale);
	

	glm::vec4 backColor;
	glm::vec4 TextColor;


	if (*param || (buttons[GLFW_MOUSE_BUTTON_1] && hover))
	{
		backColor = ColorON;
		TextColor = ColorON + TextColorAdder;
	}
	else if (hover)
	{
		backColor = (ColorOFF + ColorON) * 0.5f;
		TextColor = (ColorOFF + TextColorAdder + ColorON + TextColorAdder) * 0.5f;
	}
	else
	{
		backColor = ColorOFF;
		TextColor = ColorOFF + TextColorAdder;
	}

	UI_DrawSmoothQuad(pos, scale * 0.5f, 0.0f, backColor, Z_Index);
	UI_DrawText(text, pos + glm::vec2(-Testsize.x * 0.5f, -Testsize.y * 0.5f), textScale, TextColor, Z_Index, Additive);

	return scale;

}

//UI returns size of object
glm::vec2  UI_Slider(float* param, const char* text, glm::vec2 scrPosition, float min, float max, glm::vec2 scale, float TextScale, glm::vec4 Lcolor, glm::vec4 Bcolor, int Z_Index, bool Additive)
{

	//scrPosition.y += scale.y;
	UI_DrawLine(scrPosition, glm::vec2(scrPosition.x + scale.x, scrPosition.y),  scale.y, Lcolor, false, NULL, Z_Index);


	float range = max - min;



	float stage = (*param - min) / range;

	if (stage < 0.0f)stage = 0.0f;
	if (stage > 1.0f)stage = 1.0f;
	glm::vec2 bpos = { stage * scale.x + scrPosition.x,scrPosition.y };
	float bsize = 1.0f;
	glm::vec2 mp = LastJustPressedLMBScrMousePos;


	if (
		(mp.x - scrPosition.x) > 0 && (mp.x - scrPosition.x) < scale.x &&
		mp.y > scrPosition.y - scale.y && mp.y < scrPosition.y + scale.y
		)
	{
		bsize *= 1.1f;
		if (buttons[GLFW_MOUSE_BUTTON_1] && GetWindow(window_id)->active)
		{
			float w = ((ScreenMousePosition.x - scrPosition.x) / scale.x);
			*param = LinearInterpolation(min, max, w);


			if (*param < min)*param = min;
			if (*param > max)*param = max;

		}
	}

	stage = (*param - min) / range;

	if (stage < 0.0f)stage = 0.0f;
	if (stage > 1.0f)stage = 1.0f;

	bpos = { stage * scale.x + scrPosition.x,scrPosition.y };

	UI_DrawCircle(bpos, 2.0 * scale.y * bsize, Bcolor, false, NULL, Z_Index);

	glm::vec2 Testsize = getTextSize(text, TextScale);

	glm::vec2 textOffset = glm::vec2(scale.x + Testsize.y, -scale.y * 0.5f);

	
	UI_DrawText(text, scrPosition - glm::vec2(-textOffset.x, Testsize.y * 0.5f), TextScale,Bcolor, Z_Index);


	Testsize.x += textOffset.x ;
	Testsize.y = textOffset.y + scale.y > Testsize.y ? textOffset.y + scale.y : Testsize.y;
	return Testsize * 2.0f;


}
//UI returns size of object
glm::vec2  UI_SliderInt(int* param, const char* text, glm::vec2 scrPosition, int min,int max, glm::vec2 scale, float TextScale, glm::vec4 Lcolor, glm::vec4 Bcolor, int Z_Index, bool Additive)
{


	UI_DrawLine(scrPosition, glm::vec2(scrPosition.x + scale.x, scrPosition.y),  scale.y, Lcolor, false, NULL, Z_Index);

	float range = max - min;



	float stage = (*param - min) / range;

	if (stage < 0.0f)stage = 0.0f;
	if (stage > 1.0f)stage = 1.0f;
	glm::vec2 bpos = { stage * scale.x + scrPosition.x,scrPosition.y };
	float bsize = 1.0f;
	glm::vec2 mp = LastJustPressedLMBScrMousePos;


	if (
		(mp.x - scrPosition.x) > 0 && (mp.x - scrPosition.x) < scale.x &&
		mp.y > scrPosition.y - scale.y && mp.y < scrPosition.y + scale.y
		)
	{
		bsize *= 1.1f;
		if (buttons[GLFW_MOUSE_BUTTON_1] && GetWindow(window_id)->active)
		{
			float w = ((ScreenMousePosition.x - scrPosition.x) / scale.x);
			*param = round(LinearInterpolation(min, max, w));


			if (*param < min)*param = min;
			if (*param > max)*param = max;

		}
	}

	stage = (*param - min) / range;

	if (stage < 0.0f)stage = 0.0f;
	if (stage > 1.0f)stage = 1.0f;

	bpos = { stage * scale.x + scrPosition.x,scrPosition.y };

	UI_DrawCircle(bpos, 2.0 * scale.y * bsize, Bcolor, false, NULL, Z_Index);

	glm::vec2 Testsize = getTextSize(text, TextScale);

	glm::vec2 textOffset = glm::vec2(scale.x + Testsize.y , -scale.y * 0.5f);
	UI_DrawText(text, scrPosition + textOffset, TextScale, Bcolor, Z_Index);


	Testsize.x += textOffset.x;
	Testsize.y = textOffset.y + scale.y > Testsize.y ? textOffset.y + scale.y : Testsize.y;
	return Testsize * 2.0f;

}

//UI returns size of object
glm::vec2  UI_Drag(float* param, const char* text, glm::vec2 scrPosition, float speed, glm::vec2 scale, float TextScale, glm::vec4 Backcolor, glm::vec4 Textcolor, int Z_Index, bool Additive)
{


	UI_DrawLine(scrPosition, glm::vec2(scrPosition.x + scale.x, scrPosition.y), scale.y, Backcolor, false, NULL, Z_Index);


	if (keys[GLFW_KEY_LEFT_SHIFT])
		speed *= 10.0f;
	if (keys[GLFW_KEY_LEFT_ALT])
		speed *= 0.1f;

	glm::vec2 mp = LastJustPressedLMBScrMousePos;


	if (
		(mp.x - scrPosition.x) > 0 && (mp.x - scrPosition.x) < scale.x &&
		mp.y > scrPosition.y - scale.y && mp.y < scrPosition.y + scale.y
		)
	{
		if(GetWindow(window_id)->active){
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
		{
			Dragging = true;
			fDragBuff = *param;
		}
		if (Dragging)
			*param = fDragBuff + (ScreenMousePosition.x - LastJustPressedLMBScrMousePos.x) * speed;
		}
	}
	if (!buttons[GLFW_MOUSE_BUTTON_1])
	{
		Dragging = false;
		fDragBuff = -1;

	}


	std::string number = std::to_string(*param);
	number.pop_back();
	number.pop_back();
	number.pop_back();
	number.pop_back();

	UI_DrawText(number, scrPosition + glm::vec2(scale.x*0.5f - getTextSize(number, TextScale).x * 0.5f,-scale.y *0.5f) , TextScale, Textcolor,Z_Index);


	glm::vec2 Testsize = getTextSize(text, TextScale);

	glm::vec2 textOffset = glm::vec2(scale.x + Testsize.y , -scale.y * 0.5f);
	UI_DrawText(text, scrPosition + textOffset, TextScale, Textcolor, Z_Index);


	Testsize.x += textOffset.x;
	Testsize.y = textOffset.y + scale.y > Testsize.y ? textOffset.y + scale.y : Testsize.y;
	return Testsize * 2.0f;


}
//UI returns size of object
glm::vec2  UI_DragInt(int* param, const char* text, glm::vec2 scrPosition, float speed, glm::vec2 scale, float TextScale, glm::vec4 Backcolor, glm::vec4 Textcolor, int Z_Index, bool Additive)
{


	UI_DrawLine(scrPosition, glm::vec2(scrPosition.x + scale.x, scrPosition.y),  scale.y, Backcolor, false, NULL, Z_Index);



	if (keys[GLFW_KEY_LEFT_SHIFT])
		speed *= 10.0f;
	if (keys[GLFW_KEY_LEFT_ALT])
		speed *= 0.1f;

	glm::vec2 mp = LastJustPressedLMBScrMousePos;


	if (
		(mp.x - scrPosition.x) > 0 && (mp.x - scrPosition.x) < scale.x &&
		mp.y > scrPosition.y - scale.y && mp.y < scrPosition.y + scale.y
		)
	{
		if(GetWindow(window_id)->active){
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
		{
			Dragging = true;
			iDragBuff = *param;
		}
		if (Dragging)
			*param = iDragBuff + (ScreenMousePosition.x - LastJustPressedLMBScrMousePos.x) * speed;
		}
	}
	if (!buttons[GLFW_MOUSE_BUTTON_1])
	{
		Dragging = false;
		iDragBuff = -1;

	}


	std::string number = std::to_string(*param);

	UI_DrawText(number, scrPosition + glm::vec2(scale.x * 0.5f - getTextSize(number, TextScale).x * 0.5f, -scale.y * 0.5f), TextScale,  Textcolor, Z_Index);


	glm::vec2 Testsize = getTextSize(text, TextScale);

	glm::vec2 textOffset = glm::vec2(scale.x + Testsize.y , -scale.y * 0.5f);
	UI_DrawText(text, scrPosition + textOffset, TextScale, Textcolor, Z_Index);


	Testsize.x += textOffset.x;
	Testsize.y = textOffset.y + scale.y > Testsize.y ? textOffset.y + scale.y : Testsize.y;
	return Testsize * 2.0f;

}


int TextBoxcursorPosition = 0;
//UI returns size of object
glm::vec2  UI_TextBox(std::string* text, glm::vec2 scrPosition, int maxTextSize, glm::vec2 scale, float TextScale, glm::vec4 Backcolor, glm::vec4 Textcolor, int Z_Index , bool Additive)
{

	glm::vec2 Testsize = getTextSize(*text, TextScale);


	bool edit = false;


	glm::vec2 mp = ScreenMousePosition;


	if (
		(mp.x - scrPosition.x) > 0 && (mp.x - scrPosition.x) < scale.x &&
		mp.y > scrPosition.y - scale.y && mp.y < scrPosition.y + scale.y
		)
		if (bJustReleasedbutton[GLFW_MOUSE_BUTTON_1])
		{
			edit = false;
			LastJustPressedLMBScrMousePos = { 0.0f,0.0f };
		}
	if (bJustPressedkey[GLFW_KEY_ENTER] || bJustPressedkey[GLFW_KEY_ESCAPE])
	{
		edit = false;
		LastJustPressedLMBScrMousePos = { 0.0f,0.0f };
	}
	mp = LastJustPressedLMBScrMousePos;


	if (
		(mp.x - scrPosition.x) > 0 && (mp.x - scrPosition.x) < scale.x &&
		mp.y > scrPosition.y - scale.y && mp.y < scrPosition.y + scale.y
		)
	{

		edit = true;
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
			TextBoxcursorPosition = (*text).size();
	}
	else
		edit = false;
	if (edit && GetWindow(window_id)->active)
	{

		std::string tmptext = "";
		
		std::cout << TextFromKeyboard;;



		if (bJustPressedkey[GLFW_KEY_LEFT])
			TextBoxcursorPosition -= 1;
		if (bJustPressedkey[GLFW_KEY_RIGHT])
			TextBoxcursorPosition += 1;


		if (TextBoxcursorPosition <= -1)
			TextBoxcursorPosition = -1;
		else if (TextBoxcursorPosition > text->size() - 1)
			TextBoxcursorPosition = text->size() - 1;

		bool CursorProcessed = false;
		if (TextBoxcursorPosition == -1 && !CursorProcessed)
		{
			if (TextFromKeyboard.size() > 0 && (TextFromKeyboard.size() + tmptext.size() <= maxTextSize || maxTextSize < 0))
			{
				tmptext += TextFromKeyboard;
				TextBoxcursorPosition += TextFromKeyboard.size();
			}
			CursorProcessed = true;
			UI_DrawCube(scrPosition, glm::vec2(2.0f, scale.y * 0.9f), 0.0f, Textcolor + glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), false, NULL, Z_Index + 10);
		}
		glm::vec2 size = { 0.0f,0.0f };
		if (text->size() == 0 && !CursorProcessed)
		{

			if (bJustPressedkey[GLFW_KEY_BACKSPACE] && tmptext.size() > 0)
			{
				tmptext.pop_back();
				TextBoxcursorPosition -= 1;
			}
			else if (TextFromKeyboard.size() > 0 && (TextFromKeyboard.size() + tmptext.size() <= maxTextSize || maxTextSize < 0))
			{
				tmptext += TextFromKeyboard;
				TextBoxcursorPosition += TextFromKeyboard.size();
			}
			UI_DrawCube(scrPosition + glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, scale.y * 0.9f), 0.0f, Textcolor + glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), false, NULL, Z_Index + 10);
			CursorProcessed = true;
		}
		else
			for (int i = 0; i < text->size(); i++)
			{
				if (i == TextBoxcursorPosition && !CursorProcessed)
				{


					if (bJustPressedkey[GLFW_KEY_BACKSPACE] && text->size() > 0 && TextBoxcursorPosition >= 0)
					{
						TextBoxcursorPosition -= 1;
					}
					else if (TextFromKeyboard.size() > 0 && (TextFromKeyboard.size() + tmptext.size() <= maxTextSize || maxTextSize < 0))
					{
						tmptext += (*text)[i];
						tmptext += TextFromKeyboard;
						TextBoxcursorPosition += TextFromKeyboard.size();
					}
					else
						tmptext += (*text)[i];

					size.x += (Characters[(*text)[i]].Advance >> 6) * TextScale;
					if (size.y < Characters[(*text)[i]].Size.y * TextScale)
						size.y = Characters[(*text)[i]].Size.y * TextScale;
					UI_DrawCube(scrPosition + glm::vec2(size.x, 0.0f), glm::vec2(2.0f, scale.y * 0.9f), 0.0f, Textcolor + glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), false, NULL, Z_Index + 10);

					CursorProcessed = true;

				}
				else
				{
					tmptext += (*text)[i];
					size.x += (Characters[tmptext[i]].Advance >> 6) * TextScale;
					if (size.y < Characters[tmptext[i]].Size.y * TextScale)
						size.y = Characters[tmptext[i]].Size.y * TextScale;
				}
			
			}

		

		*text = tmptext;

		UI_DrawText(*text, scrPosition - glm::vec2(0.0f,Testsize.y*0.5f), TextScale, Textcolor + glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), Z_Index, Additive);
		UI_DrawLine(scrPosition, glm::vec2(scrPosition.x + scale.x, scrPosition.y), scale.y, Backcolor + glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), false, NULL, Z_Index);
	}
	else
	{
		UI_DrawText(*text, scrPosition - glm::vec2(0.0f, Testsize.y * 0.5f), TextScale, Textcolor, Z_Index, Additive);
		UI_DrawLine(scrPosition, glm::vec2(scrPosition.x + scale.x, scrPosition.y), scale.y, Backcolor, false, NULL, Z_Index);
	}
	Testsize.x += Testsize.x >scale.x? Testsize.x: scale.x;
	Testsize.y = scale.y > Testsize.y ? scale.y : Testsize.y;
	return Testsize * 2.0f;
}



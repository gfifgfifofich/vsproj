
#include "../Include/Helper.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <map>


#define DRAWINGIMPLEMENTATION
#include "../Include/Drawing.h"

#include "../Include/Text.h"
#include "../Include/UI.h"

//One time use. May become invalid after adding or deleating window, safe if otherwise.
Window* GetWindow(int id)
{
	return &Windows[id];
}

glm::vec2 Window::GetSize() 
{
	return (ViewportSize*0.5f) * Scale;
}

void Window::RecalculateSize()
{

	inited = false;
	Destroy();
	inited = true;
	w_ScreenAspectRatio = ViewportSize.x / ViewportSize.y;


	w_ScreenDivisorX = (ViewportSize.x / (w_ScreenAspectRatio * 2));
	w_ScreenDivisorY = (ViewportSize.y / 2.0f);

	w_ScreenDivisorX = 1.0f / w_ScreenDivisorX;
	w_ScreenDivisorY = 1.0f / w_ScreenDivisorY;
	w_ScaleMultiplyer = 1.0f / ViewportSize.y * 2.0f;
	
	if(framebuffer !=NULL)
		glDeleteFramebuffers(1,&framebuffer);
	glGenFramebuffers(1, &framebuffer);
	if(Texture !=NULL)
		glDeleteTextures(1,&Texture);
	glGenTextures(1, &Texture);

	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glBindTexture(GL_TEXTURE_2D, Texture);
	if (hdr)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ViewportSize.x, ViewportSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ViewportSize.x, ViewportSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);

	if(NormalMapFBO !=NULL)
		glDeleteFramebuffers(1,&NormalMapFBO);
	glGenFramebuffers(1, &NormalMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, NormalMapFBO);
	if(NormalMapColorBuffer !=NULL)
		glDeleteTextures(1,&NormalMapColorBuffer);
	glGenTextures(1, &NormalMapColorBuffer);
	glBindTexture(GL_TEXTURE_2D, NormalMapColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, ViewportSize.x, ViewportSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, NormalMapColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(LightColorFBO !=NULL)
		glDeleteFramebuffers(1,&LightColorFBO);
	if(LightColorBuffer !=NULL)
		glDeleteTextures(1,&LightColorBuffer);
	glGenFramebuffers(1, &LightColorFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, LightColorFBO);
	glGenTextures(1, &LightColorBuffer);
	glBindTexture(GL_TEXTURE_2D, LightColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ViewportSize.x, ViewportSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, LightColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void Window::Init(glm::vec2 ViewportSize, bool linearFilter, bool hdr)
{
	this->ViewportSize = ViewportSize;

	this->linearFilter = linearFilter;
	this->hdr = hdr;
	RecalculateSize();


	GetWindow(window_id)->Use();
	inited = true;
}

void Window::Use(bool ProcessControls)
{



	Window* prevWindow = GetWindow(window_id);
	if (prevWindow != nullptr)
	{
		prevWindow->w_LightSources = LightSources;
		prevWindow->w_SceneLayers =  SceneLayers;

		prevWindow->w_CameraPosition = CameraPosition;
		prevWindow->w_CameraScale = CameraScale;

		prevWindow->w_AmbientLight = AmbientLight;
		prevWindow->w_DirectionalLight = DirectionalLight;
	}

	ScreenAspectRatio = w_ScreenAspectRatio;

	LightSources = this->w_LightSources;
	SceneLayers =  this->w_SceneLayers;

	ScreenDivisorX = w_ScreenDivisorX;
	ScreenDivisorY = w_ScreenDivisorY;
	ScaleMultiplyer = w_ScaleMultiplyer;

	CameraPosition = w_CameraPosition;
	CameraScale = w_CameraScale;

	AmbientLight = w_AmbientLight;
	DirectionalLight = w_DirectionalLight;

	window_id = id;
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glViewport(0, 0, ViewportSize.x, ViewportSize.y);
	WIDTH = ViewportSize.x;
	HEIGHT = ViewportSize.y;
	if (ProcessControls) {
		if (id == 0)
		{
			ScreenMousePosition = { (lastX - WIDTH * 0.5f) ,(-lastY + HEIGHT * 0.5f) };
			//ScreenMousePosition -= Position;

			MousePosition.x = ScreenMousePosition.x / CameraScale.x + CameraPosition.x;
			MousePosition.y = ScreenMousePosition.y / CameraScale.y + CameraPosition.y;

			WindowMousePosition = ScreenMousePosition;

		}
		else
		{
			if(prevWindow !=nullptr)
				WindowMousePosition = (prevWindow->WindowMousePosition - Position);
			WindowMousePosition /= Scale;
			MousePosition.x = WindowMousePosition.x / CameraScale.x + CameraPosition.x;
			MousePosition.y = WindowMousePosition.y / CameraScale.y + CameraPosition.y;
			ScreenMousePosition = WindowMousePosition;
		}
		glm::vec2 ScrMP = GetWindow(0)->WindowMousePosition;
		if (AutoActive) {
			if (ScrMP.x < Position.x + ViewportSize.x * 0.5f * Scale.x && ScrMP.x >  Position.x - ViewportSize.x * 0.5f * Scale.x &&
				ScrMP.y < Position.y + ViewportSize.y * 0.5f * Scale.y && ScrMP.y >  Position.y - ViewportSize.y * 0.5f * Scale.y)

				//backgroundColor = { 0.0f,0.25f,0.0f,1.0f };
				active = true;
			else
				//backgroundColor = { 0.25f,0.0f,0.0f,1.0f };
				active = false;
		}
		if (active)
		{
			for (int i = 0; i < 1024; i++)
			{
				JustPressedkey[i] = bJustPressedkey[i];
				Holdingkey[i] = bHoldingkey[i];
			}
			for (int i = 0; i < 64; i++)
			{
				JustPressedbutton[i] = bJustPressedbutton[i];
				Holdingbutton[i] = bHoldingbutton[i];
			}

			JustPressedLMB = bJustPressedLMB;
			HoldingLMB = bHoldingLMB;
			scrollmovement = bscrollmovement;
		}
		else
		{
			for (int i = 0; i < 1024; i++)
			{
				JustPressedkey[i] = 0;
				Holdingkey[i] = 0;
			}
			for (int i = 0; i < 64; i++)
			{
				JustPressedbutton[i] = 0;
				Holdingbutton[i] = 0;
			}

			JustPressedLMB = 0;
			HoldingLMB = 0;
			scrollmovement = 0;

		}
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
			w_LastJustPressedLMBScrMousePos = WindowMousePosition;
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_3])
			w_LastJustPressedRMBScrMousePos = WindowMousePosition;
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_2])
			w_LastJustPressedMMBScrMousePos = WindowMousePosition;

		if (JustPressedbutton[GLFW_MOUSE_BUTTON_1])
			w_LastJustPressedLMBMousePos = MousePosition;
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_3])
			w_LastJustPressedRMBMousePos = MousePosition;
		if (JustPressedbutton[GLFW_MOUSE_BUTTON_2])
			w_LastJustPressedMMBMousePos = MousePosition;
		LastJustPressedLMBScrMousePos = w_LastJustPressedLMBScrMousePos;
		LastJustPressedMMBScrMousePos = w_LastJustPressedMMBScrMousePos;
		LastJustPressedRMBScrMousePos = w_LastJustPressedRMBScrMousePos;

		LastJustPressedLMBMousePos = w_LastJustPressedLMBMousePos;
		LastJustPressedMMBMousePos = w_LastJustPressedMMBMousePos;
		LastJustPressedRMBMousePos = w_LastJustPressedRMBMousePos;
	}

}

void  Window::Clear(glm::vec4 Color)
{
	glViewport(0, 0, ViewportSize.x, ViewportSize.y);

	glBindFramebuffer(GL_FRAMEBUFFER, NormalMapFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, LightColorFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(Color.r, Color.g, Color.b, Color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	

	GetWindow(window_id)->Use();
}
void Window::End()
{
	GetWindow(0)->Use();
}

void Window::Destroy()
{
	if (inited)
		Destroyed = true;
	if (Texture != NULL)
		glDeleteTextures(1, &Texture);
	if (this->framebuffer != NULL)
		glDeleteFramebuffers(1, &this->framebuffer);


	if (LightColorBuffer != NULL)
		glDeleteTextures(1, &LightColorBuffer);
	if (this->LightColorFBO != NULL)
		glDeleteFramebuffers(1, &this->LightColorFBO);


	if (NormalMapColorBuffer != NULL)
		glDeleteTextures(1, &NormalMapColorBuffer);
	if (this->NormalMapFBO != NULL)
		glDeleteFramebuffers(1, &this->NormalMapFBO);

	if (window_id == id)
		window_id = 0;
}
void Window::_Draw()
{
	int prev = window_id;

	if (Autoclear)
		Clear(backgroundColor);
	Use(false);




	unsigned int instanceCircleVBO[3];//stock circles
	unsigned int instanceVBO[3];//stock cubes
	unsigned int instanceTexturedQuadVBO[4]; 
	unsigned int instanceNormalMapCircleVBO[3];
	unsigned int instanceNormalMapCubeVBO[3];
	unsigned int instanceNormalMapTextureVBO[3];

	float aspect = (float)HEIGHT / (float)WIDTH;

	for (int i = 0; i < SceneLayers.size(); i++)
	{
		if (SceneLayers[i].Additive)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		else
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// instancing
		{
			glGenBuffers(3, &instanceCircleVBO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, instanceCircleVBO[0]);
			glBindVertexArray(CircleVAO);


			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].Circlecolors.size(), &SceneLayers[i].Circlecolors[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glVertexAttribDivisor(1, 1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceCircleVBO[1]);
			glBindVertexArray(CircleVAO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].CirclePosScale.size(), &SceneLayers[i].CirclePosScale[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

			glVertexAttribDivisor(2, 1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceCircleVBO[2]);
			glBindVertexArray(CircleVAO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].CircleRotations.size(), &SceneLayers[i].CircleRotations[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//quads
		{
			glGenBuffers(3, instanceVBO);
			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[0]);

			glBindVertexArray(quadVAO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].Quadcolors.size(), &SceneLayers[i].Quadcolors[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

			glVertexAttribDivisor(2, 1);



			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[1]);

			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].QuadPosScale.size(), &SceneLayers[i].QuadPosScale[0], GL_DYNAMIC_DRAW);
			glBindVertexArray(quadVAO);


			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);


			glVertexAttribDivisor(3, 1);


			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[2]);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].QuadRotations.size(), &SceneLayers[i].QuadRotations[0], GL_DYNAMIC_DRAW);
			glBindVertexArray(quadVAO);


			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);


			glVertexAttribDivisor(4, 1);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		}

		UseShader(InctanceQuadShader);
		glUniform1f(glGetUniformLocation(InctanceQuadShader, "aspect"), aspect);
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, SceneLayers[i].Quadcolors.size());
		glBindVertexArray(0);


		DetachShader();
		UseShader(CircleShader);
		glUniform1f(glGetUniformLocation(CircleShader, "aspect"), aspect);
		glBindVertexArray(CircleVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, SceneLayers[i].Circlecolors.size());
		glBindVertexArray(0);

		glDeleteBuffers(3, instanceCircleVBO);
		glDeleteBuffers(3, instanceVBO);





		glGenBuffers(3, instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[0]);

		glBindVertexArray(quadVAO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].SmoothQuadcolors.size(), &SceneLayers[i].SmoothQuadcolors[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

		glVertexAttribDivisor(3, 1);



		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[1]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].SmoothQuadPosScale.size(), &SceneLayers[i].SmoothQuadPosScale[0], GL_DYNAMIC_DRAW);
		glBindVertexArray(quadVAO);


		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);


		glVertexAttribDivisor(2, 1);


		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[2]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].SmoothQuadRotations.size(), &SceneLayers[i].SmoothQuadRotations[0], GL_DYNAMIC_DRAW);
		glBindVertexArray(quadVAO);


		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);


		glVertexAttribDivisor(1, 1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		UseShader(InctanceSmoothQuadShader);
		glUniform1f(glGetUniformLocation(InctanceSmoothQuadShader, "aspect"), aspect);
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, SceneLayers[i].SmoothQuadcolors.size());
		glBindVertexArray(0);


		glDeleteBuffers(3, instanceVBO);

		SceneLayers[i].SmoothQuadRotations.clear();
		SceneLayers[i].SmoothQuadcolors.clear();
		SceneLayers[i].SmoothQuadPosScale.clear();


		DetachShader();
		SceneLayers[i].Quadcolors.clear();
		SceneLayers[i].QuadPosScale.clear();
		SceneLayers[i].QuadRotations.clear();

		SceneLayers[i].Circlecolors.clear();
		SceneLayers[i].CirclePosScale.clear();
		SceneLayers[i].CircleRotations.clear();


		for (int TQA = 0; TQA < SceneLayers[i].TexturedQuads.size(); TQA++)
		{
			UseShader(InstanceTexturedQuadShader);

			glUniform1f(glGetUniformLocation(InstanceTexturedQuadShader, "aspect"), aspect);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, SceneLayers[i].TexturedQuads[TQA].material.Texture);
			glUniform1i(glGetUniformLocation(InstanceTexturedQuadShader, "Texture"), 0);

			glGenBuffers(4, instanceTexturedQuadVBO);
			glBindVertexArray(quadVAO);



			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].TexturedQuads[TQA].QuadRotations.size(), &SceneLayers[i].TexturedQuads[TQA].QuadRotations[0], GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glVertexAttribDivisor(1, 1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].TexturedQuads[TQA].QuadPosScale.size(), &SceneLayers[i].TexturedQuads[TQA].QuadPosScale[0], GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glVertexAttribDivisor(2, 1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].TexturedQuads[TQA].Quadcolors.size(), &SceneLayers[i].TexturedQuads[TQA].Quadcolors[0], GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
			glVertexAttribDivisor(3, 1);


			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].TexturedQuads[TQA].QuadDepth.size(), &SceneLayers[i].TexturedQuads[TQA].QuadDepth[0], GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glVertexAttribDivisor(4, 1);

			glUniform1i(glGetUniformLocation(InstanceTexturedQuadShader, "flipY"), SceneLayers[i].TexturedQuads[TQA].material.flipY);
			glUniform1i(glGetUniformLocation(InstanceTexturedQuadShader, "flipX"), SceneLayers[i].TexturedQuads[TQA].material.flipX);

			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, SceneLayers[i].TexturedQuads[TQA].QuadPosScale.size());

			if (Lighting && SceneLayers[i].TexturedQuads[TQA].material.NormalMap > 0 )
			{
				glBlendFunc(GL_ONE, GL_ZERO);
				UseShader(InstancedNormalMapShader);


				glUniform1f(glGetUniformLocation(InstancedNormalMapShader, "aspect"), aspect);


				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].TexturedQuads[TQA].material.NormalMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture"), 0);


				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].TexturedQuads[TQA].material.Texture);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture2"), 1);

				if (SceneLayers[i].TexturedQuads[TQA].material.Texture != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), false);
				
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].TexturedQuads[TQA].material.HeightMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMap"), 2);
				
				if (SceneLayers[i].TexturedQuads[TQA].material.HeightMap != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), false);



				bool gen = false;
				if (SceneLayers[i].TexturedQuads[TQA].material.NormalMap == BallNormalMapTexture || SceneLayers[i].TexturedQuads[TQA].material.NormalMap == CubeNormalMapTexture)
					gen = true;
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "generated"), gen);


				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "flipY"), SceneLayers[i].TexturedQuads[TQA].material.flipY + !gen);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "flipX"), SceneLayers[i].TexturedQuads[TQA].material.flipX);

				glBindFramebuffer(GL_FRAMEBUFFER, NormalMapFBO);

				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, SceneLayers[i].TexturedQuads[TQA].QuadPosScale.size());

				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

				if (SceneLayers[i].Additive)
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				else
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			

			glDeleteBuffers(4, instanceTexturedQuadVBO);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			SceneLayers[i].TexturedQuads[TQA].clear();
			DetachShader();
		}
		
		for (int PA = 0; PA < SceneLayers[i].Polygons.size(); PA++)
		{

			UseShader(InstanceTexturedQuadShader);



			glBindVertexArray(SceneLayers[i].Polygons[PA].VAO);
			glGenBuffers(4, instanceTexturedQuadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].Polygons[PA].colors.size(), &SceneLayers[i].Polygons[PA].colors[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

			glVertexAttribDivisor(3, 1);


			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[1]);


			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].Polygons[PA].PosScale.size(), &SceneLayers[i].Polygons[PA].PosScale[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

			glVertexAttribDivisor(2, 1);


			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[2]);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].Polygons[PA].Rotations.size(), &SceneLayers[i].Polygons[PA].Rotations[0], GL_DYNAMIC_DRAW);


			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);


			glVertexAttribDivisor(1, 1);

			glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].Polygons[PA].Depths.size(), &SceneLayers[i].Polygons[PA].Depths[0], GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glVertexAttribDivisor(4, 1);


			glBindVertexArray(SceneLayers[i].Polygons[PA].VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, SceneLayers[i].Polygons[PA].material.Texture);
			glUniform1i(glGetUniformLocation(InstanceTexturedQuadShader, "Texture"), 0);

			glUniform1f(glGetUniformLocation(InstanceTexturedQuadShader, "aspect"), aspect);

			glUniform1i(glGetUniformLocation(InstanceTexturedQuadShader, "flipY"), SceneLayers[i].Polygons[PA].material.flipY);
			glUniform1i(glGetUniformLocation(InstanceTexturedQuadShader, "flipX"), SceneLayers[i].Polygons[PA].material.flipX);

			glDrawArraysInstanced(GL_TRIANGLES, 0, SceneLayers[i].Polygons[PA].Size, SceneLayers[i].Polygons[PA].Rotations.size());



			if (Lighting && SceneLayers[i].Polygons[PA].material.NormalMap>0)
			{
				glBlendFunc(GL_ONE, GL_ZERO);
				UseShader(InstancedNormalMapShader);


				glUniform1f(glGetUniformLocation(InstancedNormalMapShader, "aspect"), aspect);


				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].Polygons[PA].material.NormalMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture"), 0);


				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].Polygons[PA].material.Texture);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture2"), 1);

				if (SceneLayers[i].Polygons[PA].material.Texture != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), false);
				
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].Polygons[PA].material.HeightMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMap"), 2);
				
				if (SceneLayers[i].Polygons[PA].material.HeightMap != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), false);


				bool gen = false;
				if (SceneLayers[i].Polygons[PA].material.NormalMap == BallNormalMapTexture || SceneLayers[i].Polygons[PA].material.NormalMap == CubeNormalMapTexture)
					gen = true;
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "generated"), false);


				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "flipY"), SceneLayers[i].Polygons[PA].material.flipY + !gen);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "flipX"), SceneLayers[i].Polygons[PA].material.flipX);

				glBindFramebuffer(GL_FRAMEBUFFER, NormalMapFBO);

				glDrawArraysInstanced(GL_TRIANGLES, 0, SceneLayers[i].Polygons[PA].Size, SceneLayers[i].Polygons[PA].Rotations.size());

				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

				if (SceneLayers[i].Additive)
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				else
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			}





			glDeleteBuffers(4, instanceTexturedQuadVBO);

			glBindVertexArray(0);
			DetachShader();

			SceneLayers[i].Polygons[PA].clear();
		}
		SceneLayers[i].Polygons.clear();


		for (int tt = 0; tt < SceneLayers[i].TextLines.size(); tt++)
			_DrawText(SceneLayers[i].TextLines[tt].text, SceneLayers[i].TextLines[tt].x, SceneLayers[i].TextLines[tt].y, SceneLayers[i].TextLines[tt].scale, SceneLayers[i].TextLines[tt].color);
		SceneLayers[i].TextLines.clear();

		for (int tt = 0; tt < SceneLayers[i].UI_TextLines.size(); tt++)
			_UI_DrawText(SceneLayers[i].UI_TextLines[tt].text, SceneLayers[i].UI_TextLines[tt].x, SceneLayers[i].UI_TextLines[tt].y, SceneLayers[i].UI_TextLines[tt].scale, SceneLayers[i].UI_TextLines[tt].color);
		SceneLayers[i].UI_TextLines.clear();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Rest of NormalMaps
		if (Lighting)
		{
			glBlendFunc(GL_ONE, GL_ZERO);
			glBindFramebuffer(GL_FRAMEBUFFER, NormalMapFBO);

			for (int NQA = 0; NQA < SceneLayers[i].NormalMaps.size(); NQA++)
			{
				UseShader(InstancedNormalMapShader);
				bool gen = false;
				if (SceneLayers[i].NormalMaps[NQA].material.NormalMap == BallNormalMapTexture || SceneLayers[i].NormalMaps[NQA].material.NormalMap == CubeNormalMapTexture)
					gen = true;
				glUniform1f(glGetUniformLocation(InstancedNormalMapShader, "aspect"), aspect);

				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "generated"), gen);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "flipY"), !gen);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].NormalMaps[NQA].material.NormalMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture"), 0);


				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].NormalMaps[NQA].material.Texture);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture2"), 1);

				if (SceneLayers[i].NormalMaps[NQA].material.Texture != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), false);
				

				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].NormalMaps[NQA].material.HeightMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMap"), 2);
				
				if (SceneLayers[i].NormalMaps[NQA].material.HeightMap != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), false);


				glGenBuffers(3, instanceNormalMapTextureVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, instanceNormalMapTextureVBO[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].NormalMaps[NQA].QuadRotations.size(), &SceneLayers[i].NormalMaps[NQA].QuadRotations[0], GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

				glVertexAttribDivisor(1, 1);

				glBindBuffer(GL_ARRAY_BUFFER, instanceNormalMapTextureVBO[2]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].NormalMaps[NQA].QuadDepth.size(), &SceneLayers[i].NormalMaps[NQA].QuadDepth[0], GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

				glVertexAttribDivisor(4, 1);


				glBindBuffer(GL_ARRAY_BUFFER, instanceNormalMapTextureVBO[1]);
				glBindVertexArray(quadVAO);

				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * SceneLayers[i].NormalMaps[NQA].QuadPosScale.size(), &SceneLayers[i].NormalMaps[NQA].QuadPosScale[0], GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

				glVertexAttribDivisor(2, 1);

				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, SceneLayers[i].NormalMaps[NQA].QuadPosScale.size());
				glDeleteBuffers(3, instanceNormalMapTextureVBO);

				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				SceneLayers[i].NormalMaps[NQA].clear();
				DetachShader();
			}
			SceneLayers[i].NormalMaps.clear();
			for (int PA = 0; PA < SceneLayers[i].PolygonNormalMaps.size(); PA++)
			{

				UseShader(InstancedNormalMapShader);

				glUniform1f(glGetUniformLocation(InstancedNormalMapShader, "aspect"), aspect);

				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "generated"), false);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "flipY"), true);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].PolygonNormalMaps[PA].material.Texture);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture"), 0);


				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].PolygonNormalMaps[PA].material.NormalMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "Texture2"), 1);

				if (SceneLayers[i].PolygonNormalMaps[PA].material.NormalMap != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "AlphaTexture"), false);

				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, SceneLayers[i].PolygonNormalMaps[PA].material.HeightMap);
				glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMap"), 2);
				
				if (SceneLayers[i].PolygonNormalMaps[PA].material.HeightMap != NULL)
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), true);
				else
					glUniform1i(glGetUniformLocation(InstancedNormalMapShader, "HeightMapped"), false);
	


				glBindVertexArray(SceneLayers[i].PolygonNormalMaps[PA].VAO);
				glGenBuffers(3, instanceTexturedQuadVBO);
			

				



				glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].PolygonNormalMaps[PA].Rotations.size(), &SceneLayers[i].PolygonNormalMaps[PA].Rotations[0], GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
				glVertexAttribDivisor(1, 1);


				glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[1]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)* SceneLayers[i].PolygonNormalMaps[PA].PosScale.size(), &SceneLayers[i].PolygonNormalMaps[PA].PosScale[0], GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
				glVertexAttribDivisor(2, 1);


				glBindBuffer(GL_ARRAY_BUFFER, instanceTexturedQuadVBO[3]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SceneLayers[i].PolygonNormalMaps[PA].Depths.size(), &SceneLayers[i].PolygonNormalMaps[PA].Depths[0], GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
				glVertexAttribDivisor(4, 1);

				glBindVertexArray(SceneLayers[i].PolygonNormalMaps[PA].VAO);

				glDrawArraysInstanced(GL_TRIANGLES, 0, SceneLayers[i].PolygonNormalMaps[PA].Size, SceneLayers[i].PolygonNormalMaps[PA].Rotations.size());
				glDeleteBuffers(3, instanceTexturedQuadVBO);

				glBindVertexArray(0);
				DetachShader();

				SceneLayers[i].PolygonNormalMaps[PA].clear();
			}
			SceneLayers[i].PolygonNormalMaps.clear();

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		}

	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (Lighting) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindFramebuffer(GL_FRAMEBUFFER, LightColorFBO);

		UseShader(LightShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, NormalMapColorBuffer);
		glUniform1i(glGetUniformLocation(LightShader, "NormalMap"), 0);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(glGetUniformLocation(LightShader, "BaseColor"), 2);

		//std::cout<<"\ni "<<id<<" ls = "<<LightSources.size();

		for (int i = 0; i < LightSources.size(); i++)
		{

			float aspx = ScreenDivisorX * CameraScale.x;
			float aspy = ScreenDivisorY * CameraScale.y;
			glm::vec2 Apos = glm::vec2(LightSources[i].position.x, LightSources[i].position.y);
			Apos -= CameraPosition;
			Apos.x *= aspx;
			Apos.y *= aspy;
			LightSources[i].scale.x *= aspx;
			LightSources[i].scale.y *= aspy;

			LightSources[i].position.x -= CameraPosition.x;
			LightSources[i].position.y -= CameraPosition.y;
			LightSources[i].position.x *= CameraScale.x / WIDTH;
			LightSources[i].position.y *= CameraScale.y / HEIGHT;

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, LightSources[i].texture);
			glUniform1i(glGetUniformLocation(LightShader, "Texture"), 1);

			glUniform3f(glGetUniformLocation(LightShader, "position"), LightSources[i].position.x, LightSources[i].position.y, LightSources[i].position.z);
			glUniform2f(glGetUniformLocation(LightShader, "Aposition"), Apos.x, Apos.y);
			glUniform2f(glGetUniformLocation(LightShader, "scale"), LightSources[i].scale.x, LightSources[i].scale.y);
			glUniform1f(glGetUniformLocation(LightShader, "sizeZ"), LightSources[i].scale.z);
			glUniform2f(glGetUniformLocation(LightShader, "CameraScale"), CameraScale.x, CameraScale.y);
			glUniform1f(glGetUniformLocation(LightShader, "angle"), LightSources[i].rotation);

			glUniform4f(glGetUniformLocation(LightShader, "color"), LightSources[i].color.r, LightSources[i].color.g, LightSources[i].color.b, LightSources[i].color.a);
			glUniform1f(glGetUniformLocation(LightShader, "volume"), LightSources[i].volume);
			glUniform1f(glGetUniformLocation(LightShader, "aspect"), aspect);
			glUniform2f(glGetUniformLocation(LightShader, "scr"), WIDTH, HEIGHT);



			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);


		}
		glBlendFunc(GL_ONE, GL_ZERO);
		DetachShader();


		LightSources.clear();

		Use(false);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		UseShader(AddTexturesShader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, LightColorBuffer);
		glUniform1i(glGetUniformLocation(AddTexturesShader, "Texture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(glGetUniformLocation(AddTexturesShader, "Texture2"), 1);
		glUniform2f(glGetUniformLocation(AddTexturesShader, "proportions"), DirectionalLight, AmbientLight);

		glBindVertexArray(ScreenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);

		DetachShader();
	}


	UseWindow(prev);
}



int CreateWindow()
{
	Window w;
	w.id = Windows.size();
	Windows.insert({ w.id,w });

	CurrentWindow = GetWindow(window_id);
	return w.id;
}


void UseWindow(int id)
{
	GetWindow(id)->Use();
}
void EndOfWindow()
{
	GetWindow(0)->Use();

}




void PreLoadShaders()
{


	LoadShader(&FillScreenShader, "./engine/Shaders/Default.vert", "./engine/Shaders/FillScreen.frag");

	LoadShader(&InctanceQuadShader, "./engine/Shaders/Quad/instance.vert", "./engine/Shaders/Quad/Quad.frag");
	LoadShader(&InstanceTexturedQuadShader, "./engine/Shaders/InstancedTexturedQuad/InstancedTexturedQuad.vert", "./engine/Shaders/InstancedTexturedQuad/InstancedTexturedQuad.frag");
	LoadShader(&InctanceSmoothQuadShader, "./engine/Shaders/InstancedTexturedQuad/InstancedTexturedQuad.vert", "./engine/Shaders/Quad/Bald.frag");
	
	
	
	LoadShader(&InstanceFlippedTexturedQuadShader, "./engine/Shaders/InstancedTexturedQuad/InstancedFlippedTexturedQuad.vert", "./engine/Shaders/InstancedTexturedQuad/InstancedFlippedTexturedQuad.frag");


	//Texture Generation
	LoadShader(&GradientGenShader, "./engine/Shaders/Default.vert", "./engine/Shaders/GradientGen/GradientGen.frag");
	LoadShader(&NoizeGenShader, "./engine/Shaders/NoizeGen/NoizeGen.vert" , "./engine/Shaders/NoizeGen/NoizeGen.frag");
	LoadShader(&RoundShader, "./engine/Shaders/Round/Round.vert" , "./engine/Shaders/Round/Round.frag");
	LoadShader(&AddTexturesShader, "./engine/Shaders/Default.vert", "./engine/Shaders/Textures/AddTextures.frag");
	LoadShader(&CopyTextureShader, "./engine/Shaders/Default.vert", "./engine/Shaders/Textures/Copy.frag");
	LoadShader(&GenNormalMapShader, "./engine/Shaders/Default.vert", "./engine/Shaders/NormalMap/GenNormalMap.frag");
	LoadShader(&GenLightSphereShader, "./engine/Shaders/Default.vert", "./engine/Shaders/Light/GenLightSphere.frag");

	//Post processing
	LoadShader(&BlurShader, "./engine/Shaders/blur/blur.vert" , "engine/Shaders/blur/blur.frag");
	LoadShader(&shaderBloom, "./engine/Shaders/Default.vert", "./engine/Shaders/Bloom/bloom.frag");
	LoadShader(&ScrShade, "./engine/Shaders/Default.vert", "./engine/Shaders/Screen.frag");
	LoadShader(&Chromatic, "./engine/Shaders/Default.vert", "./engine/Shaders/Chromatic/Chromatic.frag");
	LoadShader(&DownsampleBlur, "./engine/Shaders/Default.vert", "./engine/Shaders/blur/DownscaleBlur.frag");
	LoadShader(&UpsampleBlur, "./engine/Shaders/Default.vert", "./engine/Shaders/blur/UpsampleBlur.frag");

	//Drawing
	//Quad
	LoadShader(&FillShader, "./engine/Shaders/Quad/default.vert" , "./engine/Shaders/Quad/Quad.frag");
	LoadShader(&TexturedQuadShader, "./engine/Shaders/Quad/TexturedQuad.vert" , "./engine/Shaders/Quad/TexturedQuad.frag");

	//Circle	
	LoadShader(&CircleShader, "./engine/Shaders/Circle/Circle.vert", "./engine/Shaders/Circle/Circle.frag");

	//Lighting
	LoadShader(&InstancedNormalMapShader, "./engine/Shaders/InstancedNormalMap/InstancedNMDraw.vert" , "./engine/Shaders/InstancedNormalMap/NormalMapDraw.frag");
	LoadShader(&NormalMapDrawShader, "./engine/Shaders/NormalMap/NormalMapDraw.vert" , "./engine/Shaders/NormalMap/NormalMapDraw.frag");

	LoadShader(&LightShader, "./engine/Shaders/Light/Light.vert" , "./engine/Shaders/Light/LightProcess.frag");

	//Triangle	
	LoadShader(&FillTriangleShader, "./engine/Shaders/Triangle/Fill.vert" , "./engine/Shaders/Triangle/Fill.frag");
	LoadShader(&TexturedTriangleShader, "./engine/Shaders/Triangle/TexturedTriangle.vert" , "./engine/Shaders/Triangle/TexturedTriangle.frag");



	LoadShader(&GenPrimitiveTextureShader, "./engine/Shaders/Default.vert", "./engine/Shaders/GenPrimitiveTextureShader.frag");

	LoadShader(&TextShader, "./engine/Shaders/Quad/TexturedQuad.vert", "./engine/Shaders/Text.frag");

}
void SortSceneLayers()
{
	if (SceneLayers.size() > 2)
	{
		int i = 1;
		int a = 2;

		while (i <SceneLayers.size())
		{
			if ((SceneLayers[i - 1].Z_Index < SceneLayers[i].Z_Index) ||
				(SceneLayers[i - 1].Z_Index == SceneLayers[i].Z_Index) && !SceneLayers[i - 1].Additive && SceneLayers[i].Additive)
			{
				i = a;
				a++;
			}
			else if ((SceneLayers[i - 1].Z_Index > SceneLayers[i].Z_Index) ||
				((SceneLayers[i - 1].Z_Index == SceneLayers[i].Z_Index) && SceneLayers[i - 1].Additive && !SceneLayers[i].Additive))
			{
				SceneLayer tmp = SceneLayers[i - 1];
				SceneLayers[i - 1] = SceneLayers[i];
				SceneLayers[i] = tmp;
				i--;
				if (i == 0)
				{
					i = a;
					a++;
				}
			}
		}
	}
	else if (SceneLayers.size() == 2)
	{
		if ((SceneLayers[0].Z_Index >SceneLayers[1].Z_Index) ||
			((SceneLayers[0].Z_Index == SceneLayers[1].Z_Index) && SceneLayers[0].Additive && !SceneLayers[1].Additive))
		{
			SceneLayer tmp = SceneLayers[0];
			SceneLayers[0] = SceneLayers[1];
			SceneLayers[1] = tmp;
		}
	}
}
int FindSceneLayer(int Z_Index, bool Additive)
{
	int SLI = -1;

	for (int i = 0; i < SceneLayers.size(); i++)
		if (SceneLayers[i].Z_Index == Z_Index && SceneLayers[i].Additive == Additive)
			SLI = i;
	if (SLI == -1)
	{
		SceneLayer sl;
		sl.Z_Index = Z_Index;
		sl.Additive = Additive;
		SceneLayers.push_back(sl);
		SortSceneLayers();
		for (int i = 0; i < SceneLayers.size(); i++)
			if (SceneLayers[i].Z_Index == Z_Index)
				SLI = i;
	}
	return SLI;
}

void DrawLight(glm::vec2 position, glm::vec2 scale, glm::vec4 color, float volume, float rotation, unsigned int texture)
{
	LightSource ls;
	ls.volume = volume;
	ls.position = glm::vec3(position, 0.0f);
	ls.scale = glm::vec3(scale, 1.0f);
	ls.rotation = rotation;
	ls.color = color;
	ls.texture = texture;
	LightSources.push_back(ls);
}
void DrawLight(glm::vec3 position, glm::vec2 scale, glm::vec4 color, float volume, float rotation, unsigned int texture)
{
	LightSource ls;
	ls.volume = volume;
	ls.position =  glm::vec3(position.x,position.y, position.z);
	ls.scale = glm::vec3(scale, 1.0f);
	ls.rotation = rotation;
	ls.color = color;
	ls.texture = texture;
	LightSources.push_back(ls);
}
void DrawLight(glm::vec3 position, glm::vec3 scale, glm::vec4 color, float volume, float rotation, unsigned int texture)
{
	LightSource ls;
	ls.volume = volume;
	ls.position =  glm::vec3(position.x,position.y, position.z);
	ls.scale = scale;
	ls.rotation = rotation;
	ls.color = color;
	ls.texture = texture;
	LightSources.push_back(ls);
}
void NormalMapDraw(glm::vec2 position, glm::vec2 scale, unsigned int NormalMap, float rotation, int Z_Index, unsigned int Texture, bool Additive, float depth,unsigned int HeightMap)
{

	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false
	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);


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
void NormalMapDrawTriangle(
	glm::vec2 p1,
	glm::vec2 p2,
	glm::vec2 p3,
	unsigned int NormalMap,
	glm::vec2 texcoord1,
	glm::vec2 texcoord2,
	glm::vec2 texcoord3 
)
{

	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->NormalMapFBO);
	UseShader(TexturedTriangleShader);
	glBindVertexArray(TexturedTriangleVAO);
	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	p1.x = (p1.x - CameraPosition.x) * aspx;
	p1.y = (p1.y - CameraPosition.y) * aspy;

	p2.x = (p2.x - CameraPosition.x) * aspx;
	p2.y = (p2.y - CameraPosition.y) * aspy;

	p3.x = (p3.x - CameraPosition.x) * aspx;
	p3.y = (p3.y - CameraPosition.y) * aspy;

	GLfloat vertices[3][4] =
	{
		{ p1.x, p1.y,texcoord1.x,texcoord1.y},
		{ p2.x, p2.y,texcoord2.x,texcoord2.y},
		{ p3.x, p3.y,texcoord3.x,texcoord3.y}
	};


	glBindBuffer(GL_ARRAY_BUFFER, TexturedTriangleVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NormalMap);
	glUniform1i(glGetUniformLocation(TexturedTriangleShader, "Texture"), 0);

	glUniform4f(glGetUniformLocation(TexturedTriangleShader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform2f(glGetUniformLocation(TexturedTriangleShader, "scr"), WIDTH, HEIGHT);


	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);



	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->framebuffer);
}

void DrawCircle(glm::vec2 position, float r, glm::vec4 color , bool Lighted , unsigned int NormalMap , int Z_Index, bool Additive )
{
	glm::vec2 scale = glm::vec2(r, r);

	if (Lighted)
		NormalMapDraw(position, scale, NormalMap, 0.0f, Z_Index);

	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);



	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	SceneLayers[SLI].CirclePosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].CircleRotations.push_back(0.0f);
	SceneLayers[SLI].Circlecolors.push_back(color);

}
void DrawCircle(ball b, glm::vec4 color , bool Lighted, unsigned int NormalMap , int Z_Index, bool Additive)
{

	glm::vec2 position = b.position;
	float r = b.r;
	glm::vec2 scale = glm::vec2(r, r);
	if (Lighted)
		NormalMapDraw(position, scale, NormalMap, 0.0f, Z_Index);

	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);

	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false


	SceneLayers[SLI].CirclePosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].CircleRotations.push_back(0.0f);
	SceneLayers[SLI].Circlecolors.push_back(color);
}
void DrawCube(glm::vec2 position, glm::vec2 scale, float rotation, glm::vec4 color, bool Lighted , unsigned int NormalMap , int Z_Index, bool Additive)
{


	if (Lighted)
		NormalMapDraw(position, scale, NormalMap, rotation, Z_Index);

	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);




	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	SceneLayers[SLI].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].QuadRotations.push_back(rotation);

	SceneLayers[SLI].Quadcolors.push_back(color);
	SceneLayers[SLI].Quadcolors.push_back(color);


}
void DrawCube(cube c, glm::vec4 color, float rotation , bool Lighted, unsigned int NormalMap, int Z_Index , bool Additive)
{


	if (Lighted)
		NormalMapDraw(c.position, glm::vec2(c.width, c.height), NormalMap, rotation, Z_Index);

	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	glm::vec2 position = c.position - CameraPosition;
	position *= glm::vec2(aspx, aspy);
	glm::vec2 scale = glm::vec2(c.width, c.height) * glm::vec2(aspx, aspy);

	int SLI =FindSceneLayer(Z_Index, Additive);// ,bool Additive =false

	SceneLayers[SLI].QuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].QuadRotations.push_back(rotation);

	SceneLayers[SLI].Quadcolors.push_back(color);
}
void DrawLine(glm::vec2 p1, glm::vec2 p2, float width, glm::vec4 color , bool Lighted, unsigned int NormalMap , int Z_Index)
{
	glm::vec2 midpos = (p2 + p1) / 2.0f;
	float rotation = get_angle_between_points(p1, p2);
	glm::vec2 dif = p1 - p2;
	float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;
	DrawCube(midpos, glm::vec2(width, length), rotation, color, Lighted, NormalMap, Z_Index);
}
void DrawBall(ball b, glm::vec4 Color1 , glm::vec4 Color2 , bool Lighted, unsigned int NormalMap , int Z_Index)
{

	glm::vec2 univec;
	univec.x = cos(b.rotation * 5) - sin(b.rotation * 5);
	univec.y = sin(b.rotation * 5) + cos(b.rotation * 5);
	DrawLine(b.position - univec * b.r * 0.7f, b.position + univec * b.r * 0.7f, 3.0f, Color2, Lighted, CubeNormalMapTexture, Z_Index);
	DrawLine(b.position + glm::vec2(-univec.y * b.r * 0.7f, univec.x * b.r * 0.7f), b.position + glm::vec2(univec.y * b.r * 0.7f, -univec.x * b.r * 0.7f), 3.0f, Color2, Lighted, CubeNormalMapTexture, Z_Index);
	DrawCircle(b, Color1, Lighted, NormalMap, Z_Index - 1);
}

void LoadTexture(const char* filename, unsigned int* texture, int chanelsAmount)
{
	if (*texture != NULL && glIsTexture(*texture))
	{
		glDeleteTextures(1, texture);
		*texture = NULL;
	}
	if(strlen(filename)<=0)
		return;
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int ImageW, ImageH, nrChannels;
	unsigned char* TextureData = stbi_load(filename, &ImageW, &ImageH, &nrChannels, chanelsAmount);
	if (TextureData)
	{
		if (chanelsAmount == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R, ImageW, ImageH, 0, GL_R, GL_UNSIGNED_BYTE, TextureData);

		if (chanelsAmount == 2)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, ImageW, ImageH, 0, GL_RG, GL_UNSIGNED_BYTE, TextureData);

		if (chanelsAmount == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageW, ImageH, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);

		if (chanelsAmount == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImageW, ImageH, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);


		glGenerateMipmap(GL_TEXTURE_2D);
		
	}
	else
	{
		std::cout << "Failed to load texture: "<< filename << std::endl;
		glDeleteTextures(1, texture);
		*texture = NULL;
	}

	free(TextureData);
	glBindTexture(GL_TEXTURE_2D, 0);

}
void LoadTextureFromData(unsigned int* texture, int width, int height, unsigned char* Data, int chanelsAmount)
{
	if (*texture != NULL)
	{
		glDeleteTextures(1, texture);
		*texture = NULL;
	}
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



	if (chanelsAmount == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R, width, height, 0, GL_R, GL_UNSIGNED_BYTE, Data);

	if (chanelsAmount == 2)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, Data);

	if (chanelsAmount == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);

	if (chanelsAmount == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);


	glGenerateMipmap(GL_TEXTURE_2D);


	//Data = *new unsigned char*;

	/*glDeleteTextures(1, texture);
	*texture = NULL;
	std::cout << "Failed to load texture" << std::endl;
*/
	glBindTexture(GL_TEXTURE_2D, 0);

}
void fLoadTextureFromData(unsigned int* texture, int width, int height, float* Data, int chanelsAmount)
{
	if (*texture != NULL)
	{
		glDeleteTextures(1, texture);
		*texture = NULL;
	}
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



	if (chanelsAmount == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, Data);

	if (chanelsAmount == 2)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, Data);

	if (chanelsAmount == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, Data);

	if (chanelsAmount == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, Data);


	glGenerateMipmap(GL_TEXTURE_2D);


	//Data = *new unsigned char*;

	/*glDeleteTextures(1, texture);
	*texture = NULL;
	std::cout << "Failed to load texture" << std::endl;
*/
	glBindTexture(GL_TEXTURE_2D, 0);

}

void GenNoizeTexture(unsigned int* texture1, int Size, int Layers , float freq , int shape )
{
	//std::cout << "ImputTexture ID  " << *texture1;
	if (*texture1 != NULL && glIsTexture(*texture1))
	{
		//std::cout << "DELETED " << *texture1 << "\n";
		glDeleteTextures(1, texture1);
		*texture1 = NULL;
	}
	unsigned int framebuffer[2], textures[2];

	glGenFramebuffers(2, framebuffer);
	glGenTextures(2, textures);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Size, Size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Size, Size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);

	bool even = false;
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, Size, Size);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	for (int i = 0; i < Layers; i++)
	{
		glm::vec2 rngoffset = glm::vec2(rand() % 10000, rand() % 10000);
		//std::cout<<"texture "<< *texture1 <<"  x " << rngoffset.x << "  y " << rngoffset.y<<"\n";
		UseShader(NoizeGenShader);

		float weight = 1.0f;
		if (i == 0) weight = 1.0f;
		else weight = 1.0f / (pow(2, i));
		// add new layer (higher freq, less wieght)
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[even]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, even ? textures[1] : textures[0]);
		glUniform1i(glGetUniformLocation(NoizeGenShader, "PrevTexture"), 0);

		glUniform1f(glGetUniformLocation(NoizeGenShader, "sizex"), (i + 1) * freq);
		glUniform1f(glGetUniformLocation(NoizeGenShader, "sizey"), (i + 1) * freq);
		glUniform1f(glGetUniformLocation(NoizeGenShader, "weight"), weight);

		glUniform1i(glGetUniformLocation(NoizeGenShader, "shape"), shape);

		glUniform2f(glGetUniformLocation(NoizeGenShader, "offset"), rngoffset.x, rngoffset.y);

		glBindVertexArray(ScreenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);



		even = !even;

	}
	//unsigned int tmp = *texture1;
	glDeleteFramebuffers(2, framebuffer);
	glDeleteTextures(1, &textures[1]);
	*texture1 = textures[0];
	//texture2 = tmp;
	//std::cout << "	ExitTexture ID  " << *texture1 << "\n\n";

	glViewport(0, 0, WIDTH, HEIGHT);



	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->framebuffer);
}
void GenPrimitiveTexture(unsigned int* texture1, int Size, int shape,bool filter )
{
	if (*texture1 != NULL && glIsTexture(*texture1))
	{
		//std::cout << "DELETED " << *texture1 << "\n";
		glDeleteTextures(1, texture1);
		*texture1 = NULL;
	}
	unsigned int framebuffer, texture2;

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &texture2);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Size, Size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	if (!filter)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2, 0);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, Size, Size);
	UseShader(GenPrimitiveTextureShader);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	int i =  shape;
	glUniform1i(glGetUniformLocation(GenNormalMapShader, "Type"), i);

	glBindVertexArray(ScreenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	glDeleteFramebuffers(1, &framebuffer);

	*texture1 = texture2;

	glViewport(0, 0, WIDTH, HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->framebuffer);
}
void GenNormalMapTexture(unsigned int* texture1, int Size, int shape )
{

	glDeleteTextures(1, texture1);
	unsigned int framebuffer, texture2;

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &texture2);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Size, Size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2, 0);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, Size, Size);

	UseShader(GenNormalMapShader);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glUniform1i(glGetUniformLocation(GenNormalMapShader, "Type"), shape);

	glBindVertexArray(ScreenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	glDeleteFramebuffers(1, &framebuffer);

	*texture1 = texture2;

	glViewport(0, 0, WIDTH, HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->framebuffer);
}
void GenLightSphereTexture(unsigned int* texture1, int Size)
{
	if (*texture1 != NULL && glIsTexture(*texture1))
	{
		//std::cout << "DELETED " << *texture1 << "\n";
		glDeleteTextures(1, texture1);
		*texture1 = NULL;
	}
	unsigned int framebuffer;

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, texture1);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, *texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Size, Size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture1, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, Size, Size);

	UseShader(GenLightSphereShader);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glBindVertexArray(ScreenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDeleteFramebuffers(1, &framebuffer);


	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->framebuffer);
	glViewport(0, 0, WIDTH, HEIGHT);
}
void GenGradientTexture(unsigned int* texture1, glm::vec4 Color1 , glm::vec4 Color2 , int Size )
{
	if (*texture1 != NULL && glIsTexture(*texture1))
	{
		//std::cout << "DELETED " << *texture1 << "\n";
		glDeleteTextures(1, texture1);
		*texture1 = NULL;
	}
	unsigned int framebuffer;

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, texture1);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, *texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Size, Size, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture1, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, Size, Size);

	UseShader(GradientGenShader);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glUniform4f(glGetUniformLocation(GradientGenShader, "Color1"), Color1.x, Color1.y, Color1.z, Color1.w);
	glUniform4f(glGetUniformLocation(GradientGenShader, "Color2"), Color2.x, Color2.y, Color2.z, Color2.w);

	glBindVertexArray(ScreenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDeleteFramebuffers(1, &framebuffer);



	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->framebuffer);
	glViewport(0, 0, WIDTH, HEIGHT);
}


void CopyTexture(glm::vec2 size, unsigned int* to,unsigned int from,int mode)// mode: 0 - all, 1 - (r,g,b,1.0f) , 2 - (0,0,0,a), 3 - (a,a,a,a)  
{
	if (from == NULL || !glIsTexture(from))
		return;
	


	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	if (*to == NULL || !glIsTexture(*to))
		glGenTextures(1, to);


	glBindTexture(GL_TEXTURE_2D, *to);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *to, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, size.x, size.y);

	UseShader(CopyTextureShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, from);
	glUniform1i(glGetUniformLocation(CopyTextureShader, "Texture"), 0);

	glUniform1i(glGetUniformLocation(CopyTextureShader, "mode"), mode);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glBindVertexArray(ScreenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDeleteFramebuffers(1, &framebuffer);


	glBindFramebuffer(GL_FRAMEBUFFER, CurrentWindow->framebuffer);
	glViewport(0, 0, WIDTH, HEIGHT);
}

void Texture::Load()
{
	if (texture != NULL && glIsTexture(texture))
		Delete();
	if (Type == 0)
		LoadTexture(FileName.c_str(), &texture);
	else if (Type == 1)
		GenNoizeTexture(&texture, Size, Noize_Layers, Noize_Frequency, ROUND);
	else if (Type == 2)
		GenNoizeTexture(&texture, Size, Noize_Layers, Noize_Frequency, SQUERE);
	else if (Type == 3)
		GenNoizeTexture(&texture, Size, Noize_Layers, Noize_Frequency, SMOOTH_EDGE);
	else if (Type == 4)
		GenGradientTexture(&texture, Gradient_Color1, Gradient_Color2, Size);
	else if (Type == 5)
		GenLightSphereTexture(&texture, Size);
	else if (Type == 6)
		GenPrimitiveTexture(&texture, Size,0,filter);
	else if (Type == 7)
		GenPrimitiveTexture(&texture, Size, 1, filter);
	else if (Type == 8)
		texture = BallNormalMapTexture;
	else if (Type == 9)
		texture = CubeNormalMapTexture;
	else if (Type == 10)
		GenPrimitiveTexture(&texture, Size, 2, filter);
	else if (Type == 11)
		GenPrimitiveTexture(&texture, Size, 3, filter);
	if (texture == NULL)
		std::cout << "Failed to load texture:  " << FileName.c_str() << std::endl;
}
void Texture::Delete()
{
	if(texture!=NULL && glIsTexture(texture) && texture != BallNormalMapTexture&& texture != CubeNormalMapTexture)
		glDeleteTextures(1, &texture);
		
	texture = NULL;
}

void DrawShaderedQuad(glm::vec2 position, glm::vec2 scale, float rotation, unsigned int shaderProgram)
{
	if (shaderProgram != NULL)
	{
		float aspx = ScreenDivisorX * CameraScale.x;
		float aspy = ScreenDivisorY * CameraScale.y;

		position -= CameraPosition;
		position *= glm::vec2(aspx, aspy);
		scale *= glm::vec2(aspx, aspy);

		UseShader(shaderProgram);
		SetShader2f(&shaderProgram, "position", position);
		SetShader2f(&shaderProgram, "scale", scale);
		SetShader1f(&shaderProgram, "rotation", rotation);
		SetShader1f(&shaderProgram, "aspect", (float)HEIGHT / WIDTH);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		DetachShader();
	}
}
void DrawSmoothQuad(glm::vec2 position, glm::vec2 scale, float rotation , glm::vec4 color , int Z_Index , bool Additive , bool flipX , bool flipY )
{


	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);


	int SLI = FindSceneLayer(Z_Index, Additive);// ,bool Additive =false



	
	SceneLayers[SLI].SmoothQuadcolors.push_back(color);
	SceneLayers[SLI].SmoothQuadPosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].SmoothQuadRotations.push_back(rotation);

}



void DrawQuadWithMaterial(cube c, Material material, float rotation, glm::vec4 color, int Z_Index, bool Additive, float depth)
{


	glm::vec2 position = c.position;
	glm::vec2 scale = glm::vec2(c.width, c.height);

	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);


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
void DrawQuadWithMaterial(glm::vec2 position, glm::vec2 scale, Material material, float rotation, glm::vec4 color, int Z_Index, bool Additive, float depth)
{


	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);


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

void DrawTexturedQuad(glm::vec2 position, glm::vec2 scale, unsigned int texture, float rotation, glm::vec4 color, int Z_Index, unsigned int NormalMap, bool Additive, bool flipX, bool flipY, float depth, unsigned int HeightMap )
{


	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);


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
void DrawTexturedQuad(cube c, unsigned int texture, glm::vec4 color, float rotation, int Z_Index , unsigned int NormalMap, bool Additive, bool flipX, bool flipY, float depth, unsigned int HeightMap  )
{


	glm::vec2 position = c.position;
	glm::vec2 scale = glm::vec2(c.width, c.height);

	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	position -= CameraPosition;
	position *= glm::vec2(aspx, aspy);
	scale *= glm::vec2(aspx, aspy);


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
void DrawTexturedLine(unsigned int Texture, glm::vec2 p1, glm::vec2 p2, float width, glm::vec4 color, unsigned int NormalMap, int Z_Index, bool Additive, bool flipX, bool flipY,float depth, unsigned int HeightMap)
{
	glm::vec2 midpos = (p2 + p1) / 2.0f;
	float rotation = get_angle_between_points(p1, p2);
	glm::vec2 dif = p1 - p2;
	float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;
	DrawTexturedQuad(midpos, glm::vec2(width, length), Texture, rotation, color, Z_Index, NormalMap, flipX, flipY,depth,HeightMap);
}
void DrawLineWithMaterial(Material mater, glm::vec2 p1, glm::vec2 p2, float width, glm::vec4 color,int Z_Index, bool Additive)
{
	glm::vec2 midpos = (p2 + p1) / 2.0f;
	float rotation = get_angle_between_points(p1, p2);
	glm::vec2 dif = p1 - p2;
	float length = sqrt(dif.x * dif.x + dif.y * dif.y) * 0.5f;
	DrawQuadWithMaterial(midpos, glm::vec2(width, length),mater,rotation,color, Z_Index, NormalMap);
}

void DrawTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color)
{

	glBindVertexArray(TriangleVAO);
	UseShader(FillTriangleShader);
	float aspx = 1.0f * ScreenDivisorX * CameraScale.x;
	float aspy = 1.0f * ScreenDivisorY * CameraScale.y;
	p1.x = (p1.x - CameraPosition.x) * aspx;
	p1.y = (p1.y - CameraPosition.y) * aspy;

	p2.x = (p2.x - CameraPosition.x) * aspx;
	p2.y = (p2.y - CameraPosition.y) * aspy;

	p3.x = (p3.x - CameraPosition.x) * aspx;
	p3.y = (p3.y - CameraPosition.y) * aspy;

	GLfloat vertices[3][2] =
	{

		{ p1.x, p1.y},
		{ p2.x, p2.y},
		{ p3.x, p3.y}
	};


	glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniform4f(glGetUniformLocation(FillTriangleShader, "Color"), color.x, color.y, color.z, color.w);

	glUniform2f(glGetUniformLocation(FillTriangleShader, "scr"), WIDTH, HEIGHT);


	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);



}
void Window::Draw(int Z_Index)
{
	UI_DrawTexturedQuad(Position, GetSize(), Texture, 0.0f, {1.0f,1.0f,1.0f,1.0f}, Z_Index, false,false,false,true);
}


void DrawTexturedTriangle(
	glm::vec2 p1,
	glm::vec2 p2,
	glm::vec2 p3,
	unsigned int texture,
	glm::vec4 color ,
	glm::vec2 texcoord1,
	glm::vec2 texcoord2,
	glm::vec2 texcoord3
)
{

	UseShader(TexturedTriangleShader);
	glBindVertexArray(TexturedTriangleVAO);
	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;

	p1.x = (p1.x - CameraPosition.x) * aspx;
	p1.y = (p1.y - CameraPosition.y) * aspy;

	p2.x = (p2.x - CameraPosition.x) * aspx;
	p2.y = (p2.y - CameraPosition.y) * aspy;

	p3.x = (p3.x - CameraPosition.x) * aspx;
	p3.y = (p3.y - CameraPosition.y) * aspy;

	GLfloat vertices[3][4] =
	{
		{ p1.x, p1.y,texcoord1.x,texcoord1.y},
		{ p2.x, p2.y,texcoord2.x,texcoord2.y},
		{ p3.x, p3.y,texcoord3.x,texcoord3.y}
	};


	glBindBuffer(GL_ARRAY_BUFFER, TexturedTriangleVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(TexturedTriangleShader, "Texture"), 0);

	glUniform4f(glGetUniformLocation(TexturedTriangleShader, "color"), color.x, color.y, color.z, color.w);
	glUniform2f(glGetUniformLocation(TexturedTriangleShader, "scr"), WIDTH, HEIGHT);


	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	DetachShader();


}
//void DrawPolygon(polygon p, int Z_Index)
//{
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, p.Texture);
//	UseShader(TexturedTriangleShader);
//	glUniform1i(glGetUniformLocation(TexturedTriangleShader, "Texture"), 0);
//
//	glUniform4f(glGetUniformLocation(TexturedTriangleShader, "color"), p.color.x, p.color.y, p.color.z, p.color.w);
//	glUniform2f(glGetUniformLocation(TexturedTriangleShader, "scr"), WIDTH, HEIGHT);
//
//	glBindVertexArray(p.Data->VAO);
//	glDrawElements(GL_TRIANGLES, p.Data->indexes.size()*3, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//
//}


void DrawPolygon(polygon* p, int Z_Index, bool Additive)
{
	if (p->Data == NULL)
		return;
	if (p->Data->VAO == NULL)
		return;
	int SLI = FindSceneLayer(Z_Index, Additive);

	int PA = -1;

	Material m;
	m.Texture = p->Texture;
	m.NormalMap = p->NormalMap;
	m.Specular = 0;
	m.Reflective = 0;
	m.HeightMap = 0;

	for (int i = 0; i < SceneLayers[SLI].Polygons.size(); i++)
		if (SceneLayers[SLI].Polygons[i].material == m && SceneLayers[SLI].Polygons[i].VAO == p->Data->VAO)
			PA = i;
	if (PA == -1)
	{
		PolygonArray NewTQA;
		NewTQA.material = m;
		NewTQA.VAO = p->Data->VAO;
		SceneLayers[SLI].Polygons.push_back(NewTQA);
		for (int i = 0; i < SceneLayers[SLI].Polygons.size(); i++)
			if (SceneLayers[SLI].Polygons[i].material == m && SceneLayers[SLI].Polygons[i].VAO == p->Data->VAO)
				PA = i;
	}
	float aspx = ScreenDivisorX * CameraScale.x;
	float aspy = ScreenDivisorY * CameraScale.y;
	glm::vec2 position = (p->Position - CameraPosition) * glm::vec2(aspx,aspy) ;
	glm::vec2 scale = p->Scale * glm::vec2(aspx, aspy);


	SceneLayers[SLI].Polygons[PA].colors.push_back(p->color);
	SceneLayers[SLI].Polygons[PA].PosScale.push_back(glm::vec4(position, scale));
	SceneLayers[SLI].Polygons[PA].Rotations.push_back(p->Rotation);
	SceneLayers[SLI].Polygons[PA].Depths.push_back(0.0f);
	
	//if (p->NormalMap != NULL)
	/*{
		PA = -1;
		for (int i = 0; i < SceneLayers[SLI].PolygonNormalMaps.size(); i++)
			if (SceneLayers[SLI].PolygonNormalMaps[i].Material.NormalMap == p->NormalMap &&
				SceneLayers[SLI].PolygonNormalMaps[i].Material.Texture == p->Texture &&
				SceneLayers[SLI].PolygonNormalMaps[i].VAO == p->Data->VAO)
				PA = i;

		if (PA == -1)
		{
			PolygonArray NewPA;
			NewPA.Material.NormalMap = p->NormalMap;
			NewPA.Material.Texture = p->Texture;
			NewPA.VAO = p->Data->VAO;
			NewPA.Size = p->Data->data.size();

			SceneLayers[SLI].PolygonNormalMaps.push_back(NewPA);
			for (int i = 0; i < SceneLayers[SLI].PolygonNormalMaps.size(); i++)
				if (SceneLayers[SLI].PolygonNormalMaps[i].Material.NormalMap == p->NormalMap &&
					SceneLayers[SLI].PolygonNormalMaps[i].Material.Texture == p->Texture &&
					SceneLayers[SLI].PolygonNormalMaps[i].VAO == p->Data->VAO)
					PA = i;
		}	
		SceneLayers[SLI].PolygonNormalMaps[PA].colors.push_back(p->color);
		SceneLayers[SLI].PolygonNormalMaps[PA].PosScale.push_back(glm::vec4(position, scale));
		SceneLayers[SLI].PolygonNormalMaps[PA].Rotations.push_back(p->Rotation);
	}*/


}

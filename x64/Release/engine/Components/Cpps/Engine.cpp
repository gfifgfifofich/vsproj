#define EngineMainModule
#include "../Engine.h"


void _Startpthreads();
void _Deletepthreads();

void initEngine(const char* Name, GLuint width, GLuint height, bool fullScreen)
{
 	ParticleMultithreading = true;
	threadcount = std::thread::hardware_concurrency();

	unsigned int DownScaleBuffers[8] = {0};

	glm::ivec2 DownScaleBufferSizes[9] = { {0,0} };

	BackgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	scrpos = glm::vec2(0, 0);
	CameraPosition = { 0.0f ,0.0f };
	CameraScale = { 1.0f,1.0f };

	lastX = WIDTH / 2.0;
	lastY = HEIGHT / 2.0;


	WIDTH = width;
	HEIGHT = height;

	window_id = 0;

	ScreenAspectRatio = (float)WIDTH / HEIGHT;


	ScreenDivisorX = (WIDTH / (ScreenAspectRatio * 2)) - ScreenAspectRatio * 0.75f;
	ScreenDivisorY = (HEIGHT / 2.0f) - 1.0f;

	ScreenDivisorX = 1.0f / ScreenDivisorX;
	ScreenDivisorY = 1.0f / ScreenDivisorY;
	ScaleMultiplyer = 1.0f / HEIGHT * 2.0f;
	//AL_init();
	VSync;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	if (fullScreen)
		window = glfwCreateWindow(WIDTH, HEIGHT, Name, glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(WIDTH, HEIGHT, Name, nullptr, nullptr);
	glfwMakeContextCurrent(window);


	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCharCallback(window, character_callback);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int swidth, sheight;
	glfwGetFramebufferSize(window, &swidth, &sheight);

	glViewport(0, 0, swidth, sheight);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//ScreenShaderStuff for HDR and postprocessing

	//GLuint InctanceQuadShader;
	//GLuint InstanceTexturedQuadShader;

	//LoadShader(&InctanceQuadShader, "engine/Shaders/Quad/instance.vert", "engine/Shaders/Quad/Quad.frag");
	//LoadShader(&InstanceTexturedQuadShader, "engine/Shaders/InstancedTexturedQuad/InstancedTexturedQuad.vert", "engine/Shaders/InstancedTexturedQuad/InstancedTexturedQuad.frag");

	PreLoadShaders();

	float ScreenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &ScreenVAO);
	glGenBuffers(1, &ScreenVBO);
	glBindVertexArray(ScreenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenVertices), ScreenVertices, GL_STATIC_DRAW);



	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	/*glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	glGenTextures(1, &ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer, 0);*/




	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Bloom stuff
	//setting weights

	//creating buffers for downscale



	// downscale frame buffer and textures
	unsigned int downscaleFBO[8];
	glGenFramebuffers(8, downscaleFBO);

	float scalefactor = 0.75f;

	for (int i = 0; i < 8; i++)
	{

		glBindFramebuffer(GL_FRAMEBUFFER, downscaleFBO[i]);
		glGenTextures(1, &DownScaleBuffers[i]);
		glBindTexture(GL_TEXTURE_2D, DownScaleBuffers[i]);

		DownScaleBufferSizes[i].x = WIDTH * scalefactor;
		DownScaleBufferSizes[i].y = HEIGHT * scalefactor;
		scalefactor *= 0.65f;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, DownScaleBufferSizes[i].x, DownScaleBufferSizes[i].y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, DownScaleBuffers[i], 0);

	}

	DownScaleBufferSizes[8].x = WIDTH * scalefactor;
	DownScaleBufferSizes[8].y = HEIGHT * scalefactor;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	/*glGenFramebuffers(1, &NormalMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, NormalMapFBO);
	glGenTextures(1, &NormalMapColorBuffer);
	glBindTexture(GL_TEXTURE_2D, NormalMapColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, NormalMapColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &LightColorFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, LightColorFBO);
	glGenTextures(1, &LightColorBuffer);
	glBindTexture(GL_TEXTURE_2D, LightColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, LightColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/


	//HDR
	unsigned int hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// create 1 floating point color buffers
	GLuint colorBuffer2;
	glGenTextures(1, &colorBuffer2);

	glBindTexture(GL_TEXTURE_2D, colorBuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer2, 0);


	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring
	unsigned int blurFBO[2];
	unsigned int blurColorbuffers[2];
	glGenFramebuffers(2, blurFBO);
	glGenTextures(2, blurColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
		glBindTexture(GL_TEXTURE_2D, blurColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete ping!" << std::endl;
	}




	float Vertices[] = {
		// coordinates  // UV
		-1.0f,  1.0f, 0.0f,1.0f,
		 1.0f, -1.0f, 1.0f,0.0f,
		-1.0f, -1.0f, 0.0f,0.0f,

		-1.0f,  1.0f, 0.0f,1.0f,
		 1.0f, -1.0f, 1.0f,0.0f,
		 1.0f,  1.0f, 1.0f,1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);







	glGenVertexArrays(1, &CircleVAO);
	glGenBuffers(1, &CircleVBO);

	glBindVertexArray(CircleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CircleVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);


	glGenVertexArrays(1, &TextVAO);
	glGenBuffers(1, &TextVBO);
	glBindVertexArray(TextVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glGenVertexArrays(1, &TriangleVAO);
	glGenBuffers(1, &TriangleVBO);
	glBindVertexArray(TriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 2, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &TexturedTriangleVAO);
	glGenBuffers(1, &TexturedTriangleVBO);
	glBindVertexArray(TexturedTriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TexturedTriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");



	int wId = CreateWindow();
	Window* w = GetWindow(wId);
	Windows[0].Init({ WIDTH,HEIGHT });

	Windows[0].Use();

	GenNormalMapTexture(&BallNormalMapTexture, 1000, 0);
	GenNormalMapTexture(&CubeNormalMapTexture, 1000, 1);
	GenLightSphereTexture(&LightSphereTexture, 100);

	GenPrimitiveTexture(&FlatColorCircleTexture, 400, ROUND);
	GenPrimitiveTexture(&FlatColorTexture, 100, SQUERE);

	//unsigned int instanceCircleVBO[3];
	//unsigned int instanceNormalMapCircleVBO[2];
	//unsigned int instanceVBO[3];
	//unsigned int instanceNormalMapCubeVBO[2];
	//unsigned int instanceNormalMapTextureVBO[2];
	//unsigned int instanceTexturedQuadVBO[3];





	LoadFont("engine/fonts/LiberationSansR.ttf");


	On_Create();

	if (VSync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	std::cout << "Scene created\n";


	_Startpthreads();

	bool loop = false;
	while (!glfwWindowShouldClose(window)) {

		if (!loop)
			std::cout << "Running...\n";
		glfwPollEvents();


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		if(glfwWindowShouldClose(window)) return;
		//ScreenMousePosition = { (lastX - WIDTH * 0.5f) ,(-lastY + HEIGHT * 0.5f) };

		//MousePosition.x = (lastX - WIDTH * 0.5f) / CameraScale.x + CameraPosition.x;
		//MousePosition.y = (-lastY + HEIGHT * 0.5f) / CameraScale.y + CameraPosition.y;

		for (int i = 0; i < 1024; i++)
		{
			if (!Holdingkey[i] && keys[i]) JustPressedkey[i] = true;
			else JustPressedkey[i] = false;

			if (Holdingkey[i] && !keys[i]) JustReleasedkey[i] = true;
			else JustReleasedkey[i] = false;

			if (keys[i]) Holdingkey[i] = true;
			else Holdingkey[i] = false;

			bJustPressedkey[i] = JustPressedkey[i];
			bJustReleasedkey[i] = JustReleasedkey[i];
			bHoldingkey[i] = Holdingkey[i];
		}
		for (int i = 0; i < 64; i++)
		{
			if (!Holdingbutton[i] && buttons[i]) JustPressedbutton[i] = true;
			else JustPressedbutton[i] = false;

			if (Holdingbutton[i] && !buttons[i]) JustReleasedbutton[i] = true;
			else JustReleasedbutton[i] = false;

			if (buttons[i]) Holdingbutton[i] = true;
			else Holdingbutton[i] = false;

			bJustPressedbutton[i] = JustPressedbutton[i];
			bJustReleasedbutton[i] = JustReleasedbutton[i];
			bHoldingbutton[i] = Holdingbutton[i];
		}

		if (!HoldingLMB && buttons[GLFW_MOUSE_BUTTON_1]) JustPressedLMB = true;
		else JustPressedLMB = false;

		if (HoldingLMB && !buttons[GLFW_MOUSE_BUTTON_1]) ReleasedLMB = true;
		else ReleasedLMB = false;

		if (buttons[GLFW_MOUSE_BUTTON_1]) HoldingLMB = true;
		else HoldingLMB = false;

		bJustPressedLMB= JustPressedLMB;
		bReleasedLMB = ReleasedLMB;
		bHoldingLMB = HoldingLMB;

		bscrollmovement = scrollmovement;


		// update Scene
		Windows[0].Use(false);
		On_Update();
		Windows[0].Use(false);
		scrollmovement = 0;
		bscrollmovement = 0;
		TextFromKeyboard.clear();
		

		//Drawing from back, so the main scene (window 0) will get updated data.
		for (int i = Windows.size() - 1; i >= 0; i--)
			if (Windows[i].AutoDraw)
				Windows[i]._Draw();
		// Post Processing

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT);


		glBindVertexArray(ScreenVAO);
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[0]);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		UseShader(shaderBloom);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Windows[0].Texture);
		glUniform1i(glGetUniformLocation(shaderBloom, "screenTexture"), 1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		DetachShader();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (bloom) 
		{
			

			glBindVertexArray(ScreenVAO);
			UseShader(DownsampleBlur);
			for (int i = 0; i < 8; i++)
			{
				glViewport(0, 0, DownScaleBufferSizes[i].x, DownScaleBufferSizes[i].y);
				glBindFramebuffer(GL_FRAMEBUFFER, downscaleFBO[i]);

				//input texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, i == 0 ? blurColorbuffers[0] : DownScaleBuffers[i - 1]);
				glUniform1i(glGetUniformLocation(DownsampleBlur, "srcTexture"), 0);


				glUniform2f(glGetUniformLocation(DownsampleBlur, "srcResolution"), DownScaleBufferSizes[i].x, DownScaleBufferSizes[i].y);

				glDrawArrays(GL_TRIANGLES, 0, 6);

			}


			DetachShader();

			glBindVertexArray(ScreenVAO);
			UseShader(UpsampleBlur);
			glUniform1f(glGetUniformLocation(UpsampleBlur, "filterRadius"), 10.0f);
			for (int i = 7; i > 0; i--)
			{
				glViewport(0, 0, DownScaleBufferSizes[i - 1].x, DownScaleBufferSizes[i - 1].y);
				glBindFramebuffer(GL_FRAMEBUFFER, downscaleFBO[i - 1]);

				//input texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, DownScaleBuffers[i]);
				glUniform1i(glGetUniformLocation(UpsampleBlur, "srcTexture"), 0);


				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, DownScaleBuffers[i - 1]);
				glUniform1i(glGetUniformLocation(UpsampleBlur, "PrevTexture"), 1);

				glUniform1f(glGetUniformLocation(UpsampleBlur, "weight"), bloomLevels[i]);
				if (i - 1 == 0)
					glUniform1f(glGetUniformLocation(UpsampleBlur, "prewweight"), bloomLevels[0]);
				else
					glUniform1f(glGetUniformLocation(UpsampleBlur, "prewweight"), 1.0f);

				glDrawArrays(GL_TRIANGLES, 0, 6);

			}
			DetachShader();
		}
		glViewport(0, 0, WIDTH, HEIGHT);


		glBindVertexArray(ScreenVAO);
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[0]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		UseShader(ScrShade);

		glUniform1f(glGetUniformLocation(ScrShade, "bloom"), bloom);
		glUniform1f(glGetUniformLocation(ScrShade, "exposure"), SceneExposure);

		glUniform1f(glGetUniformLocation(ScrShade, "BloomStrength"), bloomIntensity);


		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Windows[0].Texture);
		glUniform1i(glGetUniformLocation(ScrShade, "scene"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DownScaleBuffers[0]);
		glUniform1i(glGetUniformLocation(ScrShade, "bloomBlur"), 0);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		DetachShader();

		unsigned int fm = blurColorbuffers[0];

		////Clearing windows that are marked to autoclear
		//for (int i = Windows.size() - 1; i >= 0; i--)
		//	if (Windows[i].Autoclear) Windows[i].Clear(Windows[i].backgroundColor);

		UseShader(Chromatic);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, fm);

		ChromaticPoint = glm::vec2(0.5f, 0.5f);
		glUniform2f(glGetUniformLocation(Chromatic, "point"), ChromaticPoint.x, ChromaticPoint.y);


		glUniform1f(glGetUniformLocation(Chromatic, "strength"), ChromaticStrength);

		glDrawArrays(GL_TRIANGLES, 0, 6);// the last drawing, directly to screen



		glBindVertexArray(0);
		DetachShader();

		if (!loop)
			std::cout << "Post processing applyied\n";
		
		delta = (glfwGetTime() - lt) ;
		lt = glfwGetTime();
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);

		if (!loop)
			std::cout << "Frame swapped\n";

		loop = true;


	}
	glfwTerminate();
	_Deletepthreads();
	std::cout << "Exiting";
	//AL_Destroy();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollmovement = yoffset;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button >= 0 && button < 64)
	{
		if (action == GLFW_PRESS)
			buttons[button] = true;
		else if (action == GLFW_RELEASE)
			buttons[button] = false;
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	lastX = xpos;
	lastY = ypos;


}
void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	char ch;
	memcpy(&ch, &codepoint, 1);
	TextFromKeyboard += ch;
}

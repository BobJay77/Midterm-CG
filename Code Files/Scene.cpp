#include "Scene.h"

std::unique_ptr<GameObject> playerEntity;
std::unique_ptr<GameObject> sunEntity;

enum class EntityType { PLAYER, ENEMY };

enum class TextureType { START = 3, TEMPSHIP, TEMPSUN, TEMPLAVA, TEMPSAND, TEMPPLANET, TEMPMOON, TEMPROCK, TEMPICE };

enum class PlayerMesh { PLAYERSHIPPENCIL, PLAYERSHIPBAT };

enum class PlanetMesh { SOLARI = 2, VERASTEN, YECHIN, KERANTIA, LUNARI, GUERISTIS, KEMINTH };

std::unique_ptr<GameObject> effect;

vector<Mesh*> Scene::m_meshes;
vector<Texture*> Scene::m_textures;


bool texTglPressed = false;
bool bloomToggle = false;
bool lightingTog = false;
bool ambTog = false;
bool specTog = false;
bool ambSpecTog = false;

Scene::Scene(string name)
	:m_name(name)
{

	if (m_textures.size() < 1) {
		// HUD //
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/Health_4-4.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/Abilities_Unavailable_Temp.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/PowerUP_Unavailable_Temp.png"));
		/////////
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/TitleScreen.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempSpaceship.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempSun.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempLava.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempSand.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempPlanet.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempMoon.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempRock.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempIce.png"));
	}
	if (m_meshes.size() < 1) {
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Ships/PlayerShips/PlayerShipPencil.obj", *m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Ships/PlayerShips/PlayerShipBat.obj", *m_meshes[int(PlayerMesh::PLAYERSHIPBAT)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Solari.obj", *m_meshes[int(PlanetMesh::SOLARI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Verasten.obj", *m_meshes[int(PlanetMesh::VERASTEN)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Yechin.obj", *m_meshes[int(PlanetMesh::YECHIN)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Kerantia.obj", *m_meshes[int(PlanetMesh::KERANTIA)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Lunari.obj", *m_meshes[int(PlanetMesh::LUNARI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Gueristis.obj", *m_meshes[int(PlanetMesh::GUERISTIS)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Keminth.obj", *m_meshes[int(PlanetMesh::KEMINTH)]);
	}
}

string Scene::GetName()
{
	return m_name;
}

void Scene::InitScene()
{
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr)
	m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {
		//add entites in here if not initialised
	}
}

void Scene::SetWindow(GLFWwindow* window)
{
	m_window = window;
}

entt::registry* Scene::GetScene()
{
	return m_sceneReg;
}

Menu::Menu(string name, unsigned int* num, bool* change)
	:Scene(name)
{
	SceneNo = num;
	switchIt = change;
}

void Menu::InitScene()
{
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr) 
		m_sceneReg = new entt::registry();
	
	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {

		auto cameraEntity = GameObject::Allocate();
		cameraEntity->AttachComponent<Transform>();
		camera = &cameraEntity->AttachComponent<Camera>(cameraEntity->GetID());
		cameraEntity->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 100.0f));

		if (m_name == "Start_Screen") {
			auto titleScreen = GameObject::Allocate();
			titleScreen->AttachComponent<Sprite2D>(m_textures[int(TextureType::START)], titleScreen->GetID(), 100, 100);
			titleScreen->AttachComponent<Transform>();
			//titleScreen->GetComponent<Transform>().SetLocalScale(temp);
		}
		else if (m_name == "Main_Menu") {

		}
		else if (m_name == "Pause_Menu") {

		}
	}
}

void Menu::Update(float deltaTime)
{
	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) {	transform.UpdateGlobal();});

	// Key Input
	KeyInput();

	// Camera Update
	camera->Update();
}

void Menu::KeyInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	if (glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_19);
	}
}

void Menu::GamepadInput()
{
	if (gamepad.getGamepadInput()) {
		//button input
		if (gamepad.buttons.A) {

		}
		else if (gamepad.buttons.B) {

		}
		else if (gamepad.buttons.X) {

		}
		else if (gamepad.buttons.Y) {

		}

		//trigger input
		if (gamepad.trigger.LT > 0 && gamepad.trigger.LT < 1) {

		}

		else if (gamepad.trigger.LT > -1 && gamepad.trigger.LT < 0) {

		}

		if (gamepad.trigger.RT > 0 && gamepad.trigger.RT < 1) {

		}

		else if (gamepad.trigger.RT > -1 && gamepad.trigger.RT < 0) {

		}

		//axes input
		if (gamepad.axes[Direction::LEFT].X > 0 && gamepad.axes[Direction::LEFT].X < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].X > -1 && gamepad.axes[Direction::LEFT].X < 0) {

		}

		if (gamepad.axes[Direction::LEFT].Y > 0 && gamepad.axes[Direction::LEFT].Y < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].Y > -1 && gamepad.axes[Direction::LEFT].Y < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].X > 0 && gamepad.axes[Direction::RIGHT].X < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].X > -1 && gamepad.axes[Direction::RIGHT].X < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].Y > 0 && gamepad.axes[Direction::RIGHT].Y < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].Y > -1 && gamepad.axes[Direction::RIGHT].Y < 0) {

		}
	}

	else
		std::cout << "No controller connected" << std::endl;

	

}

void Menu::Render(float deltaTime)
{
	m_sceneReg->view<Sprite2D>().each([=](Sprite2D& render) { render.Draw(camera); });
}

Universe::Universe(string name, unsigned int* num, bool* change)
	:Scene(name)
{
	SceneNo = num;
	switchIt = change;
}

void Universe::InitScene()
{
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr)
		m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {

		if (m_name == "Universe_19") {
			// Camera
			auto cameraEntity = GameObject::Allocate();
			cameraEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 13, 25));
			camera = &cameraEntity->AttachComponent<Camera>(cameraEntity->GetID());
			camera->PerspectiveProj(1.0f, 100000.0f, Application::GetWindowWidth() / Application::GetWindowHeight(), 1.0f);

			// Player
			playerEntity = GameObject::Allocate();
			MainPlayerID = playerEntity->GetID();
			playerEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			playerEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), MainPlayerID, *m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)], m_textures[int(TextureType::TEMPSHIP)]);
			playerEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(0.75));
			playerEntity->GetComponent<Transform>().SetLocalRot(0, 180, 0);
			 
			// Solari
			sunEntity = GameObject::Allocate();
			sunEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			sunEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), sunEntity->GetID(), *m_meshes[int(PlanetMesh::SOLARI)], m_textures[int(TextureType::TEMPSUN)], true);
			sunEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));
		
			//HUD
			auto health = GameObject::Allocate();
			health->AttachComponent<Sprite2D>(m_textures[0], health->GetID(), 15, 15);
			health->AttachComponent<Transform>().SetLocalPos(glm::vec3(-80, -80, -10));
			auto abilities = GameObject::Allocate();
			abilities->AttachComponent<Sprite2D>(m_textures[1], abilities->GetID(), 15, 15);
			abilities->AttachComponent<Transform>().SetLocalPos(glm::vec3(80, -80, -10));
			auto powerUp = GameObject::Allocate();
			powerUp->AttachComponent<Sprite2D>(m_textures[2], powerUp->GetID(), 6, 45);
			powerUp->AttachComponent<Transform>().SetLocalPos(glm::vec3(-90, 10, -10));

			//effects
			effect = GameObject::Allocate();
			effect->AttachComponent<PostEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
			effect->AttachComponent<BloomEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
			/*effect->AttachComponent<GreyscaleEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
			effect->AttachComponent<SepiaEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());*/
			/*effect->AttachComponent<ColorCorrectionEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
			effect->GetComponent<ColorCorrectionEffect>().AddLUT("Resource Files/LUTs/NeutralLUT.cube");*/

			//Setting Parent/childe
			cameraEntity->GetComponent<Transform>().SetParent(&playerEntity->GetComponent<Transform>());
			health->GetComponent<Transform>().SetParent(&cameraEntity->GetComponent<Transform>());
			abilities->GetComponent<Transform>().SetParent(&cameraEntity->GetComponent<Transform>());
			powerUp->GetComponent<Transform>().SetParent(&cameraEntity->GetComponent<Transform>());
		}
		else if (m_name == "Universe_27") {

		}
		else if (m_name == "Universe_5") {

		}

		Skybox::Init();
	}
}

void Universe::Update(float deltaTime)
{
	// Key Input
	KeyInput();

	// Camera Update 
	camera->Update();

	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) {	transform.UpdateGlobal(); });
}

void Universe::Render(float deltaTime)
{
	if (bloomToggle) {
		effect->GetComponent<PostEffect>().Clear();
		effect->GetComponent<BloomEffect>().Clear();

		effect->GetComponent<PostEffect>().BindBuffer(0);

		m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.Draw(); });
		Skybox::Draw(camera->GetView(), camera->GetProj());
	/*	m_sceneReg->view<Sprite2D>().each([=](Sprite2D& renderer) {renderer.Draw(camera); });*/

		effect->GetComponent<PostEffect>().UnbindBuffer();
		effect->GetComponent<BloomEffect>().ApplyEffect(&effect->GetComponent<PostEffect>());
		effect->GetComponent<BloomEffect>().DrawToScreen();
	}
	 
	else {
		m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.Draw(); });
		Skybox::Draw(camera->GetView(), camera->GetProj());
		m_sceneReg->view<Sprite2D>().each([=](Sprite2D& renderer) {renderer.Draw(camera); });
	}
}

void Universe::KeyInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	// Player Movement //
	auto& playerEnt = GameObject::GetComponent<Transform>(MainPlayerID);

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, -5);
		playerEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, 5);
		playerEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, 0.5f, 0.0f);
		playerEnt.RotateLocal(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{ 
		glm::vec3 temp = glm::vec3(0.0f, -0.5f, 0.0f);
		playerEnt.RotateLocal(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS && !lightingTog) {
		lightingTog = true;

		playerEntity->GetComponent<StaticRenderer>().toggleLighting();
		sunEntity->GetComponent<StaticRenderer>().toggleLighting();

	}
	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_RELEASE) {
		lightingTog = false;

	}

	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS && !ambTog) {
		ambTog = true;
		playerEntity->GetComponent<StaticRenderer>().toggleAmbient();
		sunEntity->GetComponent<StaticRenderer>().toggleAmbient();

	}
	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_RELEASE) {
		ambTog = false;

	}

	if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS && !specTog) {
		specTog = true;
		playerEntity->GetComponent<StaticRenderer>().toggleSpecular();
		sunEntity->GetComponent<StaticRenderer>().toggleSpecular();
	}
	if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_RELEASE) {
		specTog = false;

	}

	if (glfwGetKey(m_window, GLFW_KEY_4) == GLFW_PRESS && !ambSpecTog) {
		ambSpecTog = true;
		playerEntity->GetComponent<StaticRenderer>().toggleBoth();
		sunEntity->GetComponent<StaticRenderer>().toggleBoth();
	}
	if (glfwGetKey(m_window, GLFW_KEY_4) == GLFW_RELEASE) {
		ambSpecTog = false;

	}

	if (glfwGetKey(m_window, GLFW_KEY_5) == GLFW_PRESS && !bloomToggle) {
		bloomToggle = true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_5) == GLFW_RELEASE) {
		bloomToggle = false;
	}
	// Toggle Textures //
	if (glfwGetKey(m_window, GLFW_KEY_6) == GLFW_PRESS && !texTglPressed) {
		m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.toggleTexture(); });
		texTglPressed = true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_6) == GLFW_RELEASE) {
		texTglPressed = false;
	}
}

void Universe::GamepadInput()
{
	if (gamepad.getGamepadInput()) {
		//button input
		if (gamepad.buttons.A) {

		}
		else if (gamepad.buttons.B) {

		}
		else if (gamepad.buttons.X) {

		}
		else if (gamepad.buttons.Y) {

		}

		//trigger input
		if (gamepad.trigger.LT > 0 && gamepad.trigger.LT < 1) {

		}

		else if (gamepad.trigger.LT > -1 && gamepad.trigger.LT < 0) {

		}

		if (gamepad.trigger.RT > 0 && gamepad.trigger.RT < 1) {

		}

		else if (gamepad.trigger.RT > -1 && gamepad.trigger.RT < 0) {

		}

		//axes input
		if (gamepad.axes[Direction::LEFT].X > 0 && gamepad.axes[Direction::LEFT].X < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].X > -1 && gamepad.axes[Direction::LEFT].X < 0) {

		}

		if (gamepad.axes[Direction::LEFT].Y > 0 && gamepad.axes[Direction::LEFT].Y < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].Y > -1 && gamepad.axes[Direction::LEFT].Y < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].X > 0 && gamepad.axes[Direction::RIGHT].X < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].X > -1 && gamepad.axes[Direction::RIGHT].X < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].Y > 0 && gamepad.axes[Direction::RIGHT].Y < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].Y > -1 && gamepad.axes[Direction::RIGHT].Y < 0) {

		}
	}

	else
		std::cout << "No controller connected" << std::endl;

}

#include "StaticRenderer.h"

std::vector<Shader*> StaticRenderer::m_shader;

StaticRenderer::StaticRenderer(entt::entity camera, entt::entity entity, const Mesh& mesh, Texture* texture, bool lightSource) {
	m_lightSource = lightSource;
	m_tex = texture;
	m_camera = camera;
	m_entity = entity;
	m_vao = std::make_unique<VertexArray>();

	if (!m_shader.size()) {
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_untex_unlit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_untex_lit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_tex_unlit.frag"));
		m_shader.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader_tex_lit.frag"));
	}

	SetVAO(mesh);
}

void StaticRenderer::SetVAO(const Mesh& mesh) {
	const VertexBuffer* vbo;

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::POSITION)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::POSITION);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::NORMAL)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::NORMAL);

	if ((vbo = mesh.GetVBO(Mesh::VertexAttrib::TEXCOORD)) != nullptr)
		m_vao->BindBuffer(*vbo, (GLint)Mesh::VertexAttrib::TEXCOORD);
}

void StaticRenderer::toggleTexture() {
	m_textureToggle = !m_textureToggle;
}

void StaticRenderer::toggleAmbient()
{
	m_ambientToggle = !m_ambientToggle;
}

void StaticRenderer::toggleSpecular()
{
	m_specularToggle = !m_specularToggle;
}

void StaticRenderer::toggleLighting()
{
	m_lightingToggle = !m_lightingToggle;
}

void StaticRenderer::toggleBoth()
{
	m_ambientToggle = !m_ambientToggle;
	m_specularToggle = !m_specularToggle;
}

void StaticRenderer::Draw() {
	auto& transform = GameObject::GetComponent<Transform>(m_entity);

	//TODO: Material/Texture and Shader implementation
	if (m_tex == nullptr || !m_textureToggle) {
		if (!m_lightSource)
			currShader = 0;
		else
			currShader = 1;
	}
	else if (m_textureToggle && m_tex != nullptr) {
		if (!m_lightSource)
			currShader = 2;
		else
			currShader = 3;
	}

	m_shader[currShader]->use();

	if (m_tex != nullptr) {
		m_shader[currShader]->set1i(0, "albedo");
		m_tex->bind(0);
	}

	m_shader[currShader]->setVec3f(GameObject::GetComponent<Transform>(m_camera).GetLocalPos(), "camPos");
	m_shader[currShader]->setMat4fv(GameObject::GetComponent<Camera>(m_camera).GetViewProj(), "ViewProjection");
	m_shader[currShader]->setMat4fv(transform.GetGlobal(), "ModelMatrix");
	m_shader[currShader]->setMat3fv(transform.GetNormal(), "NormalMatrix");

	m_shader[currShader]->setVec3f(glm::vec3(0 + float(m_lightingToggle), 0 + float(m_lightingToggle), 0 + float(m_lightingToggle)), "lightColor");


		m_shader[currShader]->set1f(float(m_ambientToggle), "ambientPower");

		m_shader[currShader]->set1f(float(m_specularToggle), "specularStrength");


	m_vao->DrawArray();
	m_shader[currShader]->unuse();
	glBindVertexArray(GL_NONE);
}

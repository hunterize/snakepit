#include "VelocityBallRenderer.h"



CVelocityBallRenderer::CVelocityBallRenderer(int width, int height) :
	m_iScreenWidth(width), m_iScreenHeight(height)
{
}


void CVelocityBallRenderer::RenderBalls(SnakEngine::SpriteBatch& spriteBatch, const std::vector<CBall>& balls, const glm::mat4& projectionMatrix)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (m_pProgram == nullptr)
	{
		m_pProgram = std::make_unique<SnakEngine::GLSLProgram>();

		m_pProgram->CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_pProgram->AddAttribute("vertexPosition");
		m_pProgram->AddAttribute("vertexColor");
		m_pProgram->AddAttribute("vertexUV");
		m_pProgram->LinkShaders();
	}

	m_pProgram->use();

	spriteBatch.Begin();

	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = m_pProgram->GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	GLint pUniform = m_pProgram->GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (auto& ball : balls)
	{
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.m_cPosition.x - ball.m_fRadius, ball.m_cPosition.y - ball.m_fRadius, ball.m_fRadius * 2.0f, ball.m_fRadius * 2.0f);

		SnakEngine::Color color;
		GLubyte colValue = (GLubyte)(glm::clamp(glm::length(ball.m_cVelocity) * 100, 0.0f, 255.0f));
		color.r = 10;
		color.g = (GLubyte)((ball.m_cPosition.x / m_iScreenWidth) * 255.0f);
		color.b = (GLubyte)((ball.m_cPosition.y / m_iScreenHeight) * 255.0f);
		color.a = colValue;

		spriteBatch.Draw(destRect, uvRect, ball.m_uiTextureID, 0.0f, color);
	}

	spriteBatch.End();
	spriteBatch.RenderBatch();

	m_pProgram->unuse();
}

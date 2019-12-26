#pragma once
#include "MatSkyBox.h"
#include "Engine/Components/Camera.h"
#include "../Object.h"
#include "Engine/DrawCommand/Mesh.h"
#include "Engine/Global.h"

namespace TEngine {
	using namespace glm;

	MatSkyBox::MatSkyBox() : Material("./resources/shaders/skybox/skybox.vs", "./resources/shaders/skybox/skybox.fs") {}

	void MatSkyBox::Use(Camera* camera, Object* obj, DrawCmd* mesh) {
		glDepthFunc(GL_LEQUAL);
		glUseProgram(shaderId);
		SetMatrix("view", mat4(mat3(camera->GetViewMatrix())));
		SetMatrix("projection", camera->GetProjectionMatrix());
		glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ����Ӧ����Ԫ
		glBindTexture(GL_TEXTURE_CUBE_MAP, Global::skyboxTexture);
		mesh->Draw();
		glDepthFunc(GL_LESS);
	}

	MatSkyBox::~MatSkyBox() {}
}
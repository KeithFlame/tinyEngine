#pragma once
#include "MatSkyBox.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"
#include "Global.h"

namespace TEngine {
	using namespace glm;

	MatSkyBox::MatSkyBox() : Material("./resources/shaders/skybox/skybox.vs", "./resources/shaders/skybox/skybox.fs") {
		mat_view = glGetUniformLocation(shaderId, "view");
		mat_projection = glGetUniformLocation(shaderId, "projection");
	}

	void MatSkyBox::Use(Camera* camera, Object * obj, Mesh* mesh) {
		glDepthFunc(GL_LEQUAL);
		glUseProgram(shaderId);
		SetMatrix(mat4(mat3(camera->GetViewMatrix())), View);
		SetMatrix(camera->GetProjectionMatrix(), Projection);
		glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ����Ӧ����Ԫ
		glBindTexture(GL_TEXTURE_CUBE_MAP, Global::skyboxTexture);
		mesh->Draw();
		glDepthFunc(GL_LESS);
	}

	MatSkyBox::~MatSkyBox() {
	}
}
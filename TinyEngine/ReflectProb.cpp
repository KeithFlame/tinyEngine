#include "ReflectProb.h"
#include "Object.h"
#include "Camera.h"
#include "Standard.h"
#include "Render.h"
#include "Global.h"

namespace TEngine {
	ReflectProb::ReflectProb(Object* obj_) : Component(obj_) {
		texSize = 1024;
		/*����һ����պУ�������6����������ӵ�6��֡����*/
		glGenTextures(1, &cubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap); //�󶨶���cubeMap
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //����cubeMap����
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
		for (GLuint i = 0; i < 6; i++)    //��cubeMapÿһ��������ڴ�
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, texSize, texSize, 0, GL_RGB, GL_FLOAT, NULL);


		glGenFramebuffers(6, framebuffer); //����6��֡����
		for (int i = 0; i < 6; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]); //��֡����

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap, 0); //����
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			unsigned int rbo;            //������Ⱦ������󲢸�����֡�������
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texSize, texSize);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);   //����

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		float fov = 90.0f;

		Object* camObj0 = new Object("right");
		camObj0->SetParent(obj, false);
		camObj0->RotateTo(0, -glm::pi<float>() / 2, glm::pi<float>());
		cameras[0] = camObj0->AddComponent<Camera>();
		cameras[0]->renderTarget = framebuffer[0];
		cameras[0]->Zoom = fov;
		cameras[0]->width = texSize;
		cameras[0]->height = texSize;

		Object* camObj1 = new Object("left");
		camObj1->SetParent(obj, false);
		camObj1->RotateTo(0, glm::pi<float>() / 2, glm::pi<float>());
		cameras[1] = camObj1->AddComponent<Camera>();
		cameras[1]->renderTarget = framebuffer[1];
		cameras[1]->Zoom = fov;
		cameras[1]->width = texSize;
		cameras[1]->height = texSize;

		Object* camObj2 = new Object("top");
		camObj2->RotateTo(glm::pi<float>() / 2, 0, glm::pi<float>());
		camObj2->SetParent(obj, false);
		cameras[2] = camObj2->AddComponent<Camera>();
		cameras[2]->renderTarget = framebuffer[2];
		cameras[2]->Zoom = fov;
		cameras[2]->width = texSize;
		cameras[2]->height = texSize;

		Object* camObj3 = new Object("bottom");
		camObj3->RotateTo(-glm::pi<float>() / 2, 0, glm::pi<float>());
		camObj3->SetParent(obj, false);
		cameras[3] = camObj3->AddComponent<Camera>();
		cameras[3]->renderTarget = framebuffer[3];
		cameras[3]->Zoom = fov;
		cameras[3]->width = texSize;
		cameras[3]->height = texSize;

		Object* camObj4 = new Object("front");
		camObj4->SetParent(obj, false);
		camObj4->RotateTo(0, 0, glm::pi<float>());
		cameras[4] = camObj4->AddComponent<Camera>();
		cameras[4]->renderTarget = framebuffer[4];
		cameras[4]->Zoom = fov;
		cameras[4]->width = texSize;
		cameras[4]->height = texSize;

		Object* camObj5 = new Object("back");
		camObj5->SetParent(obj, false);
		camObj5->RotateTo(0, glm::pi<float>(), glm::pi<float>());
		cameras[5] = camObj5->AddComponent<Camera>();
		cameras[5]->renderTarget = framebuffer[5];
		cameras[5]->Zoom = fov;
		cameras[5]->width = texSize;
		cameras[5]->height = texSize;

		Debug::Log("intial ReflectProb.. \n");

		Render* render = obj->GetComponent<Render>();
		for (unsigned int i = 0; i < render->materials.size(); i++) {
			Standard* mat = dynamic_cast<Standard*>(render->materials.at(i));
			if (mat) {
				mat->diffuseColor = vec3(0.0f);
				mat->specularColor = vec3(0.8f);
				mat->skybox = cubeMap;
			} else {
				Debug::Log("null mat \n");
			}
		}
	}

	ReflectProb::~ReflectProb() {
		glDeleteFramebuffers(6, framebuffer);
		glDeleteTextures(1, &cubeMap);
		for (int i = 0; i < 6; i++) {
			delete cameras[i]->obj;
		}
		Render* render = obj->GetComponent<Render>();
		for (unsigned int i = 0; i < render->materials.size(); i++) {
			Standard* mat = dynamic_cast<Standard*>(render->materials.at(i));
			if (mat) {
				mat->skybox = Global::skyboxTexture;
			}
		}
	}
}
#pragma once
#include "DrawCmdFilter.h"
#include "Engine/Object.h"
#include "Engine/Materials/Material.h"
#include "Render.h"
#include "Engine/DrawCommand/Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


namespace TEngine {
	DrawCmdFilter::DrawCmdFilter() : Component() {}

	void DrawCmdFilter::LoadModel(const aiScene* scene) {
		if (scene == nullptr) return;
		processNode(scene->mRootNode, scene);
	}

	void DrawCmdFilter::processNode(aiNode* node, const aiScene* scene) {
		// ����ڵ����е���������еĻ���
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* ms = processMesh(aimesh, scene);
			drawCmds.push_back(ms);
		}
		// �������������ӽڵ��ظ���һ����
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i], scene);
	}

	Mesh* DrawCmdFilter::processMesh(aiMesh* mesh_, const aiScene* scene) {
		Mesh* mesh = new Mesh();
		mesh->vertices.data = new float[mesh_->mNumVertices * 3];
		mesh->vertices.count = mesh_->mNumVertices * 3;
		mesh->normals.data = new float[mesh_->mNumVertices * 3];
		mesh->normals.count = mesh_->mNumVertices * 3;
		mesh->uv0.data = new float[mesh_->mNumVertices * 2];
		mesh->uv0.count = mesh_->mNumVertices * 2;
		for (unsigned int i = 0; i < mesh_->mNumFaces; i++) { mesh->triangles.count += mesh_->mFaces[i].mNumIndices; }
		mesh->triangles.data = new GLuint[mesh->triangles.count];

		for (unsigned int i = 0; i < mesh_->mNumVertices; i++) {
			mesh->vertices.data[i * 3] = mesh_->mVertices[i].x;
			mesh->vertices.data[i * 3 + 1] = mesh_->mVertices[i].y;
			mesh->vertices.data[i * 3 + 2] = mesh_->mVertices[i].z;

			mesh->normals.data[i * 3] = mesh_->mNormals[i].x;
			mesh->normals.data[i * 3 + 1] = mesh_->mNormals[i].y;
			mesh->normals.data[i * 3 + 2] = mesh_->mNormals[i].z;
			if (mesh_->mTextureCoords[0]) {
				mesh->uv0.data[i * 2] = mesh_->mTextureCoords[0][i].x;
				mesh->uv0.data[i * 2 + 1] = mesh_->mTextureCoords[0][i].y;
			}
		}
		unsigned int tmpIndex = 0;
		for (unsigned int i = 0; i < mesh_->mNumFaces; i++) {
			for (unsigned int j = 0; j < mesh_->mFaces[i].mNumIndices; j++) {
				mesh->triangles.data[tmpIndex++] = mesh_->mFaces[i].mIndices[j];
			}
		}
		mesh->materialIndex = mesh_->mMaterialIndex;
		return mesh;
	}

	void DrawCmdFilter::DrawCmds(Camera* camera) {
		if (render == nullptr) {
			render = obj->GetComponent<Render>();
			return;
		}
		for (GLuint i = 0; i < drawCmds.size(); i++) {
			DrawCmd* cmd = drawCmds[i];
			Material* mat;
			if (render->materials.size() > cmd->materialIndex)
				mat = render->materials[cmd->materialIndex];
			else
				mat = Material::GetDefaultMaterial();
			mat->Use(camera, obj, cmd);
		}
	}

	DrawCmdFilter::~DrawCmdFilter() {
		for (GLuint i = 0; i < drawCmds.size(); i++)
			delete drawCmds[i];
	}
}
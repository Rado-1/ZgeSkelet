/*
ZgeSkelet Library
Copyright (c) 2014 Radovan Cervenka

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
*/

/// The main file used to compile Windows DLL and Android shared library


// Definitions

#define WIN32_LEAN_AND_MEAN
#define CAL3D_API
#define ERROR -1

typedef void* ptr;
#define LOAD_FILE(fnc) coreModel->fnc(std::string(file))
#define LOAD_BUFFER(fnc) coreModel->fnc(buffer)

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else // ANDROID
#define EXPORT "C"
#define glColor4fv(a) glColor4f(a[0], a[1], a[2], a[3])
#endif


// Includes

#include "cal3d/cal3d.h"
#include<string>
#include<memory>

#ifdef _WIN32
#include<GL/glew.h>
#else // ANDROID
#include<GLES/gl.h>
#endif

// Globals

std::string gTextureFilename;

// Core model handling

EXPORT void* zsk_CreateCoreModel(char* name) {
	return new CalCoreModel(std::string(name));
}

EXPORT void zsk_DestroyCoreModel(CalCoreModel* coreModel) {
	delete coreModel;
}

EXPORT bool zsk_LoadSkeleton(CalCoreModel* coreModel, char* file) {
	return LOAD_FILE(loadCoreSkeleton);
}

EXPORT bool zsk_LoadSkeletonBuffer(CalCoreModel* coreModel, void* buffer) {
	return LOAD_BUFFER(loadCoreSkeleton);
}

EXPORT int zsk_LoadAnimation(CalCoreModel* coreModel, char* file) {
	return LOAD_FILE(loadCoreAnimation);
}

EXPORT int zsk_LoadAnimationBuffer(CalCoreModel* coreModel, void* buffer) {
	return LOAD_BUFFER(loadCoreAnimation);
}

EXPORT int zsk_LoadMesh(CalCoreModel* coreModel, char* file) {
	return LOAD_FILE(loadCoreMesh);
}

EXPORT int zsk_LoadMeshBuffer(CalCoreModel* coreModel, void* buffer) {
	return LOAD_BUFFER(loadCoreMesh);
}

EXPORT int zsk_LoadMaterial(CalCoreModel* coreModel, int userId, char* file) {
	int materialId = LOAD_FILE(loadCoreMaterial);

	if(materialId != ERROR)
		coreModel->getCoreMaterial(materialId)->setUserData((Cal::UserData)userId);

	return materialId;
}

EXPORT int zsk_LoadMaterialBuffer(CalCoreModel* coreModel, int userId, void* buffer) {
	int materialId = LOAD_BUFFER(loadCoreMaterial);

	if(materialId != ERROR)
		coreModel->getCoreMaterial(materialId)->setUserData((Cal::UserData)userId);

	return materialId;
}

EXPORT void zsk_CreateCoreMaterialThread(CalCoreModel* coreModel, int threadId) {
	coreModel->createCoreMaterialThread(threadId);
}

EXPORT void zsk_SetCoreMaterialId(CalCoreModel* coreModel,
				int threadId, int setId, int materialId) {
	coreModel->setCoreMaterialId(threadId, setId, materialId);
}

// Model instance handling

EXPORT void* zsk_CreateModelInstance(CalCoreModel* coreModel) {
	return new CalModel(coreModel);
}

EXPORT void zsk_DestroyModelInstance(CalModel* modelInst) {
	delete modelInst;
}

EXPORT int zsk_AttachMesh(CalModel* modelInst, int meshId) {
	return modelInst->attachMesh(meshId);
}

EXPORT int zsk_DetachMesh(CalModel* modelInst, int meshId) {
	return modelInst->detachMesh(meshId);
}

EXPORT void zsk_SetLevelOfDetail(CalModel* modelInst, float lod) {
	modelInst->setLodLevel(lod);
}

EXPORT void zsk_BlendAnimCycle(CalModel* modelInst, int animationId, float weight, float delay) {
	modelInst->getMixer()->blendCycle(animationId, weight, delay);
}

EXPORT void zsk_ClearAnimCycle(CalModel* modelInst, int animationId, float delay) {
	modelInst->getMixer()->clearCycle(animationId, delay);
}

EXPORT void zsk_ExecuteAction(CalModel* modelInst, int animationId, float delayIn,
							float delayOut) {
	modelInst->getMixer()->executeAction(animationId, delayIn, delayOut);
}

EXPORT void zsk_ExecuteActionExt(CalModel* modelInst, int animationId, float delayIn,
							float delayOut, float weightTarget, bool bAutoLock) {
	modelInst->getMixer()->executeAction(animationId, delayIn, delayOut, weightTarget, bAutoLock);
}

EXPORT void zsk_Update(CalModel* modelInst, float deltaTime) {
	modelInst->update(deltaTime);
}

EXPORT void zsk_SetMaterialSet(CalModel* modelInst, int setId) {
	modelInst->setMaterialSet(setId);
}

EXPORT void zsk_SetMeshMaterialSet(CalModel* modelInst, int meshId, int setId) {
	modelInst->getMesh(meshId)->setMaterialSet(setId);
}

// Meshes and textures

EXPORT int zsk_GetMeshCount(CalModel* modelInst) {
	return modelInst->getRenderer()->getMeshCount();
}

EXPORT int zsk_GetSubmeshCount(CalModel* modelInst, int meshId) {
	return modelInst->getRenderer()->getSubmeshCount(meshId);
}

EXPORT const char* zsk_GetSubmeshTexture(CalModel* modelInst, int meshId, int submeshId) {
	CalCoreMaterial* material = modelInst->getCoreModel()->getCoreMaterial(
		modelInst->getMesh(meshId)->getSubmesh(submeshId)->getCoreMaterialId());

	gTextureFilename = material->getMapCount() > 0 ? material->getMapFilename(0) : "";

	return gTextureFilename.c_str();
}

EXPORT int zsk_GetSubmeshUserMaterialId(CalModel* modelInst, int meshId, int submeshId) {
	CalCoreMaterial* material = modelInst->getCoreModel()->getCoreMaterial(
		modelInst->getMesh(meshId)->getSubmesh(submeshId)->getCoreMaterialId());

	return (int)material->getUserData();
}

// Rendering

EXPORT void zsk_RenderModelInstance(CalModel* modelInst, bool bSetColor, bool bRenderTexture) {

	// get the renderer of the model
	CalRenderer* renderer = modelInst->getRenderer();

	// begin the rendering loop
	if(!renderer->beginRendering()) return;

	// we will use vertex arrays, so enable them
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// get the number of meshes
	int meshCount = renderer->getMeshCount();

	// render all meshes of the model
	for(int meshId = 0; meshId < meshCount; meshId++) {

		// get the number of submeshes
		int submeshCount = renderer->getSubmeshCount(meshId);

		// render all submeshes of the mesh
		for(int submeshId = 0; submeshId < submeshCount; submeshId++) {

			// select mesh and submesh for further data access
			if(renderer->selectMeshSubmesh(meshId, submeshId)) {

				// set the color
				if(bSetColor) {
					unsigned char meshColor[4];
					GLfloat materialColor[4];

					// set the material ambient color
					renderer->getAmbientColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;
					materialColor[1] = meshColor[1] / 255.0f;
					materialColor[2] = meshColor[2] / 255.0f;
					materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
					glMaterialfv(GL_BACK, GL_AMBIENT, materialColor);

					// set the material diffuse color
					renderer->getDiffuseColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;
					materialColor[1] = meshColor[1] / 255.0f;
					materialColor[2] = meshColor[2] / 255.0f;
					materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
					glMaterialfv(GL_BACK, GL_DIFFUSE, materialColor);

					glColor4fv(materialColor);

					// set the material specular color
					renderer->getSpecularColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;
					materialColor[1] = meshColor[1] / 255.0f;
					materialColor[2] = meshColor[2] / 255.0f;
					materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
					glMaterialfv(GL_BACK, GL_SPECULAR, materialColor);
				}

				// get the transformed vertices of the submesh
				static float meshVertices[30000][3];
				renderer->getVertices(&meshVertices[0][0]);

				// get the transformed normals of the submesh
				static float meshNormals[30000][3];
				renderer->getNormals(&meshNormals[0][0]);

				// get the faces of the submesh
				static CalIndex meshFaces[50000][3];
				int faceCount = renderer->getFaces(&meshFaces[0][0]);

				// set the vertex and normal buffers
				glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
				glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

				if(bRenderTexture && renderer->getMapCount() > 0) {
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);

					// get the texture coordinates of the submesh
					static float meshTextureCoordinates[30000][2];
					int textureCoordinateCount = renderer->
							getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

					if(textureCoordinateCount > 0)
						// set the texture buffer
						glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
				}

				// draw the submesh

				#ifdef CAL_16BIT_INDICES
				glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
				#else
				glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);
				#endif

				if(bRenderTexture && renderer->getMapCount() > 0)
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
	}

	// clear vertex array state
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// end the rendering
	renderer->endRendering();
}

EXPORT void zsk_RenderSubmesh(CalModel* modelInst,
				int meshId, int submeshId, bool bSetColor, bool bRenderTexture) {

	// get the renderer of the model
	CalRenderer* renderer = modelInst->getRenderer();

	// begin the rendering loop
	if(!renderer->beginRendering()) return;

	// select mesh and submesh for further data access
	if(renderer->selectMeshSubmesh(meshId, submeshId)) {

		// we will use vertex arrays, so enable them
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		// set the color
		if(bSetColor) {
			unsigned char meshColor[4];
			GLfloat materialColor[4];

			// set the material ambient color
			renderer->getAmbientColor(&meshColor[0]);
			materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
			glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
			glMaterialfv(GL_BACK, GL_AMBIENT, materialColor);

			// set the material diffuse color
			renderer->getDiffuseColor(&meshColor[0]);
			materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
			glMaterialfv(GL_BACK, GL_DIFFUSE, materialColor);

			glColor4fv(materialColor);

			// set the material specular color
			renderer->getSpecularColor(&meshColor[0]);
			materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
			glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
			glMaterialfv(GL_BACK, GL_SPECULAR, materialColor);
		}

		// get the transformed vertices of the submesh
		static float meshVertices[30000][3];
		renderer->getVertices(&meshVertices[0][0]);

		// get the transformed normals of the submesh
		static float meshNormals[30000][3];
		renderer->getNormals(&meshNormals[0][0]);

		// get the faces of the submesh
		static CalIndex meshFaces[50000][3];
		int faceCount;
		faceCount = renderer->getFaces(&meshFaces[0][0]);

		// set the vertex and normal buffers
		glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
		glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

		if(bRenderTexture && renderer->getMapCount() > 0) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			// get the texture coordinates of the submesh
			static float meshTextureCoordinates[30000][2];
			int textureCoordinateCount = renderer->
					getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

			if(textureCoordinateCount > 0)
				// set the texture buffer
				glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
		}

		// draw the submesh

		#ifdef CAL_16BIT_INDICES
		glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
		#else
		glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);
		#endif

		// clear vertex array state
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		if(bRenderTexture && renderer->getMapCount() > 0)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


	// end the rendering
	renderer->endRendering();
}

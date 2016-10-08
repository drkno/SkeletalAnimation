#include <iostream>
#include <GL/freeglut.h>
#include "Stage.h"
#include <vector>

using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"

const aiScene* person;
const aiScene* thing;
GLuint scene_list = 0;
aiVector3D scene_min, scene_max, scene_center;
float secsPerTick = NULL;
unsigned int tick = 0;
Stage* stage = new Stage();


#define HALF_WIDTH 300
#define HALF_DEPTH 200


vector<aiVector3D> vertices = vector<aiVector3D>();

vector<aiVector3D> normals = vector<aiVector3D>();

const aiScene* loadModel(const char* fileName, bool isAnimation)
{
	auto scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr) exit(1);
	if (isAnimation) {
		secsPerTick = scene->mAnimations[0]->mTicksPerSecond == 0.0 ? 0.02 : 1.0 / scene->mAnimations[0]->mTicksPerSecond;
		get_bounding_box(scene, &scene_min, &scene_max);
	}
	for (auto i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		for (auto j = 0; j < mesh->mNumVertices; j++) {
			vertices.push_back(mesh->mVertices[j]);
			normals.push_back(mesh->mNormals[j]);
		}
	}
	return scene;
}

void updateVerts(const aiScene* scene)
{
	auto offset = 0;

	for (auto i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		for (auto j = 0; j < mesh->mNumBones; j++)
		{
			auto bone = mesh->mBones[j];
			auto offsetMatrix = bone->mOffsetMatrix;
			auto currentNode = scene->mRootNode->FindNode(bone->mName);
			while (currentNode != nullptr)
			{
				offsetMatrix = currentNode->mTransformation * offsetMatrix;
				currentNode = currentNode->mParent;

			}
			auto transposeMatrix = offsetMatrix;
			transposeMatrix.Transpose();
			transposeMatrix.Inverse();

			for (auto k = 0; k < bone->mNumWeights; k++)
			{
				auto vertex = bone->mWeights[k].mVertexId;
				mesh->mVertices[vertex] = offsetMatrix * vertices[vertex + offset];
				mesh->mNormals[vertex] = transposeMatrix * normals[vertex + offset];
			}
		}
		offset += mesh->mNumVertices;
	}
}

void initialise()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	person = loadModel("Model_Files/dwarf.x", true);
	//thing = loadModel("Model_Files/Scene/Street_environment_V01.obj", false);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1.0, 1000.0);
}

void update(int value)
{
	auto anim = person->mAnimations[0];
	for (int i = 0; i < anim->mNumChannels; i++)
	{
		auto chnl = anim->mChannels[i];
		auto posn = chnl->mPositionKeys[tick % chnl->mNumPositionKeys].mValue;
		auto rotn = chnl->mRotationKeys[tick % chnl->mNumRotationKeys].mValue;
		auto matPos = aiMatrix4x4();
		matPos.Translation(posn, matPos);
		auto matRotn3 = rotn.GetMatrix();
		auto matRot = aiMatrix4x4(matRotn3);
		auto matprod = matPos * matRot;
		auto node = person->mRootNode->FindNode(chnl->mNodeName);
		node->mTransformation = matprod;
	}
	tick = (tick + 1) % static_cast<int>(anim->mDuration);
	glutPostRedisplay();
	glutTimerFunc(secsPerTick * 1000, update, 0);
}

void render(const aiScene* sc, const aiNode* nd)
{
	aiMatrix4x4 m = nd->mTransformation;
	aiMesh* mesh;
	aiFace* face;

	aiTransposeMatrix4(&m); //Convert to column-major order
	glPushMatrix();
	glMultMatrixf(reinterpret_cast<float*>(&m)); //Multiply by the transformation matrix for this node

							   // Draw all meshes assigned to this node
	for (int n = 0; n < nd->mNumMeshes; n++)
	{
		mesh = sc->mMeshes[nd->mMeshes[n]];
		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
		mesh->HasNormals() ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
		mesh->HasVertexColors(0) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

		//Get the polygons from each mesh and draw them
		for (int k = 0; k < mesh->mNumFaces; k++)
		{
			face = &mesh->mFaces[k];
			GLenum face_mode;

			switch (face->mNumIndices)
			{
			case 1: face_mode = GL_POINTS;
				break;
			case 2: face_mode = GL_LINES;
				break;
			case 3: face_mode = GL_TRIANGLES;
				break;
			default: face_mode = GL_POLYGON;
				break;
			}

			glBegin(face_mode);

			for (int i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				if (mesh->HasVertexColors(0))
				{
					glEnable(GL_COLOR_MATERIAL);
					glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
					glColor4fv(reinterpret_cast<GLfloat*>(&mesh->mColors[0][index]));
				}
				else
					glDisable(GL_COLOR_MATERIAL);
				if (mesh->HasNormals())
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}
	}

	// Draw all children
	for (int i = 0; i < nd->mNumChildren; i++)
		render(sc, nd->mChildren[i]);

	glPopMatrix();
}

void display()
{
	float pos[4] = {50, 50, 50, 1};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, 3, 0, 0, -5, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// scale the whole asset to fit into our view frustum 
	float tmp = scene_max.x - scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	tmp = 1.f / tmp;
	glScalef(tmp, tmp, tmp);

	// center the model
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

	
	

//	stage->display();

	updateVerts(person);
	render(person, person->mRootNode);
	glPushMatrix();
	glScalef(25, 25, 25);
	//render(thing, thing->mRootNode);
	glPopMatrix();

	auto verticies = person->mMeshes[0]->mVertices[0];
	cout << verticies.x << " " << verticies.y << " " << verticies.z << endl;
//	verticies *= tmp;
	gluLookAt(0, 0, 15, verticies.x, verticies.y, verticies.z, 0, 1, 0);

	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Assimp Test");
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	initialise();
	glutDisplayFunc(display);
	glutTimerFunc(secsPerTick * 1000, update, 0);
	glutMainLoop();

	aiReleaseImport(person);
	//aiReleaseImport(thing);
	delete stage;
}

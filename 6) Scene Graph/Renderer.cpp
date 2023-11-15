 #include "Renderer.h"
#include"../nclgl/MeshMaterial.h"
 Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	camera = new Camera();
	/*shader=new Shader("PerPixelVertex.glsl","PerPixelFragment.glsl");*/
	shader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
		if (!shader -> LoadSuccess()) {
		 return;
		
	}
	
	 projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
	 (float)width/(float)height, 45.0f);
	
	 camera -> SetPosition(Vector3(0,30,175));
	
	 root = new SceneNode();
	 root -> AddChild(new CubeRobot(cube));
	
	 glEnable(GL_DEPTH_TEST);
	 init = true;

	 shipMesh = Mesh::LoadFromMeshFile("F3_Red.msh");
	 shipMaterial = new MeshMaterial("F3_Red.mat");
	 for (int i = 0; i < shipMesh->GetSubMeshCount(); ++i) {
		 const MeshMaterialEntry* matEntry =
			 shipMaterial->GetMaterialForLayer(i);

		 const string* filename = nullptr;
		 matEntry->GetEntry("Diffuse", &filename);
		 string path = TEXTUREDIR + *filename;
		 GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
			 SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		 matTextures.emplace_back(texID);
	 }
	 
	
}

 Renderer ::~Renderer(void) {
	 delete root;
	 delete shader;
	 delete camera;
	 delete cube;

	 delete shipMesh;	 delete shipMaterial;} void Renderer::UpdateScene(float dt) {
	camera -> UpdateCamera(dt);
	viewMatrix = camera -> BuildViewMatrix();
	root -> Update(dt);
	
}

 void Renderer::RenderScene() {
	 glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	 BindShader(shader);
	 UpdateShaderMatrices();
	
	 glUniform1i(glGetUniformLocation(shader -> GetProgram(),
		 "diffuseTex"), 0);
	
	 for (int i = 0; i < shipMesh->GetSubMeshCount(); ++i) {
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		 shipMesh->DrawSubMesh(i);
		 
	 }

	/* glUniform1i(glGetUniformLocation(shader->GetProgram(),
		 "diffuseTex"), 1);
	DrawNode(root);*/
}
 void Renderer::DrawNode(SceneNode * n) {
	 if (n -> GetMesh()) {
		 Matrix4 model = n -> GetWorldTransform() *
		 Matrix4::Scale(n -> GetModelScale());
		 glUniformMatrix4fv(
		 glGetUniformLocation(shader -> GetProgram(),"modelMatrix"), 1, false, model.values);
		
		 glUniform4fv(glGetUniformLocation(shader -> GetProgram(),"nodeColour"), 1, (float*)&n -> GetColour());
		
		 glUniform1i(glGetUniformLocation(shader -> GetProgram(),"useTexture"), 0); // Next tutorial ;)
				 n -> Draw(*this);
				
	}
	 
	  for (vector < SceneNode* >::const_iterator
	  i = n -> GetChildIteratorStart();
	  i != n -> GetChildIteratorEnd(); ++i) {
	  DrawNode(*i);
	 
	}
	}
#pragma once
#include"../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"

 class Renderer : public OGLRenderer {
 public:
	 virtual void UpdateScene(float dt);
	Renderer(Window & parent);
	virtual ~Renderer(void);
	
	virtual void RenderScene();
	
	void SwitchToPerspective();
	 void SwitchToOrthographic();
	
	 inline void SetScale(float s) { scale = s; }
	 inline void SetRotation(float r) { rotation = r; }
	 inline void SetPosition(Vector3 p) { position = p; }
	
 protected:
	 Camera* camera = new Camera;
	 Mesh * triangle;
	 Shader * matrixShader;
	 float scale;
	 float rotation;
	 Vector3 position;
	
};

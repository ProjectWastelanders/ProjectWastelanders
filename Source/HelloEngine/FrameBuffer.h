#pragma once
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void BindShadowBuffer();
	void BindShadowTexture();

	void SetDimensions(int width, int height);
	void SetBufferInfo();

	void SetDepthMapInfo();

	uint GetTexture() { return textureColorbuffer; }
	uint GetDepthTexture() { return depthMap; }

private:

	void RegenerateBuffer();
private:
	uint FBO = 0; // Frame Buffer Object (contains the result of the entire frame texture)
	uint textureColorbuffer = 0;
	uint RBO = 0;

	int width = 1280;
	int height = 720;

	/*Depth map for shadow mapping*/
	uint depthMapFBO = 0;
	uint depthMap = 0;
	const uint SHADOW_WIDTH = 1024;
	const uint SHADOW_HEIGHT = 1024;

	friend class CameraObject;
};


#pragma once

#include <boost/shared_ptr.hpp>

namespace graphic
{

class Shader;
class MeshShader;
class VertexBuffer;
class IndexBuffer;
class Texture;
class Material;
struct Animation;
class Camera;
class DrawCall;
class Font;
class Scheduler;
class Renderer;
class D3D9Renderer;

typedef boost::shared_ptr<Shader> ShaderPtr;
typedef boost::shared_ptr<MeshShader> MeshShaderPtr;
typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef boost::shared_ptr<Texture> TexturePtr;
typedef boost::shared_ptr<Material> MaterialPtr;
typedef boost::shared_ptr<Animation> AnimationPtr;
typedef boost::shared_ptr<Camera> CameraPtr;
typedef boost::shared_ptr<DrawCall> DrawCallPtr;
typedef boost::shared_ptr<Font> FontPtr;
typedef boost::shared_ptr<Scheduler> SchedulerPtr;
typedef boost::shared_ptr<Renderer> RendererPtr;
typedef boost::shared_ptr<D3D9Renderer> D3D9RendererPtr;

}


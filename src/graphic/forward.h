#pragma once

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace graphic
{

class Shader;
class UberShader;
class MeshShader;
class VertexBuffer;
class IndexBuffer;
class Texture;
class Material;
struct Animation;
class Camera;
class DrawCall;
class UberDrawCall;
class Font;
class Scheduler;
class Renderer;
class D3D9Renderer;

typedef boost::shared_ptr<Shader> ShaderPtr;
typedef boost::shared_ptr<UberShader> UberShaderPtr;
typedef boost::shared_ptr<MeshShader> MeshShaderPtr;
typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef boost::shared_ptr<Texture> TexturePtr;
typedef boost::shared_ptr<Material> MaterialPtr;
typedef boost::shared_ptr<Animation> AnimationPtr;
typedef boost::shared_ptr<Camera> CameraPtr;
typedef boost::shared_ptr<DrawCall> DrawCallPtr;
typedef boost::shared_ptr<UberDrawCall> UberDrawCallPtr;
typedef boost::shared_ptr<Font> FontPtr;
typedef boost::shared_ptr<Scheduler> SchedulerPtr;
typedef boost::weak_ptr<Renderer> RendererWeakPtr;
typedef boost::shared_ptr<Renderer> RendererPtr;
typedef boost::weak_ptr<D3D9Renderer> D3D9RendererWeakPtr;
typedef boost::shared_ptr<D3D9Renderer> D3D9RendererPtr;

}


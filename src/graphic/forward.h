#pragma once

#include <boost/shared_ptr.hpp>

namespace graphic
{

class Shader;
class VertexBuffer;
class IndexBuffer;
class Texture;
class Material;
struct Animation;
class Camera;
class DrawCall;
class Scheduler;
class Renderer;

typedef boost::shared_ptr<Shader> ShaderPtr;
typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef boost::shared_ptr<Texture> TexturePtr;
typedef boost::shared_ptr<Material> MaterialPtr;
typedef boost::shared_ptr<Animation> AnimationPtr;
typedef boost::shared_ptr<Camera> CameraPtr;
typedef boost::shared_ptr<DrawCall> DrawCallPtr;
typedef boost::shared_ptr<Scheduler> SchedulerPtr;
typedef boost::shared_ptr<Renderer> RendererPtr;

}


#include "display.h"

using namespace motorcar;



Display::Display(OpenGLContext *glContext, glm::vec2 displayDimensions, PhysicalNode *parent, const glm::mat4 &transform)
    :PhysicalNode(parent, transform)
    ,m_glContext(glContext)
    ,m_dimensions(displayDimensions)

{
    m_glContext->makeCurrent();

    glm::ivec2 res = this->size();


    glGenFramebuffers(1, &m_scratchFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_scratchFrameBuffer);

    glGenRenderbuffers(1, &m_scratchColorBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_scratchColorBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, res.x, res.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_scratchColorBuffer);


    glGenRenderbuffers(1, &m_scratchDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_scratchDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, res.x, res.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_scratchDepthBuffer);

    std::cout << "Checking Scratch Framebuffer:" << std::endl;
    switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
            case(GL_FRAMEBUFFER_COMPLETE):
                std::cout << "Framebuffer Complete" << std::endl;
                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete" << std::endl;
                break;
            case(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT):
                std::cout << "Framebuffer Attachment Incomplete/Missing" << std::endl;
                break;
            case(GL_FRAMEBUFFER_UNSUPPORTED):
                std::cout << "Framebuffer Unsupported" << std::endl;
                break;
            default:
                std::cout << "Framebuffer is Incomplete for Unknown Reasons" << std::endl;
                break;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

Display::~Display()

{
}

void Display::prepareForDraw()
{
   //std::cout << "display size: " << this->resolution().x << ", " << this->resolution().y <<std::endl;
//    Geometry::printMatrix(worldTransform());

//    for(ViewPoint *viewpoint : viewpoints()){
//        viewpoint->updateViewMatrix();
//    }
    glContext()->makeCurrent();
    glClearColor(.7f, .85f, 1.f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc (GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
}



void Display::addViewpoint(ViewPoint *v)
{
    m_viewpoints.push_back(v);
}


std::vector<ViewPoint *> Display::viewpoints() const
{
    return m_viewpoints;
}

glm::vec2 Display::dimensions() const
{
    return m_dimensions;
}





Geometry::Ray Display::worldRayAtDisplayPosition(glm::vec2 pixel)
{
    ViewPoint *cam = viewpoints().front();
//    glm::vec3 cameraCenterOfFocus(cam->worldTransform() * cam->centerOfFocus());
//    glm::vec3 pixelPos = worldPositionAtDisplayPosition(pixel);
//    Geometry::printVector(pixelPos);
//    std::cout << std::endl;

//    Geometry::Ray ray(cameraCenterOfFocus, glm::normalize(pixelPos - cameraCenterOfFocus));
//    //ray.print();
//    return ray;
    return cam->worldRayAtDisplayPosition(pixel.x, pixel.y);
}

glm::vec3 Display::worldPositionAtDisplayPosition(glm::vec2 pixel)
{
//    Geometry::printVector(glm::vec3(pixel, 0));
//    Geometry::printVector(glm::vec3(resolution(), 0));
//    Geometry::printVector(glm::vec3(size(), 0));


    return glm::vec3(worldTransform() * glm::vec4(((pixel / glm::vec2(size())) - glm::vec2(0.5)) * dimensions(), 0 , 1));
}

glm::ivec2 Display::size()
{
    return glContext()->defaultFramebufferSize();
}







OpenGLContext *Display::glContext() const
{
    return m_glContext;
}

void Display::setGlContext(OpenGLContext *glContext)
{
    m_glContext = glContext;
}


GLuint Display::scratchFrameBuffer() const
{
    return m_scratchFrameBuffer;
}




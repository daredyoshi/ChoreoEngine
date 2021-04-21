#include "OpenGLShader.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <unordered_map>

static GLenum stringToShaderType(const std::string& type){
   if (type == "vertex") return GL_VERTEX_SHADER;
   if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

   CE_CORE_ASSERT(false, "Unknown shader type!");
   return 0;
}

ChoreoEngine::OpenGLShader::OpenGLShader(const std::string& path){
    CE_PROFILE_FUNCTION();  
    auto shaderSources = preProcess(path);
    compile(shaderSources);


    // figure out the name from the filepath
    // assets/shaders/Texture.shader.glsl
    auto lastSlash = path.find_last_of("/\\");
    // what if there are no slashes in filename?
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash+1;
    auto lastDot = path.rfind(".");
    // filenames do not need extentions. 
    // Get the count of the substring
    auto count =lastDot == std::string::npos ? path.size() - lastSlash : lastSlash - lastSlash;  
    m_name = path.substr(lastSlash, count);
}

ChoreoEngine::OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    :m_name{name}
{
    CE_PROFILE_FUNCTION();  
    std::unordered_map<GLenum, std::string> shaderSources;
    shaderSources[GL_VERTEX_SHADER] = vertexSrc;
    shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

    compile(shaderSources);
}

std::unordered_map<GLenum, std::string> ChoreoEngine::OpenGLShader::preProcess(const std::string& path)
{
    CE_PROFILE_FUNCTION();  
    std::string source = readFile(path);
    std::unordered_map<GLenum, std::string> shaderSources;

    // find the first occurance of type
    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    // from taht position gather all the src into the map of the corresponding type
    while(pos!= std::string::npos){
        size_t eol = source.find_first_of("\r\n", pos);
        CE_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
        // there must be a space beween #type typename
        size_t begin = pos + typeTokenLength + 1;
        // get the type name
        std::string type = source.substr(begin, eol - begin);

        // add more here later when we support them
        GLenum shaderType = stringToShaderType(type);
        CE_CORE_ASSERT(shaderType!=0, "Invalid shader type specification");

        // find the next line and type token 
        size_t nextLinePos = source.find_first_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        // dump everything between them into output
        shaderSources[shaderType] = source.substr(nextLinePos, 
                pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }


	return shaderSources;
}


std::string ChoreoEngine::OpenGLShader::readFile(const std::string& path){

    CE_PROFILE_FUNCTION();  
    // this should probably be refactored to a per-platform filesystem
    std::string result;
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if(in){
        // figure out how long to make the resut string
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        // go back to beginning and read in result string
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    } 
    else{
        CE_CORE_ERROR("Could not load file {0}", path);
    }
    return result;
}

void ChoreoEngine::OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources){
    CE_PROFILE_FUNCTION();  
    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program =glCreateProgram(); 
    CE_CORE_ASSERT(shaderSources.size() <=5, "Maximum no. of shaders is 5! You provided {0}", shaderSources.size());
    // 5 is the maximum no. of shaers ever
    std::array<GLenum, 5> glShaderIds{};

    unsigned int glShaderIDIdx{0};
    for (auto& kv : shaderSources){
        GLenum type = kv.first;
        const std::string& source = kv.second;

        // Create an empty shader handle
        GLuint shader = glCreateShader(type);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar *sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        // Compile the vertex shader
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(shader);

            // Use the infoLog as you see fit.
            CE_CORE_ERROR("{0}", infoLog.data());
            CE_CORE_ASSERT(false, "Shader Compilation failure");
            break;
        }
        glAttachShader(program, shader);
        glShaderIds[glShaderIDIdx++] = shader;
    }

    // Link our m_rendererId
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_rendererId, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the m_rendererId anymore.
        glDeleteProgram(m_rendererId);
        // Don't leak shaders either.
        for(auto id : glShaderIds){
            glDeleteShader(id);
        }

        // Use the infoLog as you see fit.
        CE_CORE_ERROR("{0}", infoLog.data());
        CE_CORE_ERROR("Linking Shader failure");
        
        // In this simple m_rendererId, we'll just leave
        return;
    }

    // to prevent geting a valid program withou shaders
    m_rendererId= program;

    // Always detach shaders after a successful link.
    for(auto id : glShaderIds){
        glDeleteShader(id);
    }
}

ChoreoEngine::OpenGLShader::~OpenGLShader()
{
    CE_PROFILE_FUNCTION();  
    glDeleteProgram(m_rendererId);
}

void ChoreoEngine::OpenGLShader::bind() const
{
    CE_PROFILE_FUNCTION();  
    glUseProgram(m_rendererId);
}
void ChoreoEngine::OpenGLShader::unbind() const
{
    CE_PROFILE_FUNCTION();  
    glUseProgram(0); 
}
void ChoreoEngine::OpenGLShader::setFloat(const std::string& name, const float val)const 
{
    CE_PROFILE_FUNCTION();  
    uploadUniformFloat(name, val);
}

void ChoreoEngine::OpenGLShader::setFloat3(const std::string& name, const glm::vec3& val)const 
{
    CE_PROFILE_FUNCTION();  
    uploadUniformFloat3(name, val);
}

void ChoreoEngine::OpenGLShader::setFloat4(const std::string& name, const glm::vec4& val)const
{
    CE_PROFILE_FUNCTION();  
    uploadUniformFloat4(name, val);
}
void ChoreoEngine::OpenGLShader::setMat4(const std::string& name, const glm::mat4& val) const 
{
    CE_PROFILE_FUNCTION();  
    uploadUniformMat4(name, val);
}

void ChoreoEngine::OpenGLShader::setInt(const std::string& name, const int val) const{
    CE_PROFILE_FUNCTION();  
    uploadUniformInt(name, val);
} 
void ChoreoEngine::OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& val) const
{
    GLint location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(val));
}

void ChoreoEngine::OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& val) const
{
    GLint location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
}


void ChoreoEngine::OpenGLShader::uploadUniformFloat(const std::string& name, const float val) const
{
    GLint location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1f(location, val);
}

void ChoreoEngine::OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& val) const
{
    GLint location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform2fv(location, 1, glm::value_ptr(val));
}

void ChoreoEngine::OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& val) const
{
    GLint location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(val));
}

void ChoreoEngine::OpenGLShader::uploadUniformInt(const std::string& name, const int val) const
{
    GLint location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1i(location, val);
}

void ChoreoEngine::OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& val) const
{
    GLint location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(val));
}


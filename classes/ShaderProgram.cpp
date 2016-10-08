#include "ShaderProgram.h"

//static void checkShaderError(GLuint pShader, GLuint pFlag, bool pIsProgramm, const std::string& pErrorMessage);




ShaderProgram::ShaderProgram(){
}

ShaderProgram::ShaderProgram(const std::string& pFilename){
    
    /*std::string vertexShader = pFilename + ".vs";
     std::string fragmentShader = pFilename + ".fs";
     
     
     
     if(this->load(vertexShader.c_str(), fragmentShader.c_str())){
     
     }else{
     
     }*/
}

GLuint ShaderProgram::createShader(const std::string& text, GLenum shaderType){
    
    if(shaderType == GL_VERTEX_SHADER){
        this->loadVertexShader(text.c_str());
        
    }else if(shaderType == GL_FRAGMENT_SHADER){
        this->loadFragmentShader(text.c_str());
        
        
        
    }
    
    return 0;
}

bool ShaderProgram::load(const char* pVertexShader, const char* pFragmentShader){
    
    bool vertexShaderIsSet = false;
    bool fragmentShaderIsSet = false;
    
    if(strcmp(pVertexShader, "NOSHADER")==0 && strcmp(pFragmentShader, "NOSHADER")==0){
        return false;
    }
    
    if(strcmp(pVertexShader, "NOSHADER")!=0){
        
        if(!loadVertexShader(pVertexShader)){
            return false;
        }
        
        
        vertexShaderIsSet = true;
        
    }else{
        std::cout << "Kein Vertexshader gesetzt." << std::endl;
    }
    
    
    if(strcmp(pFragmentShader, "NOSHADER")!=0){
        
        
        if(!loadFragmentShader(pFragmentShader)){
            return false;
        }
        
        
        fragmentShaderIsSet = true;
    }else{
        std::cout << "Kein Fragmentshader gesetzt." << std::endl;
    }
    
    
    std::string compileErrors;
    
    if(compile(&compileErrors)){
        this->m_ShaderProgram = glCreateProgram();
        
        if(vertexShaderIsSet)
            glAttachShader(this->m_ShaderProgram, this->m_VertexShader);
        
        if(fragmentShaderIsSet)
            glAttachShader(this->m_ShaderProgram, this->m_FragmentShader);
        
        glLinkProgram(this->m_ShaderProgram);
    }else{
        std::cout << "---- Compilation Errors! ----" << std::endl;
        std::cout << compileErrors << std::endl;
        
        return false;
    }
    
    return true;
}

bool ShaderProgram::loadVertexShader(const char * VertexShader){
    
    this->m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    const char* vertexShaderSrc = readFile(VertexShader);
    
    glShaderSource(this->m_VertexShader, 1, &vertexShaderSrc, NULL);
    
    return true;
}

bool ShaderProgram::loadFragmentShader(const char * FragmentShader){
    
    this->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    
    const char* fragmentShaderSrc = readFile(FragmentShader);
    
    glShaderSource(this->m_FragmentShader, 1, &fragmentShaderSrc, NULL);
    
    return true;
}

bool ShaderProgram::compile(std::string * CompileErrors){
    bool flag1 = compileShader(this->m_VertexShader, CompileErrors);
    bool flag2 = compileShader(this->m_FragmentShader, CompileErrors);
    return  flag1 && flag2;
}


bool ShaderProgram::compileShader(GLuint shader, std::string* CompileErrors){
    
    GLint isCompiled = 0;
    
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    
    if(isCompiled == GL_FALSE){
        
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::cout << "MaxLength = " << maxLength << std::endl;
        
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        
        for(int i = 0; i < errorLog.size(); i++){
            CompileErrors->push_back((char)errorLog[i]);
        }
        
        std::cout << "CompileError Log" << std::endl;
        std::cout << *CompileErrors << std::endl;
        
        glDeleteShader(shader);
        
        return false;
        
    }
    
    return true;
}

GLint ShaderProgram::getParameterID(const char * ParameterName) const{
    
    return glGetUniformLocation(this->m_ShaderProgram, ParameterName);
}

void ShaderProgram::setParameter(GLint ID, float Param){
    glUniform1f(ID, Param);
}

void ShaderProgram::setParameter(GLint ID, int Param){
    
    glUniform1f(ID, Param);
}

void ShaderProgram::setParameter(GLint ID, const Vector & Param){
    
    glUniform3f(ID, Param.getX(), Param.getY(), Param.getZ());
}

void ShaderProgram::setParameter(GLint ID, const Color & Param){
    
    glUniform3f(ID, Param.R, Param.G, Param.B);
}

void ShaderProgram::setParameter(GLint ID, const Matrix & Param){
    glUniformMatrix4fv(ID, 16, false, Param.m);
}

void ShaderProgram::activate() const{
    glUseProgram(this->m_ShaderProgram);
}

void ShaderProgram::deactivate() const{
    glUseProgram(0);
}


ShaderProgram::~ShaderProgram(){
    glDeleteProgram(m_ShaderProgram);
}
/*
 static void checkShaderError(GLuint pShader, GLuint pFlag, bool pIsProgramm, const std::string& pErrorMessage){
 
 GLint success = 0;
 GLchar error[1024] = { 0 };
 
 if(pIsProgramm){
 glGetProgramiv(pShader, pFlag, &success);
 }else{
 glGetShaderiv(pShader, pFlag, &success);
 }
 
 
 if(success == GL_FALSE){
 
 if(pIsProgramm){
 glGetProgramInfoLog(pShader, sizeof(error), NULL, error);
 }else{
 
 glGetShaderInfoLog(pShader, sizeof(error), NULL, error);
 }
 
 std::cerr << "Error: '" << error << "'" << std::endl;
 }
 
 }*/

const char* ShaderProgram::readFile(const char *fileName) {
    
    std::ifstream file(fileName);
    if(!file.is_open()){
        std::cout << "Fehler beim Einlesen der Datei." << std::endl;
        return 0;
    }
    std::string line;
    std::string result;
    
    while (std::getline(file, line)) {
        result += line + "\n";
    }
    //std::cout << "FILE " << line <<std::endl;
    char *result1 = strdup(result.c_str());
    return result1;
}

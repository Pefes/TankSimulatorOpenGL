#include "ObjectLoader.h"

ObjectLoader::ObjectLoader()
{
    //ctor
}

ObjectLoader::~ObjectLoader()
{
    //dtor
}

ObjectLoader::loadOBJ(
    const char * path,
    std::vector < glm::vec4 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec4 > & out_normals,
    std::vector < glm::vec4 > & out_colors
) {
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec4 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec4 > temp_normals;

    printf("Loading object...\n");

    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Could not open the file!\n");
        return false;
    }

    while( 1 ){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec4 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            vertex.w = 1.0;
            temp_vertices.push_back(vertex);
        } else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        } else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec4 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            normal.w = 0;
            temp_normals.push_back(normal);
        } else if ( strcmp( lineHeader, "f" ) == 0 ){

            while(1) {
                unsigned int vertexIndex, uvIndex, normalIndex;
                int matches = fscanf(file, "%d/%d/%d", &vertexIndex, &uvIndex, &normalIndex);

                if (matches != 3)
                int matches = fscanf(file, "%d//%d//%d", &vertexIndex, &uvIndex, &normalIndex);

                if (matches != 3){
                    if(!fscanf(file, "\n"))
                        break;
                    else
                    {
                        printf("Error, could not load the object!\n");
                        return false;
                    }
                }

                vertexIndices.push_back(vertexIndex);
                uvIndices    .push_back(uvIndex);
                normalIndices.push_back(normalIndex);
            }
        }
    }

    for( unsigned int i=0; i<vertexIndices.size(); i++ )
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec4 vertex = temp_vertices[ vertexIndex-1 ];
        out_vertices.push_back(vertex);
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        out_uvs.push_back(uv);
        unsigned int normalIndex = normalIndices[i];
        glm::vec4 normal = temp_normals[ normalIndex-1 ];
        out_normals.push_back(normal);

        // to jest tutaj tylko po to, ¿eby jebn¹æ kolor bo nie umiem w tesktury :/
        glm::vec4 color;
        color.x = 0.8; color.y = 0.3; color.z = 0.5; color.w = 1;
        out_colors.push_back(color);
    }

}

GLuint ObjectLoader::loadTexture(char* filename) {

    GLuint tex;
    glActiveTexture(GL_TEXTURE0);
    //Wczytanie do pamięci komputera
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, filename);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    //Import do pamięci karty graficznej
    glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    return tex;
}

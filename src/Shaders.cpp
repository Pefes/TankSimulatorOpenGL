#include "Shaders.h"


const char *const vertexShaderCode = {
    "#version 330\n"

    "uniform mat4 M;"
    "uniform mat4 V;"
    "uniform mat4 P;"
    "uniform vec4 lp_1;"
    "uniform vec4 lp_2;"

    //Atrybuty
    "in vec4 vertex;"    //wspolrzedne wierzcholka w przestrzeni modelu
    "in vec4 normal;" //wspolrzedne wektora normalnego w przestrzeni modelu
    "in vec2 texCoord0;" //wspolrzedna teksturowania

    //Zmienne interpolowane dla swiatla 1
    "out vec4 l_1;"
    "out vec4 n_1;"
    "out vec4 v_1;"

    //Zmienne interpolowane dla swiatla 2
    "out vec4 l_2;"
    "out vec4 n_2;"
    "out vec4 v_2;"

    "out vec2 iTexCoord0;"
    "out vec2 iTexCoord1;"

        "void main(void) {"

        "l_1=normalize(V*(lp_1-M*vertex));" //Wektor "do swiatla" w przestrzeni oka
        "n_1=normalize(V*M*normal);" //Wektor normalny w przestrzeni oka
        "v_1=normalize(vec4(0,0,0,1)-V*M*vertex);" //Wektor "do obserwatora" w przestrzeni oka

        "l_2=normalize(V*(lp_2-M*vertex));" //Wektor "do œwiatla" w przestrzeni oka
        "n_2=normalize(V*M*normal);" //Wektor normalny w przestrzeni oka
        "v_2=normalize(vec4(0,0,0,1)-V*M*vertex);" //Wektor "do obserwatora" w przestrzeni oka

        "iTexCoord0=texCoord0;" //Zapewnienie interpolacji wspólrzêdnych teksturowania podanych poprzez atrybut
        "iTexCoord1=(n_1.xy+1)/2;" //Wyliczenie wspólrzednych do environment mapping

        "gl_Position = P*V*M*vertex;"

        "}"
};

const char *const fragmentShaderCode = {
    "#version 330\n"

    //Zmienne interpolowane

    "in vec4 l_1;" //interpolowany wektor "do swiatla" w przestrzeni oka
    "in vec4 n_1;" //interpolowany wektor normalny w przestrzeni oka
    "in vec4 v_1;" //interpolowany wektor "do obserwatora" w przestrzeni oka

    "in vec4 l_2;" //interpolowany wektor "do swiatla" w przestrzeni oka
    "in vec4 n_2;" //interpolowany wektor normalny w przestrzeni oka
    "in vec4 v_2;" //interpolowany wektor "do obserwatora" w przestrzeni oka

    "in vec2 iTexCoord0;" //interpolowane wspolrzêdne teksturowania dla tekstury w textureMap0
    "in vec2 iTexCoord1;" //interpolowane wspolrzêdne teksturowania dla tekstury w textureMap1

    "uniform sampler2D textureMap0;" //Jednostka teksturujaca 0
    "uniform sampler2D textureMap1;" //Jednostka teksturujaca 1

    "out vec4 pixelColor;" //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

    "void main(void) {"
        "vec4 kd=mix(texture(textureMap0,iTexCoord0),texture(textureMap1,iTexCoord1),0.3);"  //Kolor materialu dla swiatla  rozpraszanego
        "vec4 ld=vec4(1,1,1,1);" //Kolor swiatla  rozpraszanego
        "vec4 ks=vec4(1,1,1,1);" //Kolor odbic materialu
        "vec4 ls=vec4(1,1,1,1);" //Kolor swiatla odbijanego

        "vec4 ml_1=normalize(l_1);"
        "vec4 mn_1=normalize(n_1);"
        "vec4 mv_1=normalize(v_1);"
        "vec4 mr_1=reflect(-ml_1,mn_1);" //Wektor kierunku odbicia w przestrzeni oka

        "vec4 ml_2=normalize(l_2);"
        "vec4 mn_2=normalize(n_2);"
        "vec4 mv_2=normalize(v_2);"
        "vec4 mr_2=reflect(-ml_2,mn_2);" //Wektor kierunku odbicia w przestrzeni oka

        "float nl_1=clamp(dot(mn_1,ml_1),0,1);" //cos kata pomiedzy wektorami n i l
        "float rv_1=pow(clamp(dot(mr_1,mv_1),0,1),25);" //cos kata pomiedzy wektorami r i v podniesiony do potegi (wykyadnik Phonga)

        "float nl_2=clamp(dot(mn_2,ml_2),0,1);" //cos kata pomiedzy wektorami n i l
        "float rv_2=pow(clamp(dot(mr_2,mv_2),0,1),25);" //cos kata pomiedzy wektorami r i v podniesiony do potegi (wykladnik Phonga)

        "pixelColor=vec4(kd.rgb*ld.rgb*nl_1+ks.rgb*ls.rgb*rv_1 + kd.rgb*ld.rgb*nl_2+ks.rgb*ls.rgb*rv_2, kd.a);"
    "}"
};

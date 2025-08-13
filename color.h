typedef struct Color
{
  float red, green, blue;
} Color;

Color color(float,float,float);
Color color_add(Color, Color);
Color color_sub(Color, Color);
Color color_mul(Color, Color);
Color color_smul(Color, float);

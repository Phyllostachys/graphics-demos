#include <glm/glm.hpp>

glm::vec3 triangleColor = glm::vec3(1.0f, 0.0f, 0.0f);
float time;
glm::vec4 outColor;
glm::vec3 light = glm::vec3(800.0, 450.0, 50.0);

float intersect(glm::vec3 ray, glm::vec3 dir, glm::vec3 center, float radius)
{
    glm::vec3 oc = center - ray;
    float dot_loc = glm::dot(dir, oc);
    float lsq = glm::dot(dir, dir);
    float discriminant = (dot_loc * dot_loc) - lsq * (glm::dot(oc, oc) - (radius * radius));

    if (discriminant < 0.0)
    {
        return -1.0;
    }

    float i0 = (-dot_loc - glm::sqrt(discriminant)) / lsq;
    if (i0 >= 0.0)
    {
        return i0;
    }
    float i1 = (-dot_loc + glm::sqrt(discriminant)) / lsq;
    return i1;
}

int main()
{
    glm::vec4 gl_FragCoord = glm::vec4(780.0f, 490.0f, 0.0, 1.0f);
    outColor = glm::vec4(triangleColor, 1.0f);
    glm::vec3 spherePos = glm::vec3(800.0f, 450.0f, -20.0f);
    float sphereRadius = 50.0;
    light.x = light.x + 100.0f * glm::sin(glm::mod(time, 60.0f) * 3.14f);
    //light.y = light.y + 100 * cos(mod(time, 60.0) * 3.14);
    light.z = light.z + 10.0f * glm::sin(glm::mod(time, 60.0f) * 3.14f) + 10.0f * glm::cos(glm::mod(time, 60.0f) * 3.14f);

    if (glm::length(glm::vec3(gl_FragCoord.x - spherePos.x, gl_FragCoord.y - spherePos.y, gl_FragCoord.z - spherePos.z)) > sphereRadius)
    {
        outColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        return 0;
    }

    glm::vec3 rayOrigin = glm::vec3(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z);
    rayOrigin.z = 0.0f;
    glm::vec3 rayDir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));

    float t = intersect(rayOrigin, rayDir, spherePos, sphereRadius);
    if (t < 0.0) {
        outColor = 0.5f * glm::vec4(triangleColor, 1.0f);
        return 0;
    }

    glm::vec3 intersectionPoint = rayOrigin + t * rayDir;
    glm::vec3 sphereNormal = glm::normalize(intersectionPoint - spherePos);
    rayDir = glm::normalize(rayOrigin - intersectionPoint);
    rayDir = glm::reflect(rayDir, sphereNormal);
    float intensity = glm::dot(sphereNormal, light - intersectionPoint);

    outColor = glm::vec4(intensity * triangleColor, 1.0f);

    return 0;
}

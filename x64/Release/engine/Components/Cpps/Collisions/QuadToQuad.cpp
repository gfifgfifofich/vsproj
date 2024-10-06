
#include "../../Include/Helper.h"
#include "../../Include/Collisions/QuadToQuad.h"

void QuadToQuadCollision(cube* quad1, cube* quad2)
{
    if (quad1->position.x + quad1->width < quad2->position.x - quad2->width ||
        quad1->position.x - quad1->width > quad2->position.x + quad2->width ||
        quad1->position.y + quad1->height < quad2->position.y - quad2->height ||
        quad1->position.y - quad1->height > quad2->position.y + quad2->height)
        return;
    //else Colided

    bool plane1;// y = x
    bool plane2;// y =-x;
    glm::vec2 relpos = quad2->position - quad1->position;
    relpos.x *= (quad1->height + quad2->height) / (quad1->width + quad2->width);
    if (relpos.y < relpos.x)
        plane1 = false;// under
    else
        plane1 = true; // above
    if (relpos.y < -relpos.x)
        plane2 = false;// under
    else
        plane2 = true; // above

    if (plane1 && plane2)
    {
        quad1->position.y = quad2->position.y - quad2->height - quad1->height;
        quad1->CollisionSides[0] = true;
    }
    if (plane1 && !plane2)
    {
        quad1->position.x = quad2->position.x + quad2->width + quad1->width;
        quad1->CollisionSides[3] = true;
    }
    if (!plane1 && plane2)
    {
        quad1->position.x = quad2->position.x - quad2->width - quad1->width;
        quad1->CollisionSides[1] = true;
    }
    if (!plane1 && !plane2)
    {
        quad1->position.y = quad2->position.y + quad2->height + quad1->height;
        quad1->CollisionSides[2] = true;
    }
}
bool QuadToQuadCollisionCheck(cube* quad1, cube* quad2)
{
    if (quad1->position.x + quad1->width < quad2->position.x - quad2->width ||
        quad1->position.x - quad1->width > quad2->position.x + quad2->width ||
        quad1->position.y + quad1->height < quad2->position.y - quad2->height ||
        quad1->position.y - quad1->height > quad2->position.y + quad2->height)
        return false;
    return true;
}
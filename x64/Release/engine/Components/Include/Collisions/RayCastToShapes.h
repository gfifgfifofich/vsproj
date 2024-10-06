#pragma once
#include "../Objects.h"
#include "BallToBall.h"
#include "BallToLine.h"
#include "BallToPolygon.h"
#include "CircleToQuad.h"
bool CastRayToQuad(Ray* r, cube c);
bool CastRayToBall(Ray* r, ball b);
bool CastRayToTriangle(Ray* r, Triangle t);
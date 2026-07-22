#ifndef COLISAO_H
#define COLISAO_H

/*
Cabeçalho da colisão
*/

struct AABB {
    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;
};

inline bool verifyCollision(const AABB& a, const AABB& b) {
    return (a.maxX > b.minX && a.minX < b.maxX) &&
           (a.maxY > b.minY && a.minY < b.maxY) &&
           (a.maxZ > b.minZ && a.minZ < b.maxZ);
}

inline AABB createAABB(float posX, float posY, float posZ,
                      float halfW, float halfH, float halfD) {
    return { posX - halfW, posX + halfW,
             posY - halfH, posY + halfH,
             posZ - halfD, posZ + halfD };
}

#endif
